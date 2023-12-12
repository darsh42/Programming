#ifndef CARTRIDGE_H_INCLUDED
#define CARTRIDGE_H_INCLUDED

#include "GBC.h"

struct cartridge_header {
    /*
    ** Specifies if GameBoy Colour:
    ** 0X80 - Backwards compatible
    ** 0XC0 - GBC only function used
    */
    uint8_t GBC_flag;
    uint8_t new_licensee_code[2];
    /*
     ** Specifies if Super GameBoy:
     ** 0X00 - Backwards compatible
     ** 0X03 - GBC only function used
    */
    uint8_t SGB_flag;
    /*
    ** Mapper used in game
    */
    uint8_t cartridge_type;
    /*
    ** ROM size avalible
    ** 32 KiB * (1 << (ROM_size));
    */
    uint8_t ROM_size;
    /*
    ** RAM size on Cartridge
    ** 0X00 - No RAM
    ** 0X01 - Unused
    ** 0X02 - 8KiB, 1 bank(s)
    ** 0X03 - 32 KiB, 4 bank(s)
    ** 0X04 - 128 KiB, 16 bank(s)
    ** 0X05 - 63 KiB, 8 bank(s)
    */
    uint8_t RAM_size;
    uint8_t destination_code;
    uint8_t old_licensee_code;
    uint8_t mask_ROM_version_num;
    uint8_t checksum;
    uint8_t global_checksum[2];
};



#endif // CARTRIDGE_H_INCLUDED
