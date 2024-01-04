#include "ppu.h"

struct ppu ppu;

struct ppu *get_ppu() {
    return &ppu;
}

uint32_t *get_display() {
    return (uint32_t *) ppu.display;
}

void ppu_init() {
    for (int y = 0; y < 144; y++) {
        for (int x = 0; x < 160; x++) {
            ppu.display[y][x][0] = 0;
            ppu.display[y][x][1] = 0;
            ppu.display[y][x][2] = 0;
        }
    }

    // Get LCDC regs
    ppu.LCDC = mem_pointer(mLCDC);
    ppu.LCDS = mem_pointer(mLCDS);

    // get viewport regs
    ppu.SCY = mem_pointer(mSCY);
    ppu.SCX = mem_pointer(mSCX);

    // get window regs
    ppu.WY = mem_pointer(mWY);
    ppu.WX = mem_pointer(mWX);

    // LY and LY compare
    ppu.LY = mem_pointer(mLY);
    ppu.LYC = mem_pointer(mLYC);

    // Palette
    ppu.PAL_bg = mem_pointer(mPAL);
    ppu.PAL_sp_0 = mem_pointer(mPAL_sp_0);
    ppu.PAL_sp_1 = mem_pointer(mPAL_sp_1);
}

int get_colour(int colID, uint8_t PAL) {
    int col;

    switch(colID) {
        case(0): col = ((PAL & 0b00000011) >> 0); break;
        case(1): col = ((PAL & 0b00001100) >> 2); break;
        case(2): col = ((PAL & 0b00110000) >> 4); break;
        case(3): col = ((PAL & 0b11000000) >> 6); break;
    }

    switch(col) {
        case(0): return 0XFF;
        case(1): return 0X88;
        case(2): return 0X44;
        case(3): return 0X00;
    }

    return 0;
}

