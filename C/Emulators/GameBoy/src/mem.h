#ifndef MEM_H_INCLUDED
#define MEM_H_INCLUDED

#include "common.h"
#include "cartridge.h"

#define ROM_BANK_SIZE 0X4000
#define RAM_BANK_SIZE 0X2000

struct mem {

    /*
    ** Main memory
    ** |--------------| <- 0XFFFF Interrupt Enabled Register
    ** |--------------| <- 0XFFFE High RAM Area
    ** |--------------| <- 0XFF7F I/O Registers
    ** |--------------| <- 0XFEFF Unused
    ** |--------------| <- 0XFE9F OAM
    ** |              |
    ** |--------------| <- 0XFDFF Echo RAM
    ** |              |
    ** |              |
    ** |--------------| <- 0XDFFF Working RAM
    ** |              |
    ** |              |
    ** |--------------| <- 0XBFFF Cartiridge RAM
    ** |              |
    ** |              |
    ** |--------------| <- 0X9FFF Background Map
    ** |              |
    ** |--------------| <- 0X97FF Tile RAM
    ** |              |
    ** |              |
    ** |--------------| <- 0X7FFF Game ROM BANK N (switchable)
    ** |              |
    ** |              |
    ** |              |
    ** |--------------| <- 0X3FFF Game ROM BANK 1
    ** |              |
    ** |              |
    ** |              |
    ** |--------------| <- 0X00FF Boot ROM
    **  --------------  <- 0X0000
    */
    struct cartridge_header cartridge_header;

    uint8_t VRAM[0X2000];
    uint8_t WRAM[0X2000];
    uint8_t OAM[0X00A0];
    uint8_t IO[0X80];
    uint8_t HRAM[0X80];
    uint8_t IE;

    /* MBC stuff */
    int MBC;
    bool hasBattery;

    /* MCB3 */
    uint8_t RTC;
    uint8_t RTC_latch;
    bool RTC_enabled;

    /* Memory Banks */
    bool hasROMbanks;
    bool ROM_bank_mode;
    uint16_t ROM_bank_number;
    uint8_t *ROMbanks;

    /* RAM Banks */
    bool hasRAMbanks;
    bool RAM_enabled;
    uint16_t RAM_bank_number;
    uint8_t *RAMbanks;
};


#endif // MEM_H_INCLUDED
