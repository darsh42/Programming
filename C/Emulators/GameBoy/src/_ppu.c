#include "ppu.h"

struct ppu ppu;
struct fetcher fetcher;

struct ppu *get_ppu() {
    return &ppu;
}

uint32_t *get_display() {
    return (uint32_t *) ppu.display;
}

void fetcher_start() {
    fetcher.ticks = 0;
    for (int i = 0; i < 16; i++)
        fetcher.FIFO_back[i] = 0;
    fetcher.tile_num = 0;
    // get current tile line
    fetcher.tile_line = *ppu.LY % 8;
    // get correct tile map ID based on which scanline is rendered
    //                      base addr      scanline offset
    fetcher.tile_map_addr = 0X9800 + ((uint16_t) *ppu.SCY/8) * 32 + ((uint16_t) *ppu.LY/8) * 32;
    // get correct tile data location
    fetcher.tile_data_addr = 0X8000;
    // set the initial fetcher mode
    fetcher.mode = 0;
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

void fetcher_exec() {
    fetcher.ticks++;
    if (fetcher.ticks < 2) return;

    fetcher.ticks = 0;
    switch(fetcher.mode) {
        case(0):
            // Read the tile ID
            fetcher.tile_id = mem_read(fetcher.tile_map_addr + fetcher.tile_num);
            fetcher.mode++;
            break;
        case(1): {
            // READ LSB
            uint16_t offset = fetcher.tile_num * 16 + fetcher.tile_line * 2;
            uint16_t addr = fetcher.tile_data_addr + offset + 0;
            fetcher.lsb = mem_read(addr);

            fetcher.mode++;
            break;
        }
        case(2): {
            // READ MSB
            uint16_t offset = fetcher.tile_num * 16 + fetcher.tile_line * 2;
            uint16_t addr = fetcher.tile_data_addr + offset + 1;
            fetcher.msb = mem_read(addr);
            fetcher.mode++;
            break;
        }
        case(3):
            // PUSH TO FIFO
            // if there is space for 8 pixels
            if(fetcher.back_index % 8 == 7) {
                uint8_t base = (fetcher.back_index + 9) % 16;
                for (int pixel_pos = 0; pixel_pos < 8; pixel_pos++) {
                    int pixel = (TEST_BIT(fetcher.msb, 7) << 1) | TEST_BIT(fetcher.lsb, 7);
                    fetcher.FIFO_back[base + pixel_pos] = pixel;
                    fetcher.msb <<= 1;
                    fetcher.lsb <<= 1;
                }

                fetcher.tile_num++;
            }

            fetcher.mode = 0;
            break;
    }
}

void ppu_exec() {
    ppu.ticks++;

    switch(*ppu.LCDS & 0b00000011) {
        case(OAM_SEARCH_MODE):
            // end of OAM_SEARCH
            if (ppu.ticks == 40) {
                ppu.pixel = 0;
                /* RESET FETCHER TODO: fix implementation for LCDC flags */
                fetcher_start();
                // change mode to PIXEL_TRANSFER
                (*ppu.LCDS) = RESET_BIT(*ppu.LCDS, 0);
                (*ppu.LCDS) = RESET_BIT(*ppu.LCDS, 1);

                (*ppu.LCDS) = SET_BIT(*ppu.LCDS, 0);
                (*ppu.LCDS) = SET_BIT(*ppu.LCDS, 1);
            }
            break;
        case(PIXEL_TRANSFER_MODE):
            fetcher_exec();

            // get pixel
            int colour = get_colour(fetcher.FIFO_back[fetcher.back_index]);

            // put colour for RGB channels
            ppu.display[*ppu.LY][ppu.pixel][0] = colour;
            ppu.display[*ppu.LY][ppu.pixel][1] = colour;
            ppu.display[*ppu.LY][ppu.pixel][2] = colour;

            // increment index but keep index within range
            fetcher.back_index++;
            fetcher.back_index %= 16;

            // end of pixel transfer
            if (++ppu.pixel == 160) {
                // change mode to HBLANK
                (*ppu.LCDS) = RESET_BIT(*ppu.LCDS, 0);
                (*ppu.LCDS) = RESET_BIT(*ppu.LCDS, 1);
            }

            break;
        case(HBLANK):
            // increment scanline when all ticks consumed
            if (ppu.ticks == 456) {
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
            if (ppu.ticks == 456) {
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