void ppu_exec(int cycles) {
    ppu.ticks += cycles;

    ppu.start_mode = *ppu.LCDC & 0b00000011;
    ppu.interrupt_requested = false;

    // coincidence flag
    if (*ppu.LY == *ppu.LYC) {
        (*ppu.LCDS) = SET_BIT(*ppu.LCDS, 2);

        if (TEST_BIT(*ppu.LCDS, 6))
            interrupt_request(0b00000010);
    }

    switch(*ppu.LCDS & 0b00000011) {
        case(OAM_SEARCH_MODE): {
            if (ppu.ticks < 80) break;
            // clear previous sprites
            for (int i = 0; i < ppu.sprite_count; i++) {
                ppu.sprites[i] = (struct sprite) {0,0,0,0};
            }

            // reset sprite number
            ppu.sprite_count = 0;

            // OAM search routine
            for (int sprite_num = 0; sprite_num < 40 && ppu.sprite_count != 10; sprite_num++) {
                /* get all data from OAM if the object is on the current scanline  *
                 * and less than 10 objects have been found                        */
                uint8_t y_pos = mem_read(0XFE00 + 4 * sprite_num + 0);
                uint8_t x_pos = mem_read(0XFE00 + 4 * sprite_num + 1);
                uint8_t sprite_height = (TEST_BIT(*ppu.LCDC, 2)) ? 16: 8;

                // is sprite on the screen
                if (x_pos == 0)
                    continue;

                // does sprite overlap current scanline
                if (*ppu.LY + 16 < y_pos)
                    continue;

                // does sprite overlap current scanline
                if (*ppu.LY + 16 >= y_pos + sprite_height)
                    continue;

                ppu.sprites[ppu.sprite_count].y_pos = mem_read(0XFE00 + 4 * sprite_num + 0);
                ppu.sprites[ppu.sprite_count].x_pos = mem_read(0XFE00 + 4 * sprite_num + 1);
                ppu.sprites[ppu.sprite_count].tile_index = mem_read(0XFE00 + 4 * sprite_num + 2);
                ppu.sprites[ppu.sprite_count].attributes = mem_read(0XFE00 + 4 * sprite_num + 3);
                ppu.sprite_count++;
            }

            // end of OAM_SEARCH

            // change mode to PIXEL_TRANSFER
            (*ppu.LCDS) = RESET_BIT(*ppu.LCDS, 0);
            (*ppu.LCDS) = RESET_BIT(*ppu.LCDS, 1);

            (*ppu.LCDS) = SET_BIT(*ppu.LCDS, 0);
            (*ppu.LCDS) = SET_BIT(*ppu.LCDS, 1);

            ppu.interrupt_requested = TEST_BIT(*ppu.LCDS, 5);
            break;
        }
        case(PIXEL_TRANSFER_MODE): {
            if (ppu.ticks < 376) break;

            /************************* BACKGROUND AND WINDOW RENDERING ***************************/
            uint8_t tile_line, tile_id, lsb, msb;
            uint16_t tile_map_addr, tile_data_addr;
            bool usingWin, isSigned;
            int colourBit;

            // if within Window
            if (TEST_BIT(*ppu.LCDC, 5) && *ppu.WY <= *ppu.LY) {
                usingWin = true;

                // when window is on set the window tile
                if (TEST_BIT(*ppu.LCDC, 6)) tile_map_addr = 0X9C00  + (((uint8_t) *ppu.LY - *ppu.WY) / 8) * 32;
                else                        tile_map_addr = 0X9800  + (((uint8_t) *ppu.LY - *ppu.WY) / 8) * 32;

                tile_line = (*ppu.LY - *ppu.WY) % 8;
            } else {
                // if not in window use the background tile map
                usingWin = false;

                // get the tile map area
                // From the top left of the sceen (pos of SCY) get the current scanline down
                if (TEST_BIT(*ppu.LCDC, 3)) tile_map_addr = 0X9C00  + ((((uint8_t) (*ppu.SCY + *ppu.LY))/8) & 0XFF) * 32;
                else                        tile_map_addr = 0X9800  + ((((uint8_t) (*ppu.SCY + *ppu.LY))/8) & 0XFF) * 32;

                tile_line = (*ppu.SCY + *ppu.LY) % 8;
            }

            // get the tile data area
            if (TEST_BIT(*ppu.LCDC, 4)) {
                isSigned = false;
                tile_data_addr = 0X8000;
            } else {
                isSigned = true;
                tile_data_addr = 0X9000;
            }

            // set the starting variables

            // rendering all pixels on scanline
            for (int pix = 0; pix < 160; pix++) {
                // if completed a tile in the x-axis go to next tile
                // Account for SCROLLX, SCROLLX is the start pos of the screen
                uint8_t x_pos = *ppu.SCX + pix;

                // if the rendering is inside of the window get the xoffset from start of window
                if (usingWin) x_pos = pix - *ppu.WX + 7;

                // which 8bit wide tile
                uint8_t tile_col = (x_pos/8);

                // get the tile ID from background or window tile map
                tile_id = mem_read(tile_map_addr + tile_col);

                uint16_t lsb_addr, msb_addr;

                // get the addresses of the background bytes
                if (isSigned) {
                    // for signed addressing at 0X8800
                    lsb_addr = tile_data_addr + ((int8_t) tile_id) * 16 + tile_line * 2 + 0;
                    msb_addr = tile_data_addr + ((int8_t) tile_id) * 16 + tile_line * 2 + 1;
                } else {
                    // for unsigned addressing at 0X8000
                    lsb_addr = tile_data_addr + tile_id * 16 + tile_line * 2 + 0;
                    msb_addr = tile_data_addr + tile_id * 16 + tile_line * 2 + 1;
                }

                // read background bytes;
                if (!TEST_BIT(*ppu.LCDC, 0)) {
                    lsb = 0;
                    msb = 0;
                } else {
                    lsb = mem_read(lsb_addr);
                    msb = mem_read(msb_addr);
                }


                // which pixel from the tile we want
                colourBit = -(x_pos % 8 - 7);

                // contains palette ID
                int pixel = (TEST_BIT(msb, colourBit) << 1) | (TEST_BIT(lsb, colourBit));

                int colour = get_colour(pixel, *ppu.PAL_bg);

                ppu.display[*ppu.LY][pix][0] = colour;
                ppu.display[*ppu.LY][pix][1] = colour;
                ppu.display[*ppu.LY][pix][2] = colour;
            }
            /*************************************************************************************/
            /******************************** SPRITE RENDERING ***********************************/
            struct sprite sprite;

            // sprite height
            uint8_t sprite_height = (TEST_BIT(*ppu.LCDC, 2)) ? 16: 8;

            // loop through each sprite and render them on the current scanline
            for (int sprite_num = 0; sprite_num < ppu.sprite_count; sprite_num++) {
                sprite = ppu.sprites[sprite_num];

                uint16_t msb_addr, lsb_addr;

                uint8_t sprite_line = (*ppu.LY - (sprite.y_pos - 16));
                // test if flipped vertically
                if (TEST_BIT(sprite.attributes, 6)) {
                    sprite_line -= sprite_height;
                    sprite_line *= -1;
                }

                // get tile data from first tile for normal sprites
                lsb_addr = 0X8000 + sprite.tile_index * 16 + sprite_line * 2 + 0;
                msb_addr = 0X8000 + sprite.tile_index * 16 + sprite_line * 2 + 1;

                uint8_t lsb = mem_read(lsb_addr);
                uint8_t msb = mem_read(msb_addr);

                 // loop through all the 8 pixels of the fetched sprite
                 for (int pix = -7, pixel; pix <= 0; pix++) {
                     // if the pixel cannot be seen on the screen
                     if (sprite.x_pos + pix < 0 || sprite.x_pos + pix > 160)
                         continue;

                     // Test if flipped horizontally
                     if (TEST_BIT(sprite.attributes, 5)) pixel = (TEST_BIT(msb, 0) << 1) | (TEST_BIT(lsb, 0));
                     else                                pixel = (TEST_BIT(msb, 7) << 1) | (TEST_BIT(lsb, 7));

                     if (pixel != 0) {

                        int colour;
                        if (TEST_BIT(sprite.attributes, 4)) colour = get_colour(pixel, *ppu.PAL_sp_1);
                        else                                colour = get_colour(pixel, *ppu.PAL_sp_0);


                        // Test priority, the complement of (if the bit is set and the colour is 0) then render
                        uint8_t bg_col = get_colour(0, *ppu.PAL_bg);
                        if (!TEST_BIT(sprite.attributes, 7) || ppu.display[*ppu.LY][sprite.x_pos + pix - 1][0] == bg_col) {
                            ppu.display[*ppu.LY][sprite.x_pos + pix - 1][0] = colour;
                            ppu.display[*ppu.LY][sprite.x_pos + pix - 1][1] = colour;
                            ppu.display[*ppu.LY][sprite.x_pos + pix - 1][2] = colour;
                        }

                     }
                     // move the next pixel into place for reading
                     if (TEST_BIT(sprite.attributes, 5)) {
                         lsb >>= 1;
                         msb >>= 1;
                     } else {
                         lsb <<= 1;
                         msb <<= 1;
                     }
                 }
            }
            /*************************************************************************************/

            // end of pixel transfer
            // change mode to HBLANK
            (*ppu.LCDS) = RESET_BIT(*ppu.LCDS, 0);
            (*ppu.LCDS) = RESET_BIT(*ppu.LCDS, 1);

            break;
        }
        case(HBLANK): {
            // increment scanline when all ticks consumed
            if (ppu.ticks >= 456) {
                (*ppu.LY)++;
                ppu.ticks = 0;

                // coincidence flag
                if (*ppu.LY == *ppu.LYC)
                    (*ppu.LCDS) = SET_BIT(*ppu.LCDS, 2);

                if (*ppu.LY == 144) {
                    // change mode to VBLANK
                    (*ppu.LCDS) = RESET_BIT(*ppu.LCDS, 0);
                    (*ppu.LCDS) = RESET_BIT(*ppu.LCDS, 1);

                    (*ppu.LCDS) = SET_BIT(*ppu.LCDS, 0);
                    interrupt_request(0b00000001);
                } else {
                    // change mode to OAM_SEARCH
                    (*ppu.LCDS) = RESET_BIT(*ppu.LCDS, 0);
                    (*ppu.LCDS) = RESET_BIT(*ppu.LCDS, 1);

                    (*ppu.LCDS) = SET_BIT(*ppu.LCDS, 1);
                }

                ppu.interrupt_requested = TEST_BIT(*ppu.LCDS, 3);
            }
            break;
        }
        case(VBLANK): {
            if (ppu.ticks >= 456) {
                (*ppu.LY)++;
                ppu.ticks = 0;

                // End of VBLANK
                if (*ppu.LY == 153) {
                    (*ppu.LY) = 0;

                    // change mode to OAM_SEARCH
                    (*ppu.LCDS) = RESET_BIT(*ppu.LCDS, 0);
                    (*ppu.LCDS) = RESET_BIT(*ppu.LCDS, 1);

                    (*ppu.LCDS) = SET_BIT(*ppu.LCDS, 1);
                    ppu.interrupt_requested = TEST_BIT(*ppu.LCDS, 4);
                }

            }
            break;
        }
    }

    if (ppu.interrupt_requested && ppu.start_mode != (*ppu.LCDC & 0b00000011))
        interrupt_request(0b00000010);
}
