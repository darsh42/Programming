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
    ppu.PAL = mem_pointer(mPAL);
}

int get_colour(int colID) {
    int col;
    switch(colID) {
        case(0): col = ((*ppu.PAL & 0b00000011) >> 0); break;
        case(1): col = ((*ppu.PAL & 0b00001100) >> 2); break;
        case(2): col = ((*ppu.PAL & 0b00110000) >> 4); break;
        case(3): col = ((*ppu.PAL & 0b11000000) >> 6); break;
    }

    switch(col) {
        case(0): return 0XFF;
        case(1): return 0XAA;
        case(2): return 0X77;
        case(3): return 0X22;
    }

    return 0;
}

void ppu_exec(int cycles) {
    ppu.ticks += cycles;

    switch(*ppu.LCDS & 0b00000011) {
        case(OAM_SEARCH_MODE):
            // OAM search routine


            // end of OAM_SEARCH
            if (ppu.ticks >= 80) {
                // change mode to PIXEL_TRANSFER
                (*ppu.LCDS) = RESET_BIT(*ppu.LCDS, 0);
                (*ppu.LCDS) = RESET_BIT(*ppu.LCDS, 1);

                (*ppu.LCDS) = SET_BIT(*ppu.LCDS, 0);
                (*ppu.LCDS) = SET_BIT(*ppu.LCDS, 1);
            }
            break;
        case(PIXEL_TRANSFER_MODE): {
            uint8_t tile_line, tile_id, lsb, msb;
            uint16_t tile_map_addr, tile_data_addr;
            bool usingWin, isSigned;


            // if within Window BUG: Removed scolling for cpu testing puposes
            if (TEST_BIT(*ppu.LCDC, 5) && *ppu.WY < *ppu.LY) {
                usingWin = true;

                // when window is on set the window tile
                // tile_map_addr = 0X9C00  + ((((uint16_t) (*ppu.LY + *ppu.WX))/8) & 0XFF) * 32;
                if (TEST_BIT(*ppu.LCDC, 6)) {
                    tile_map_addr = 0X9C00  + ((((uint16_t) (*ppu.LY))/8) & 0XFF) * 32;
                } else {
                    tile_map_addr = 0X9800  + ((((uint16_t) (*ppu.LY))/8) & 0XFF) * 32;
                }
            } else {
                // if not in window use the background tile map
                usingWin = false;

                // get the tile map area
                if (TEST_BIT(*ppu.LCDC, 3)) {
                    tile_map_addr = 0X9C00  + ((((uint16_t) (*ppu.LY))/8) & 0XFF) * 32;
                } else {
                    tile_map_addr = 0X9800  + ((((uint16_t) (*ppu.LY))/8) & 0XFF) * 32;
                }
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
            tile_line = *ppu.LY % 8;

            // rendering all pixels on scanline
            for (int pix = 0; pix < 160; pix++) {
                // if completed a tile in the x-axis go to next tile
                if (pix % 8 == 0) {
                    // Account for SCROLLX
                    uint8_t xoffset = (pix + *ppu.SCX)/8;

                    // if the rendering is inside of the window get the xoffset from start of window
                    if (usingWin) {
                        xoffset -= *ppu.WX;
                    }

                    // which 8bit wide tile
                    uint8_t tile_col = xoffset & 0X1f;

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
                    lsb = mem_read(lsb_addr);
                    msb = mem_read(msb_addr);

                }

                // contains palette ID
                int pixel = (TEST_BIT(msb, 7) << 1) | (TEST_BIT(lsb, 7));

                int colour = get_colour(pixel);

                ppu.display[*ppu.LY][pix][0] = colour;
                ppu.display[*ppu.LY][pix][1] = colour;
                ppu.display[*ppu.LY][pix][2] = colour;

                // move the next pixel into place for reading (ALWAYS reading bit 7)
                lsb <<= 1;
                msb <<= 1;
            }

            // end of pixel transfer
            // change mode to HBLANK
            (*ppu.LCDS) = RESET_BIT(*ppu.LCDS, 0);
            (*ppu.LCDS) = RESET_BIT(*ppu.LCDS, 1);

            break;
        }
        case(HBLANK):
            // increment scanline when all ticks consumed
            if (ppu.ticks >= 456) {
                (*ppu.LY)++;
                ppu.ticks = 0;

                if (*ppu.LY == 144) {
                    // change mode to VBLANK
                    (*ppu.LCDS) = RESET_BIT(*ppu.LCDS, 0);
                    (*ppu.LCDS) = RESET_BIT(*ppu.LCDS, 1);

                    (*ppu.LCDS) = SET_BIT(*ppu.LCDS, 0);
                } else {
                    // change mode to OAM_SEARCH
                    (*ppu.LCDS) = RESET_BIT(*ppu.LCDS, 0);
                    (*ppu.LCDS) = RESET_BIT(*ppu.LCDS, 1);

                    (*ppu.LCDS) = SET_BIT(*ppu.LCDS, 1);
                }
            }
            break;
        case(VBLANK):
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
                }

            }
            break;
    }
}
