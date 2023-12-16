#include "ppu.h"

struct ppu ppu;

struct ppu *get_ppu() {return &ppu;}

// BUG: cannot get display for rendering
uint8_t ***ppu_get_display() {return (uint8_t ***) ppu.display;}

/* gets palette colours for pixels */
int get_colour(uint8_t colour_num) {
    uint8_t palette = mem_read(0XFF47);
    int hi, lo;

    switch(colour_num) {
        case 0: hi = 1; lo = 0; break;
        case 1: hi = 3; lo = 2; break;
        case 2: hi = 5; lo = 4; break;
        case 3: hi = 7; lo = 0; break;
    }

    return (TEST_BIT(palette, hi) << 1) | (TEST_BIT(palette, lo));
}

/* initializes ppu */
void ppu_init() {
    for (int x = 0; x < 160; x++) {
        for (int y = 0; y < 144; y++) {
            ppu.display[x][y][0] = 0;
            ppu.display[x][y][1] = 0;
            ppu.display[x][y][2] = 0;
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
    // ppu.WX = mem_pointer(0XFF4B); 1, &ppu.status_read) - 7;

    // LY and LY compare
    ppu.LY = mem_pointer(mLY);
    ppu.LYC = mem_pointer(mLYC);

    ppu.m_scanline_counter = 456;
}

/* renders background to scanline */
void ppu_render_background() {
    if (TEST_BIT(*ppu.LCDC, 5)) {
        if (*ppu.WY <= *ppu.LY)
            ppu.usingWin = true;
        else
            ppu.usingWin = false;
    }

    if (ppu.usingWin) {
        if (TEST_BIT(*ppu.LCDC, 3))
            ppu.background_mem = 0X9C00;
        else
            ppu.background_mem = 0X9800;
    } else {
        if (TEST_BIT(*ppu.LCDC, 6))
            ppu.background_mem = 0X9C00;
        else
            ppu.background_mem = 0X9800;
    }

    // offsets by scrollY
    uint8_t Ypos = *ppu.LY + *ppu.SCY;
    uint8_t tile_row = ((uint8_t)(Ypos / 8)) * 32;

    // loops through each pixel in scanline
    for (int pixel = 0; pixel < 160; pixel++) {
        // offsets by scrollX
        uint8_t Xpos = pixel + *ppu.SCX;

        // if using windows
        if (TEST_BIT(*ppu.LCDC, 5)) {
            if (pixel >= *ppu.WX - 7) {
                Xpos = pixel - *ppu.WX - 7;
            }
        }

        uint8_t tile_col = (uint8_t) Xpos/8;

        // get the tile number from the tile map
        uint8_t tile_index = mem_read(ppu.background_mem + tile_row + tile_col);

        // get row of the tile
        uint8_t line = 2 * (Ypos % 8);
        uint16_t tile_addr = ADDR(tile_index, TEST_BIT(*ppu.LCDC, 4)) + line;

        // get tile data
        uint16_t tile_data_1 = mem_read(tile_addr);
        uint16_t tile_data_2 = mem_read(tile_addr + 1);

        int colourbit = Xpos % 8;
        colourbit -= 7;
        colourbit *= -1;

        int colournum = (TEST_BIT(tile_data_2, colourbit)) | (TEST_BIT(tile_data_1, colourbit));


        int red = 0;
        int blue = 0;
        int green = 0;

        switch(get_colour(colournum)) {
            case 0: red = 0XFF; blue = 0XFF; green = 0XFF; break; // WHITE
            case 1: red = 0XCC; blue = 0XCC; green = 0XCC; break; // LIGHT GREY
            case 2: red = 0X77; blue = 0X77; green = 0X77; break; // DARK GREY
        }

        int final_y = mem_read(0XFF44);

        if ((final_y < 0 || final_y > 143) || (pixel < 0 || pixel > 159))
            continue;

        ppu.display[pixel][final_y][0] = red;
        ppu.display[pixel][final_y][1] = green;
        ppu.display[pixel][final_y][2] = blue;
    }
}

/* renders sprites to scanline */
void ppu_render_sprites() {

}

void LCD_status() {
    if (!TEST_BIT(*ppu.LCDC, 7)) {
        // If the display is turened off then set the mode to 0X01
        ppu.m_scanline_counter = 456;
        *ppu.LY = 0;

        // keep all BITs execpt mode specifiers
        *ppu.LCDS &= 0b11111100;
        *ppu.LCDS = SET_BIT(*ppu.LCDS, 0);
        return;
    }

    uint8_t cur_mode = *ppu.LCDS & 0b00000011;
    uint8_t mode = 0;
    bool req_interrupt = false;

    if (*ppu.LY >= 144) {
        // mode 0X01: Vertical Blank
        mode = 1;
        *ppu.LCDS = SET_BIT(*ppu.LCDS, 0);
        *ppu.LCDS = RESET_BIT(*ppu.LCDS, 1);
        req_interrupt = TEST_BIT(*ppu.LCDS, 4);
    } else {
        if (ppu.m_scanline_counter > 456 - 80) {
            // mode 0X02: OAM scan sequence
            mode = 2;
            *ppu.LCDS = RESET_BIT(*ppu.LCDS, 0);
            *ppu.LCDS = SET_BIT(*ppu.LCDS, 1);
            req_interrupt = TEST_BIT(*ppu.LCDS, 5);
        } else if (ppu.m_scanline_counter > 456 - 172 - 80) {
            // mode 0X03: Drawing pixels
            mode = 3;
            *ppu.LCDS = SET_BIT(*ppu.LCDS, 0);
            *ppu.LCDS = SET_BIT(*ppu.LCDS, 1);
        } else {
            // mode 0X00: Horizontal Blank
            mode = 0;
            *ppu.LCDS = RESET_BIT(*ppu.LCDS, 0);
            *ppu.LCDS = RESET_BIT(*ppu.LCDS, 1);
            req_interrupt = TEST_BIT(*ppu.LCDS, 3);
        }

        // if mode has changed request interrupt
        if (req_interrupt && (mode != cur_mode)) {
            interrupt_request(0b00000010);
        }

        // check coincidence flag
        if (*ppu.LYC == *ppu.LY) {
            *ppu.LCDS = SET_BIT(*ppu.LCDS, 2);

            // if interrupt is set for LY == LYC
            if (TEST_BIT(*ppu.LCDS, 6)) {
                interrupt_request(0b00000010);
            }
        } else {
            *ppu.LCDS = RESET_BIT(*ppu.LCDS, 2);
        }
    }
}

void ppu_exec(int oldtime, int currenttime) {
    int cycles = currenttime - oldtime;

    // set LCD status
    LCD_status();

    // if LCD is on
    if (TEST_BIT(*ppu.LCDC, 7))
        ppu.m_scanline_counter -= cycles;
    else
        return;

    if (ppu.m_scanline_counter > 0)
        return;

    (*ppu.LY)++;
    ppu.m_scanline_counter = 456;

    if (*ppu.LY < 144)
        // render scanline
        ppu_render_background();

    else if (*ppu.LY == 144)
        // VBlank interrupt
        interrupt_request(0b00000001);
    else if (*ppu.LY > 153)
        // reset LY
        *ppu.LY = 0X00;

    return;
}
