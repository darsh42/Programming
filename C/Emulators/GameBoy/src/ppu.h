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

    // Palette
    uint8_t *PAL;

    // Window
    bool usingWin;

    // misc
    uint16_t tile_data_base_addr;
    bool isSigned;

    uint16_t tile_index_base_addr;

    int m_scanline_counter;


    // new impl
    int ticks;
    int pixel;
};

struct fetcher {
    // fetch clock
    int ticks;

    /*
    ** fetcher mode
    ** - 0b00 - read tile ID
    ** - 0b01 - read tile lsb
    ** - 0b10 - read tile msb
    ** - 0b11 - push to FIFO
    */
    int mode;

    // store tile number
    int tile_num;
    int tile_id;

    uint8_t lsb;
    uint8_t msb;

    // store pixel data
    int FIFO_back[16];
    int FIFO_sprite[16];

    // keep FIFO indexes
    int back_index;
    int sprite_index;

    // store tile map and tile data base addresses
    uint16_t tile_map_addr;
    uint16_t tile_data_addr;

    uint8_t tile_line;
};

/* Interrupt functions */
void interrupt_request(uint8_t request);

/* Memory functions */
extern uint8_t *mem_pointer(uint16_t addr);
extern uint8_t mem_read(uint16_t addr);
extern void mem_write(uint16_t addr, uint8_t data);

#endif // PPU_H_INCLUDED
