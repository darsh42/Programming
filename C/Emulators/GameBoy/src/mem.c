#include "mem.h"
#include <stdint.h>

struct mem mem;

struct mem *get_mem() {return &mem;}


/* Initizializes memory and sets High RAM */
void mem_init() {
    // Set default ROM bank
    mem.ROM_bank_number = 0X01;

    /* Setting High RAM */
    mem.main[0XFF00] = 0XCF; mem.main[0XFF01] = 0X00;
    mem.main[0XFF02] = 0X7E; mem.main[0XFF04] = 0X00;
    mem.main[0XFF06] = 0X00; mem.main[0XFF07] = 0XF8;
    mem.main[0XFF0F] = 0XE1; mem.main[0XFF10] = 0X80;
    mem.main[0XFF11] = 0XBF; mem.main[0XFF12] = 0XF3;
    mem.main[0XFF13] = 0XFF; mem.main[0XFF14] = 0XBF;
    mem.main[0XFF16] = 0X3F; mem.main[0XFF17] = 0X00;
    mem.main[0XFF18] = 0XFF; mem.main[0XFF19] = 0XBF;
    mem.main[0XFF1A] = 0X7F; mem.main[0XFF1B] = 0XFF;
    mem.main[0XFF1C] = 0X9F; mem.main[0XFF1D] = 0XFF;
    mem.main[0XFF1E] = 0XBF; mem.main[0XFF20] = 0XFF;
    mem.main[0XFF21] = 0X00; mem.main[0XFF22] = 0X00;
    mem.main[0XFF23] = 0XBF; mem.main[0XFF24] = 0X77;
    mem.main[0XFF25] = 0XF3; mem.main[0XFF26] = 0XF1;
    mem.main[0XFF40] = 0X91; mem.main[0XFF41] = 0X85;
    mem.main[0XFF42] = 0X00; mem.main[0XFF43] = 0X00;
    mem.main[0XFF44] = 0X00; mem.main[0XFF45] = 0X00;
    mem.main[0XFF46] = 0XFF; mem.main[0XFF47] = 0XFC;
    mem.main[0XFF48] = 0X00; mem.main[0XFF49] = 0X00;
    mem.main[0XFF4A] = 0X00; mem.main[0XFF4B] = 0X00;
    mem.main[0XFF4D] = 0XFF; mem.main[0XFF4F] = 0XFF;
    mem.main[0XFF51] = 0XFF; mem.main[0XFF52] = 0XFF;
    mem.main[0XFF53] = 0XFF; mem.main[0XFF54] = 0XFF;
    mem.main[0XFF55] = 0XFF; mem.main[0XFF56] = 0XFF;
    mem.main[0XFF68] = 0XFF; mem.main[0XFF69] = 0XFF;
    mem.main[0XFF6A] = 0XFF; mem.main[0XFF6B] = 0XFF;
    mem.main[0XFF70] = 0XFF; mem.main[0XFFFF] = 0X00;
}

/*
** Load GameBoy ROM
** Load correct mapper info
*/
bool cart_has_RAM(uint8_t cart_type) {
    switch (cart_type) {
        case(0X02):
        case(0X03):
            return true;
        default: return false;
    }
}

int mem_cartridge_load(char **filename) {
    FILE *rom;
    if ( (rom = fopen(*(filename), "rb")) == NULL) {
        fprintf(stderr, "[Error] mem.c: Cannot read rom <%s>\n", *filename);
        fclose(rom);
        return 1;
    }

    /* load cartridge header */
    fseek(rom, sizeof(uint8_t) * mHeaderStart, SEEK_SET);
    ftell(rom);
    int read_status = 0;
    if ((read_status = fread(&mem.cartridge_header, 1,  sizeof(mem.cartridge_header), rom)) == 0) {
        fprintf(stderr, "[Error] mem.c: Could not read <%s> cartrige header, bytes read: %d\n", *filename, read_status);
        fclose(rom);
        return 1;
    }

    /* load cartrige */

    fseek(rom, 0, SEEK_SET);
    if ((read_status = fread(mem.main, 1, 0X8000, rom)) == 0) {
        fprintf(stderr, "[Error] mem.c: Could not load <%s> memory bank 0, bytes read: %d\n", *filename, read_status);
        fclose(rom);
        return 1;
    }
    // create memory banks
    if (mem.cartridge_header.ROM_size != 0X00) {
        // (Banksize << (value + 1)) - base banks
        if ((mem.ROMbanks = malloc((ROM_BANK_SIZE << (mem.cartridge_header.ROM_size + 1)) - 2)) == NULL) {
            fprintf(stderr, "[Error] mem.c: Could malloc ROMbanks\n");
            fclose(rom);
            return 1;
        }

        if ((read_status = fread(mem.main, 1, sizeof(mem.ROMbanks), rom)) == 0) {
            fprintf(stderr, "[Error] mem.c: Could not load <%s> memory banks, bytes read: %d\n", *filename, read_status);
            fclose(rom);
            return 1;
        }
    }

    fclose(rom);

    // allocating for RAM banks
    if (cart_has_RAM(mem.cartridge_header.cartridge_type)) {
        if (mem.cartridge_header.RAM_size >= 0X02 && mem.cartridge_header.RAM_size <= 0X05) {
            switch(mem.cartridge_header.RAM_size) {
                case(0X02): mem.RAMbanks = malloc(RAM_BANK_SIZE * 1); break;
                case(0X03): mem.RAMbanks = malloc(RAM_BANK_SIZE * 4); break;
                case(0X04): mem.RAMbanks = malloc(RAM_BANK_SIZE * 16); break;
                case(0X05): mem.RAMbanks = malloc(RAM_BANK_SIZE * 8); break;
            }

            if (mem.RAMbanks == NULL) {
                fprintf(stderr, "[Error] mem.c: Could malloc RAMbanks\n");
                return 1;
            }
        }
    }


    return 0;
}

