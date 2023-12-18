#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include "common.h"

// Get each flag
#define FZ ((cpu.AF.lower & 0X80) == 0X80)
#define FN ((cpu.AF.lower & 0X40) == 0X40)
#define FH ((cpu.AF.lower & 0X20) == 0X20)
#define FC ((cpu.AF.lower & 0X10) == 0X10)

#define FLAG_SET_Z(Z) cpu.AF.lower |= (Z << 7);
#define FLAG_SET_N(N) cpu.AF.lower |= (N << 6);
#define FLAG_SET_H(H) cpu.AF.lower |= (H << 5);
#define FLAG_SET_C(C) cpu.AF.lower |= (C << 4);

#define PUSH_TO_STACK16BIT(val) mem_write(--cpu.SP, val >> 8); mem_write(--cpu.SP, val)
#define PUSH_TO_STACK(val) mem_write(--cpu.SP, val)

#define POP_FROM_STACK16BIT(val) val = mem_read(cpu.SP++); val |= (mem_read(cpu.SP++) << 8)
#define POP_FROM_STACK(val) val = mem_read(cpu.SP++);

// Calculate Half or Full Carry
#define     HALF_CARRY16BIT(a, b, sign) (((a & 0X0fff) sign (b & 0X0fff)) & 0X1000) == 0X1000
#define          CARRY16BIT(a, b, sign) (((a & 0Xffff) sign (b & 0Xffff)) & 0X10000) == 0X10000
#define     HALF_CARRY(a, b, sign) (((a & 0X0f) sign (b & 0X0f)) & 0X010) == 0X010
#define          CARRY(a, b, sign) (((a & 0Xff) sign (b & 0Xff)) & 0X100) == 0X100
#define     MINHALF_CARRY16BIT(a, b) (((a & 0X0fff) - (b & 0X0fff)) & 0X1000) == ((a & 0X0fff) & 0X1000)
#define          MINCARRY16BIT(a, b) (((a & 0Xffff) - (b & 0Xffff)) & 0X10000) == ((a & 0Xffff) & 0X10000)
#define     MINHALF_CARRY(a, b) (((a & 0X0f) - (b & 0X0f)) & 0X010) != ((a & 0X0f) & 0X010)
#define          MINCARRY(a, b) (((a & 0Xff) - (b & 0Xff)) & 0X100) == ((a & 0Xff) & 0x0100)

#define GET_16_BIT() mem_read(cpu.PC++) | (mem_read(cpu.PC++) << 8)
#define UPPER_LOWER(reg, upper) (upper) ? reg->upper: reg->lower

/* Register definition */
typedef union {
    struct {
        uint16_t lower: 8;
        uint16_t upper: 8;
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
    int clock;
    bool prefix;
    uint8_t CIR;
};

/* Memory functions */
extern uint8_t mem_read(uint16_t addr);
extern void mem_write(uint16_t addr, uint8_t data);

#endif // CPU_H_INCLUDED
