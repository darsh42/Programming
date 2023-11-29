#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include "common.h"

#define     HALF_CARRY(a, b, sign) (((a & 0X0f) sign (b & 0X0f)) & 0X010) == 0X010
#define          CARRY(a, b, sign) (((a & 0Xff) sign (b & 0Xff)) & 0X100) == 0X100

typedef union {
    struct {
        uint16_t upper: 8;
        uint16_t lower: 8;
    };
    uint16_t full;

} reg_u;

struct cpu {
    /*
    ** PC - Program counter
    ** SP - Stack Pointer
    */
    uint16_t PC;
    uint16_t SP;

    /*
    ** AF - Accumulator(full) and Flags(lower)
    ** BC - B reg(upper) and C reg(lower)
    ** DE - D reg(upper) and E reg(lower)
    **
    ** FLAGS: ZNHC0000
    **        zero(Z), add/sub(N),
    **        half carry(H), carry(C),
    **        unused(0)
    */
    reg_u AF;
    reg_u BC;
    reg_u DE;
    reg_u HL;

    /*
    ** Interrupt Management Enable
    */
    bool IME;

    /*
    ** Memory: GB work RAM
    */
    uint8_t memory[0X1000];

    /*
    ** Convenience attrs
    */
    bool prefix;
    size_t clock;
    uint8_t CIR;
};


#endif // CPU_H_INCLUDED
