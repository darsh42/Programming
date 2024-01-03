#ifndef PPU_H_INCLUDED
#define PPU_H_INCLUDED

#include "common.h"

#define   SIGNED_ADDR(tile_num)  ((int8_t) tile_num * 16 + 0X8800)
#define UNSIGNED_ADDR(tile_num) ((uint8_t) tile_num * 16 + 0X8000)
#define TILEDATA_ADDR(tile_num, mode) (mode) ? SIGNED_ADDR(tile_num): UNSIGNED_ADDR(tile_num)
#define TILEINDEX_ADDR(a) (a) ? 0X9C00 : 0X9800

#define OAM_ENTRY_SIZE_BYTES 4
#define OAM_SEARCH_MODE 2
#define PIXEL_TRANSFER_MODE 3
#define HBLANK 0
#define VBLANK 1

extern int sdl_render();

struct sprite {
    /*
    ** One OAM sprite entry
    ** y_pos -> stores objects vertical position + 16
    ** x_pos -> stores objects horizontal position + 8
    ** tile_index -> stores the tile index of the sprite
    ** attributes -> stores object attributes
    */
    uint8_t y_pos;
    uint8_t x_pos;
    uint8_t tile_index;
    uint8_t attributes;
};


struct ppu {
    /*
    ** Display
    */
    uint8_t display[144][160][3];

    /*
    ** Registers
    ** - Determine Viewport
    ** - Window overlay
    */
    uint8_t *SCX, *SCY;
    uint8_t *WX, *WY;

    // LY - Scanline
    uint8_t *LY;
    uint8_t *LYC;

    /*
    ** LCD Control register:
    **   7 - LCD & PPU enable, 0 = off, 1 = on
    **   6 - Window tile map area, 0 = 0X9800 - 0X9BFF, 1 = 0X8000 - 0X8FFF
    **   5 - Window Enable - 0 = off, 1 = on
    **   4 - BG & Window tiles - 0 = 0X8800 - 0X97FF, 1 = 0X8000 - 0X8FFF
    **   3 - BG Tile map - 0 = 0X9800 - 0X9BFF, 1 = 0X9C00 - 0X9FFF
    **   2 - OBJ size - 0 = 8x8, 1 = 8x16
    **   1 - OBJ enable - 0 = off, 1 = on
    **   0 - BG & Window enable/priority - 0 = off, 1 = on
    */
    uint8_t *LCDC;

    /*
    ** LCD Status register:
    **   Defines conditions for STAT interrupts
    **   7 -
    **   6 - LYC int select
    **   5 - Mode 2 int select
    **   4 - Mode 1 int select
    **   3 - Mode 0 int select
    **   2 - LYC == LY
    **   1 - ppu mode
    **   0 - ppu mode
    */
    uint8_t *LCDS;

    // OAM variables
    int sprite_count;
    struct sprite sprites[10];

    // Palette
    uint8_t *PAL_bg;
    uint8_t *PAL_sp_0;
    uint8_t *PAL_sp_1;

    // ppu clock
    int ticks;

    // starting ppu render mode
    uint8_t start_mode;
    bool interrupt_requested;
};

/* Interrupt functions */
void interrupt_request(uint8_t request);

/* Memory functions */
extern uint8_t *mem_pointer(uint16_t addr);
extern uint8_t mem_read(uint16_t addr);
extern void mem_pixel_transfer(bool state);

// render function
extern int sdl_render();
#endif // PPU_H_INCLUDED
