#include "mem.h"

struct mem *mem;

/* Initizializes memory and sets High RAM */
void mem_init(struct mem *GBmem) {
    mem = GBmem;

    mem->main[0XFF00] = 0XCF;
    mem->main[0XFF01] = 0X00;
    mem->main[0XFF02] = 0X7E;
    mem->main[0XFF04] = 0X00;
    mem->main[0XFF06] = 0X00;
    mem->main[0XFF07] = 0XF8;
    mem->main[0XFF0F] = 0XE1;
    mem->main[0XFF10] = 0X80;
    mem->main[0XFF11] = 0XBF;
    mem->main[0XFF12] = 0XF3;
    mem->main[0XFF13] = 0XFF;
    mem->main[0XFF14] = 0XBF;
    mem->main[0XFF16] = 0X3F;
    mem->main[0XFF17] = 0X00;
    mem->main[0XFF18] = 0XFF;
    mem->main[0XFF19] = 0XBF;
    mem->main[0XFF1A] = 0X7F;
    mem->main[0XFF1B] = 0XFF;
    mem->main[0XFF1C] = 0X9F;
    mem->main[0XFF1D] = 0XFF;
    mem->main[0XFF1E] = 0XBF;
    mem->main[0XFF20] = 0XFF;
    mem->main[0XFF21] = 0X00;
    mem->main[0XFF22] = 0X00;
    mem->main[0XFF23] = 0XBF;
    mem->main[0XFF24] = 0X77;
    mem->main[0XFF25] = 0XF3;
    mem->main[0XFF26] = 0XF1;
    mem->main[0XFF40] = 0X91;
    mem->main[0XFF41] = 0X85;
    mem->main[0XFF42] = 0X00;
    mem->main[0XFF43] = 0X00;
    mem->main[0XFF44] = 0X00;
    mem->main[0XFF45] = 0X00;
    mem->main[0XFF46] = 0XFF;
    mem->main[0XFF47] = 0XFC;
    mem->main[0XFF48] = 0X00;
    mem->main[0XFF49] = 0X00;
    mem->main[0XFF4A] = 0X00;
    mem->main[0XFF4B] = 0X00;
    mem->main[0XFF4D] = 0XFF;
    mem->main[0XFF4F] = 0XFF;
    mem->main[0XFF51] = 0XFF;
    mem->main[0XFF52] = 0XFF;
    mem->main[0XFF53] = 0XFF;
    mem->main[0XFF54] = 0XFF;
    mem->main[0XFF55] = 0XFF;
    mem->main[0XFF56] = 0XFF;
    mem->main[0XFF68] = 0XFF;
    mem->main[0XFF69] = 0XFF;
    mem->main[0XFF6A] = 0XFF;
    mem->main[0XFF6B] = 0XFF;
    mem->main[0XFF70] = 0XFF;
    mem->main[0XFFFF] = 0X00;
}

/* Loads correct mapper info */

/* Reads size number of bytes at addr returning uint16_t and setting status */
uint16_t mem_read(uint16_t addr, int size, bool *status) {
    uint16_t r;
    switch (size) {
        case 1: *status = true; r = mem->main[addr]; break;
        case 2: *status = true; r = mem->main[addr] | (mem->main[addr + 1] << 8); break;
        default: *status = false; r = 0; break;
    }

    return r;
}

/* Writes size number of bytes to addr with void return and status set */
void mem_write(uint16_t val, uint16_t addr, int size, bool *status) {
    switch (size) {
        case 1:
            *status = true;
            mem->main[addr] = val;
            break;
        case 2:
            *status = true;
            mem->main[addr] = (val & 0XFF);
            mem->main[addr + 1] = val >> 8;
            break;
        default:
            *status = false;
            break;
    }

    return;
}

uint8_t _mem_read(uint16_t addr, int size) {
    uint8_t data;

    if (addr >= 0X0000 && addr <= 0X3FFF) {
        // ROM bank 00
        data = mem->main[addr];
    } else if (addr >= 0X4000 && addr <= 0X7FFF) {
        // ROM bank 01-FF
        data = mem->main[addr];
    } else if (addr >= 0X8000 && addr <= 0X9FFF) {
        // Video RAM
        data = mem->main[addr];
    } else if (addr >= 0XA000 && addr <= 0XBFFF) {
        // Cartride RAM
        data = mem->main[addr];
    } else if (addr >= 0XC000 && addr <= 0XDFFF) {
        // Work RAM, CGB mode switchable banks

    }


    return data;
}
