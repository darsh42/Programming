#include "mem.h"

struct mem mem;

struct mem *get_mem() {return &mem;}

/* Initizializes memory and sets High RAM */
void mem_init() {
    // Set default ROM bank
    mem.ROM_bank_number = 0X01;
    mem.RAM_bank_number = 0x00;

    mem.ROM_bank_mode = false;
    mem.RAM_enabled = false;

    mem.hasROMbanks = false;
    mem.hasRAMbanks = false;

    mem.pixel_transfer = false;

    /* Setting High RAM */
    mem.IO[0X00] = 0XCF; mem.IO[0X01] = 0X00;
    mem.IO[0X02] = 0X7E; mem.IO[0X04] = 0X00;
    mem.IO[0X06] = 0X00; mem.IO[0X07] = 0XF8;
    mem.IO[0X0F] = 0XFF; mem.IO[0X10] = 0X80;
    mem.IO[0X11] = 0XBF; mem.IO[0X12] = 0XF3;
    mem.IO[0X13] = 0XFF; mem.IO[0X14] = 0XBF;
    mem.IO[0X16] = 0X3F; mem.IO[0X17] = 0X00;
    mem.IO[0X18] = 0XFF; mem.IO[0X19] = 0XBF;
    mem.IO[0X1A] = 0X7F; mem.IO[0X1B] = 0XFF;
    mem.IO[0X1C] = 0X9F; mem.IO[0X1D] = 0XFF;
    mem.IO[0X1E] = 0XBF; mem.IO[0X20] = 0XFF;
    mem.IO[0X21] = 0X00; mem.IO[0X22] = 0X00;
    mem.IO[0X23] = 0XBF; mem.IO[0X24] = 0X77;
    mem.IO[0X25] = 0XF3; mem.IO[0X26] = 0XF1;
    mem.IO[0X40] = 0X91; mem.IO[0X41] = 0X85;
    mem.IO[0X42] = 0X00; mem.IO[0X43] = 0X00;
    mem.IO[0X44] = 0X00; mem.IO[0X45] = 0X00;
    mem.IO[0X46] = 0XFF; mem.IO[0X47] = 0XFC;
    mem.IO[0X48] = 0X00; mem.IO[0X49] = 0X00;
    mem.IO[0X4A] = 0X00; mem.IO[0X4B] = 0X00;
    mem.IO[0X4D] = 0XFF; mem.IO[0X4F] = 0XFF;
    mem.IO[0X51] = 0XFF; mem.IO[0X52] = 0XFF;
    mem.IO[0X53] = 0XFF; mem.IO[0X54] = 0XFF;
    mem.IO[0X55] = 0XFF; mem.IO[0X56] = 0XFF;
    mem.IO[0X68] = 0XFF; mem.IO[0X69] = 0XFF;
    mem.IO[0X6A] = 0XFF; mem.IO[0X6B] = 0XFF;
    mem.IO[0X70] = 0XFF; mem.IE = 0X00;
}

/*
** Load GameBoy ROM
** Load correct mapper info
*/
void cart_has_RAM(uint8_t cart_type) {
    switch (cart_type) {
        case(0X02):
        case(0X03):
            mem.hasRAMbanks = true; break;
        default:
            mem.hasRAMbanks =  false; break;
    }
}

