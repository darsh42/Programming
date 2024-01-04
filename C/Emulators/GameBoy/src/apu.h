#ifndef APU_H_INCLUDED
#define APU_H_INCLUDED

#include "common.h"

struct apu {
    /* APU registers
     * CH1 - Pulse channel
     * CH2 - Pulse channel
     * CH3 -  Wave channel
     * CH4 - Noise channel */
    uint8_t *CH1;
    uint8_t *CH2;
    uint8_t *CH3;
    uint8_t *CH4;

    uint8_t left;
    uint8_t right;
};

extern uint8_t *mem_pointer(uint16_t addr);
extern uint8_t mem_read(uint16_t addr);

#endif // APU_H_INCLUDED
