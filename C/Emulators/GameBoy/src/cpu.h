#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include "common.h"

// Get each flag
#define FZ ((cpu.AF.lower & 0X80) == 0X80)
#define FN ((cpu.AF.lower & 0X40) == 0X40)
#define FH ((cpu.AF.lower & 0X20) == 0X20)
#define FC ((cpu.AF.lower & 0X10) == 0X10)

// Calculate Half or Full Carry
#define     HALF_CARRY(a, b, sign) (((a & 0X0f) sign (b & 0X0f)) & 0X010) == 0X010
#define          CARRY(a, b, sign) (((a & 0Xff) sign (b & 0Xff)) & 0X100) == 0X100

/* Register definition */
typedef union {
    struct {
        uint16_t upper: 8;
        uint16_t lower: 8;
    };
    uint16_t full;

} reg_u;

/* cpu state struct */
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
    ** Interrupt Master Enable
    */
    bool IME;

    /*
    ** Convenience attrs
    */
    bool prefix;
    int clock;
    uint8_t CIR;
};

/* Memory functions */
extern uint16_t mem_read(uint16_t addr, int size, bool *status);
extern void mem_write(uint16_t val, uint16_t addr, int size, bool *status);

#endif // CPU_H_INCLUDED