/*
** Gets memory address of location in emulator memory
** - Used in reading and manipulating memory registers and controls
*/
uint8_t *mem_pointer(uint16_t addr) {
    return &mem.main[addr];
}

uint8_t mem_read(uint16_t addr) {
     uint8_t data = 0XFF;

     if (addr >= 0X0000 && addr <= 0X3FFF) {
         // ROM bank 00
         data = mem.main[addr];
     } else if (addr >= 0X4000 && addr <= 0X7FFF) {
         // ROM bank 01-FF
         if (mem.ROM_bank_number == 0X01) {
            // this emulator stores bank 0X01 in main memory
             data = mem.main[addr];
         }
         else {
             // transforming addr to map to ROM banks
             data = mem.ROMbanks[mem.ROM_bank_number * ROM_BANK_SIZE + addr - 0X4000];
         }
     } else if (addr >= 0X8000 && addr <= 0X9FFF) {
         // Video RAM
         data = mem.main[addr];
     } else if (addr >= 0XA000 && addr <= 0XBFFF) {
         // Cartridge RAM
         if (mem.RAM_enabled) {
             if (mem.RAM_bank_number == 0X00) {
                 data = mem.main[addr];
             }
             else {
                 data = mem.RAMbanks[mem.RAM_bank_number * RAM_BANK_SIZE + addr - 0XA000];
             }
         }
     } else if (addr >= 0XC000 && addr <= 0XDFFF) {
         // Work RAM, CGB mode switchable banks
         data = mem.main[addr];
     } else if (addr >= 0XE000 && addr <= 0XFDFF) {
         // Echo RAM, prohibited area by nintendo
         data = mem.main[addr];
     } else if (addr >= 0XFE00 && addr <= 0XFE9F) {
         // Object Attribute Memory
         data = mem.main[addr];
     } else if (addr >= 0XFF00 && addr <= 0XFF7F) {
         // I/O Registers
         data = mem.main[addr];
     } else if (addr >= 0XFF80 && addr <= 0XFFFE) {
         // High RAM
         data = mem.main[addr];
     }


     return data;
 }

void mem_write(uint16_t addr, uint8_t data) {

    if (addr >= 0X0000 && addr <= 0X1FFF) {
        // Enabling RAM bank
        if (!mem.RAM_enabled) {
            // enable RAM when 0X0A has been written to range
            if (data == 0X0A) mem.RAM_enabled = true;

            return;
        }
    } else if (addr >= 0X2000 && addr <= 0X3FFF) {
        // ROM bank select for range 0X4000 - 0X7FFF
        // 5-bit RAM banking number
        data &= 0b00011111;

        // bank number of 0X00 is equal to 0X01
        if (data == 0X00) data = 0X01;

        // &= 0b11100000 to save upper 3 bits for extended cart size
        mem.ROM_bank_number &= 0b11100000;
        mem.ROM_bank_number |= data;
    } else if (addr >= 0X4000 && addr <= 0X5FFF) {
        // RAM bank select or ROM bank select extension
        // 2-bit value

        data &= 0b00000011;
        if (mem.ROM_bank_mode) {
            data <<= 5;
            mem.ROM_bank_number &= 0b00011111;
            mem.ROM_bank_number |= data;
        } else {
            mem.RAM_bank_number = data;
        }
    } else if (addr >= 0X6000 && addr <= 0X7FFF) {
        // ROM/RAM banking mode switch
        data &= 0b00000001;

        mem.ROM_bank_mode = (data == 0);

        if (mem.ROM_bank_mode)
            mem.RAM_bank_number = 0X00;

    } else if (addr >= 0X8000 && addr <= 0X9FFF) {
        // Video RAM
        mem.main[addr] = data;
    } else if (addr >= 0XA000 && addr <= 0XBFFF) {
        // Cartridge RAM
        if (mem.RAM_enabled)
            mem.RAMbanks[mem.RAM_bank_number * RAM_BANK_SIZE + addr - 0XA000] = data;
    } else if (addr >= 0XC000 && addr <= 0XDFFF) {
        // Work RAM, CGB mode switchable banks
        mem.main[addr] = data;
    } else if (addr >= 0XE000 && addr <= 0XFDFF) {
        // Echo RAM, prohibited area by nintendo
    } else if (addr >= 0XFE00 && addr <= 0XFE9F) {
        // Object Attribute Memory
        mem.main[addr] = data;
        mem.main[addr - 0X2000] = data;
    } else if (addr >= 0XFF00 && addr <= 0XFF7F) {
        // I/O Registers

        if (addr == mDIV) data = 0;
        if (addr == mLY) data = 0;


        mem.main[addr] = data;
    } else if (addr >= 0XFF80 && addr <= 0XFFFE) {
        // High RAM
        mem.main[addr] = data;
    }
 }
