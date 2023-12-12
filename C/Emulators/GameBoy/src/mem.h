#ifndef MEM_H_INCLUDED
#define MEM_H_INCLUDED

#include "common.h"

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
    uint8_t main[0X10000];

    /* Memory Banks */
    uint8_t banks[0X8000];
};


#endif // MEM_H_INCLUDED
