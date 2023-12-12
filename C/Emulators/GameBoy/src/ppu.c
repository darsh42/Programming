#include "ppu.h"

struct ppu *ppu;

int get_colour(struct ppu *ppu, uint8_t colour_num) {
    uint8_t palette = mem_read(0XFF47, 1, &ppu->status_read);
    int hi, lo;

    switch(colour_num) {
        case 0: hi = 1; lo = 0; break;
        case 1: hi = 3; lo = 2; break;
        case 2: hi = 5; lo = 4; break;
        case 3: hi = 7; lo = 0; break;
    }

    return (GET_BIT(palette, hi) << 1) | (GET_BIT(palette, lo));
}

void ppu_init(struct ppu *GBppu) {
    ppu = GBppu;
    for (int x = 0; x < 160; x++) {
        for (int y = 0; y < 144; y++) {
            ppu->display[x][y][0] = 0;
            ppu->display[x][y][1] = 0;
            ppu->display[x][y][2] = 0;
        }
    }
}

void ppu_exec(struct ppu *ppu) {
    // Get LCDC regs
    ppu->LCDC = mem_read(0XFF40, 1, &ppu->status_read);
    ppu->LCDS = mem_read(0XFF41, 1, &ppu->status_read);

    // get viewport regs
    ppu->SCY = mem_read(0XFF42, 1, &ppu->status_read);
    ppu->SCX = mem_read(0XFF43, 1, &ppu->status_read);

    // get window regs
    ppu->WY = mem_read(0XFF4A, 1, &ppu->status_read);
    ppu->WX = mem_read(0XFF4B, 1, &ppu->status_read) - 7;

    // LY and LY compare
    ppu->LY = mem_read(0XFF44, 1, &ppu->status_read);
    ppu->LYC = mem_read(0XFF45, 1, &ppu->status_read);

    if (TEST_BIT(ppu->LCDC, 5)) {
        if (ppu->WY <= ppu->LY)
            ppu->usingWin = true;
        else
            ppu->usingWin = false;
    }

    if (ppu->usingWin) {
        if (TEST_BIT(ppu->LCDC, 3))
            ppu->background_mem = 0X9C00;
        else
            ppu->background_mem = 0X9800;
    } else {
        if (TEST_BIT(ppu->LCDC, 6))
            ppu->background_mem = 0X9C00;
        else
            ppu->background_mem = 0X9800;
    }

    // offsets by scrollY
    uint8_t Ypos = ppu->LY + ppu->SCY;
    uint8_t tile_row = ((uint8_t)(Ypos / 8)) * 32;

    // loops through each pixel in scanline
    for (int pixel = 0; pixel < 160; pixel++) {
        // offsets by scrollX
        uint8_t Xpos = pixel + ppu->SCX;

        // if using windows
        if (TEST_BIT(ppu->LCDC, 5)) {
            if (pixel >= ppu->WX) {
                Xpos = pixel - ppu->WX;
            }
        }

        uint8_t tile_col = (uint8_t) Xpos/8;

        // get the tile number from the tile map
        uint8_t tile_index = mem_read(ppu->background_mem + tile_row + tile_col, 1, &ppu->status_read);

        // get row of the tile
        uint8_t line = 2 * (Ypos % 8);
        uint16_t tile_addr = ADDR(tile_index, TEST_BIT(ppu->LCDC, 4)) + line;

        // get tile data
        uint16_t tile_data_1 = mem_read(tile_addr, 1, &ppu->status_read);
        uint16_t tile_data_2 = mem_read(tile_addr + 1, 1, &ppu->status_read);

        int colourbit = Xpos % 8;
        colourbit -= 7;
        colourbit *= -1;

        int colournum = (GET_BIT(tile_data_2, colourbit)) | (GET_BIT(tile_data_1, colourbit));


        int red = 0;
        int blue = 0;
        int green = 0;

        switch(get_colour(ppu, colournum)) {
            case 0: red = 0XFF; blue = 0XFF; green = 0XFF; break; // WHITE
            case 1: red = 0XCC; blue = 0XCC; green = 0XCC; break; // LIGHT GREY
            case 2: red = 0X77; blue = 0X77; green = 0X77; break; // DARK GREY
        }

        int final_y = mem_read(0XFF44, 1, &ppu->status_read);

        if ((final_y < 0 || final_y > 143) || (pixel < 0 || pixel > 159))
            continue;

        ppu->display[pixel][final_y][0] = red;
        ppu->display[pixel][final_y][1] = green;
        ppu->display[pixel][final_y][2] = blue;
    }
}