int mem_cartridge_load(char *filename) {
    FILE *rom;
    if ( (rom = fopen(filename, "rb")) == NULL) {
        fprintf(stderr, "[Error] mem.c: Cannot read rom <%s>\n", filename);
        return 1;
    }

    /* load cartridge header */
    fseek(rom, sizeof(uint8_t) * mHeaderStart, SEEK_SET);
    ftell(rom);
    int read_status = 0;
    if ((read_status = fread(&mem.cartridge_header, 1,  sizeof(mem.cartridge_header), rom)) == 0) {
        fprintf(stderr, "[Error] mem.c: Could not read <%s> cartrige header, bytes read: %d\n", filename, read_status);
        fclose(rom);
        return 1;
    }

    /* load cartrige */

    fseek(rom, 0, SEEK_SET);
    // (Banksize << (value + 1)) - base banks

    if (mem.cartridge_header.ROM_size != 0X00)
        mem.hasROMbanks = true;

    int banksize = 0X8000 * ((1 << mem.cartridge_header.ROM_size));
    if ((mem.ROMbanks = malloc(banksize * sizeof(uint8_t))) == NULL) {
        fprintf(stderr, "[Error] mem.c: Could malloc ROMbanks\n");
        fclose(rom);
        return 1;
    }

    if ((read_status = fread(mem.ROMbanks, 1, banksize * sizeof(uint8_t), rom)) == 0) {
        fprintf(stderr, "[Error] mem.c: Could not load <%s> memory banks, bytes read: %d\n", filename, read_status);
        fclose(rom);
        return 1;
    }

    fclose(rom);

    // allocating for RAM banks
    cart_has_RAM(mem.cartridge_header.cartridge_type);

    if (mem.hasRAMbanks) {
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


    return 0;
}

/*
** Gets memory address of location in emulator memory
** - Used in reading and manipulating memory registers and controls
*/
uint8_t *mem_pointer(uint16_t addr) {
    if (addr <= 0X3FFF) return &mem.ROMbanks[addr];
    else if (addr >= 0X4000 && addr <= 0X7FFF) {
        return &mem.ROMbanks[mem.ROM_bank_number * ROM_BANK_SIZE + addr - 0X4000];
    }
    else if (addr >= 0X8000 && addr <= 0X9FFF) return &mem.VRAM[addr - 0X8000];
    else if (addr >= 0XA000 && addr <= 0XBFFF) {
        if (!mem.RAM_enabled) return NULL;
        return &mem.RAMbanks[mem.RAM_bank_number * RAM_BANK_SIZE + addr - 0XA000];
    }
    else if (addr >= 0XC000 && addr <= 0XDFFF) return &mem.WRAM[addr - 0XC000];
    else if (addr >= 0XFE00 && addr <= 0XFE9F)  return &mem.OAM[addr - 0XFE00];
    else if (addr >= 0XFF00 && addr <= 0XFF7F)   return &mem.IO[addr - 0XFF00];
    else if (addr >= 0XFF80 && addr <= 0XFFFE) return &mem.HRAM[addr - 0XFF80];
    else if (addr == 0XFFFF) return &mem.IE;

    return NULL;
}

uint8_t mem_read(uint16_t addr) {
     uint8_t data = 0XFF;

     if (addr <= 0X3FFF) data = mem.ROMbanks[addr];
     else if (addr >= 0X4000 && addr <= 0X7FFF) {
         data = mem.ROMbanks[mem.ROM_bank_number * ROM_BANK_SIZE + (addr - 0X4000)];
     }
     else if (addr >= 0X8000 && addr <= 0X9FFF) data = mem.VRAM[addr - 0X8000];
     else if (addr >= 0XA000 && addr <= 0XBFFF) {
         if (!mem.RAM_enabled) return data;
         data = mem.RAMbanks[mem.RAM_bank_number * RAM_BANK_SIZE + addr - 0XA000];
     }
     else if (addr >= 0XC000 && addr <= 0XDFFF) data = mem.WRAM[addr - 0XC000];
     else if (addr >= 0XFE00 && addr <= 0XFE9F)  data = mem.OAM[addr - 0XFE00];
     else if (addr >= 0XFF00 && addr <= 0XFF7F)   data = mem.IO[addr - 0XFF00];
     else if (addr >= 0XFF80 && addr <= 0XFFFE) data = mem.HRAM[addr - 0XFF80];
     else if (addr == 0XFFFF) data = mem.IE;

     return data;
 }

void mem_write(uint16_t addr, uint8_t data) {
    if (addr <= 0X1FFF) {
        // Enabling RAM bank
        // enable RAM when 0X0A has been written to range
        if (!mem.RAM_enabled && mem.hasRAMbanks && data == 0X0A)
            mem.RAM_enabled = true;

    }
    else if (addr >= 0X2000 && addr <= 0X3FFF) {
        if (mem.hasROMbanks) {
            // ROM bank select for range 0X4000 - 0X7FFF
            // 5-bit RAM banking number
            data &= 0b00011111;

            // bank number of 0X00 is equal to 0X01
            if (data == 0X00) data = 0X01;

            // &= 0b11100000 to save upper 3 bits for extended cart size
            mem.ROM_bank_number &= 0b11100000;
            mem.ROM_bank_number |= data;
        }
    }
    else if (addr >= 0X4000 && addr <= 0X5FFF) {
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
    }
    else if (addr >= 0X6000 && addr <= 0X7FFF) {
        // ROM/RAM banking mode switch
        data &= 0b00000001;

        mem.ROM_bank_mode = (data == 0);

        if (mem.ROM_bank_mode)
            mem.RAM_bank_number = 0X00;

    }
    else if (addr >= 0X8000 && addr <= 0X9FFF) mem.VRAM[addr - 0X8000] = data;
    else if (addr >= 0XA000 && addr <= 0XBFFF) {
        // Cartridge RAM
        if (!mem.RAM_enabled) return;
        mem.RAMbanks[mem.RAM_bank_number * RAM_BANK_SIZE + addr - 0XA000] = data;
    }
    else if (addr >= 0XC000 && addr <= 0XDFFF) mem.WRAM[addr - 0XC000] = data;
    else if (addr >= 0XFE00 && addr <= 0XFE9F)  mem.OAM[addr - 0XFE00] = data;
    else if (addr >= 0XFF00 && addr <= 0XFF7F) {
        // I/O Registers
        if (addr == mDMA) {
            // byte specifies start addr of DMA as 0XBB00 where BB is byte
            uint16_t DMA_transfer_addr = data << 8;

            // write from 0XBB00 - 0XBB9F to OAM (0XFE00 - 0XFE9F)
            for (int i = 0; i < 0XA0; i++)
                mem_write(0XFE00 + i, mem_read(DMA_transfer_addr + i));
        }

        if (addr == mIF) return;
        if (addr == mDIV) data = 0;
        if (addr == mLY) data = 0;
        if (mem.pixel_transfer && addr == mPAL) return;


        mem.IO[addr - 0XFF00] = data;
    }
    else if (addr >= 0XFF80 && addr <= 0XFFFE) mem.HRAM[addr - 0XFF80] = data;
    else if (addr == 0XFFFF) mem.IE = data;

    return;
 }
