#include "cpu.h"

struct cpu cpu;

////////////////////////////////////////////////////////////////

struct cpu *get_cpu() { return &cpu; }
int cpu_clocks() { return cpu.clock; }
void cpu_clock_reset() { cpu.clock = 0; }
bool cpu_IME() { return cpu.IME; }

////////////////////////////////////////////////////////////////

int GET_16_BIT() {
    uint16_t tmp;
    tmp = mem_read(cpu.PC++);
    tmp |= mem_read(cpu.PC++) << 8;

    return tmp;
}

void cpu_init() {
    cpu.PC = 0X100;
    cpu.SP = 0XFFFE;

    cpu.AF.full = 0X01B0;
    // cpu.AF.lower = (mem_checksum() == 0) ? 0X80: 0XC0;
    cpu.BC.full = 0X0013;
    cpu.DE.full = 0X00D8;
    cpu.HL.full = 0X014D;

    cpu.IME = 0;
    cpu.prefix = 0;
    cpu.clock = 0;
    cpu.CIR = 0;

    // cpu.PC = 0;
    // cpu.SP = 0XFFFE;

    // cpu.AF.upper = 0;
    // cpu.AF.lower = 0;
    // cpu.BC.full = 0;
    // cpu.DE.full = 0;
    // cpu.HL.full = 0;

    // cpu.IME = 0;
    // cpu.prefix = 0;
    // cpu.clock = 0;
    // cpu.CIR = 0;
}

void cpu_ISR(uint8_t service_routine_addr) {
    cpu.IME = false;

    mem_write(--cpu.SP, cpu.PC >> 8);
    mem_write(--cpu.SP, cpu.PC);
    cpu.PC = service_routine_addr;

    cpu.clock += 5;
}

int opcodes();

int prefixed();

int cpu_exec() {
    if (cpu.HALT) {
        if ((mem_read(mIE) & mem_read(mIF)) != 0) {
            cpu.HALT = false;
        }

        return 0;
    }

    int cycles;

    cpu.CIR = mem_read(cpu.PC++);

    if (cpu.EI) {
        cpu.IME = true;
        cpu.EI = false;
    }

    cycles = opcodes();

    cpu.clock += cycles;

    // make sure lower nybble of flags reg is always clear
    cpu.AF.lower &= 0XF0;
    return cycles;
}

int ADC(uint8_t val, bool indirect) {
    uint8_t tmp = cpu.AF.upper + val + FC;

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(((cpu.AF.upper & 0Xf) + (val & 0Xf) + FC) > 0Xf)
    FLAG_SET_C(((cpu.AF.upper & 0Xff) + (val & 0Xff) + FC) > 0Xff)

    cpu.AF.upper = tmp;
    return (indirect) ? 8: 4;
}

int ADD(uint8_t val, bool indirect) {
    uint8_t tmp = cpu.AF.upper +  val;

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(HALF_CARRY(cpu.AF.upper, val, +));
    FLAG_SET_C(CARRY(cpu.AF.upper, val, +));

    cpu.AF.upper = tmp;
    return (indirect) ? 8: 4;
}

int ADDHL(uint16_t val) {
    uint16_t tmp = cpu.HL.full + val;

    FLAG_SET_N(0);
    FLAG_SET_H(HALF_CARRY16BIT(cpu.HL.full, val, +));
    FLAG_SET_C(CARRY16BIT(cpu.HL.full, val, +));

    cpu.HL.full = tmp;
    return 8;
}

int ADDSP(int8_t val) {
    uint16_t tmp = cpu.SP + val;

    FLAG_SET_Z(0);
    FLAG_SET_N(0);
    FLAG_SET_H(HALF_CARRY(cpu.SP, val, +));
    FLAG_SET_C(CARRY(cpu.SP, val, +));

    cpu.SP = tmp;
    return 16;
}

int AND(uint8_t val, bool indirect) {
    uint8_t tmp = cpu.AF.upper & val;

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(1);
    FLAG_SET_C(0);

    cpu.AF.upper = tmp;
    return (indirect) ? 8: 4;
}

int BIT(uint8_t val, uint8_t bit, bool indirect) {
    bool tmp = TEST_BIT(val, bit);

    FLAG_SET_Z(!tmp);
    FLAG_SET_N(0);
    FLAG_SET_H(1);

    return (indirect) ? 8: 4;
}

int CALL() {
    uint16_t addr = GET_16_BIT();

    PUSH_TO_STACK16BIT(cpu.PC);

    cpu.PC = addr;

    return 6;
}

int CALLCC(bool condition) {
    int cycles = 0;
    if (condition) {
        cycles += CALL();
    } else {
        cycles += 3;
        cpu.PC++;
        cpu.PC++;
    }

    return cycles;
}

int CCF() {
    FLAG_SET_N(0);
    FLAG_SET_H(0);
    FLAG_SET_C(!FC);
    return 4;
}

int CP(uint8_t val, bool indirect) {
    uint8_t tmp = cpu.AF.upper - val;

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(1);
    FLAG_SET_H(MINHALF_CARRY(cpu.AF.upper, val));
    FLAG_SET_C(MINCARRY(cpu.AF.upper, val));

    return (indirect) ? 8: 4;
}

int CPL() {
    uint8_t tmp = ~cpu.AF.upper;

    FLAG_SET_N(1);
    FLAG_SET_H(1);

    cpu.AF.upper = tmp;
    return 4;
}

int DAA() {
    if (!FN) {
        if (FC || cpu.AF.upper > 0X99) {
            cpu.AF.upper += 0X60;
            FLAG_SET_C(1);
        }
        if (FH || (cpu.AF.upper & 0X0F) > 0X09) {
            cpu.AF.upper += 0X06;
        }
    } else {
        if (FC) {
            cpu.AF.upper -= 0X60;
        }
        if (FH) {
            cpu.AF.upper -= 0X06;
        }
    }

    FLAG_SET_Z(cpu.AF.upper == 0);
    FLAG_SET_H(0);
    return 4;
}

int DEC(reg_u *reg, bool upper) {
    uint8_t tmp = UPPER_LOWER(reg, upper) - 1;

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(1);
    FLAG_SET_H(MINHALF_CARRY(UPPER_LOWER(reg, upper), 1));

    if (upper) reg->upper = tmp;
    else       reg->lower = tmp;

    return 4;
}

int DECHL() {
    uint8_t tmp = mem_read(cpu.HL.full);

    FLAG_SET_Z(tmp - 1 == 0);
    FLAG_SET_N(1);
    FLAG_SET_H(MINHALF_CARRY(tmp, 1));

    mem_write(cpu.HL.full, tmp - 1);
    return 8;
}

int DEC16(uint16_t *reg) {
    uint16_t tmp = *reg - 1;

    *reg = tmp;
    return 8;
}

int DI() {
    cpu.IME = false;
    return 4;
}

int EI() {
    cpu.EI = true;
    return 4;
}

int HALT() {
    cpu.HALT = true;
    return 4;
}

int INC(reg_u *reg, bool upper) {
    uint8_t tmp =  UPPER_LOWER(reg, upper) + 1;

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(HALF_CARRY(UPPER_LOWER(reg, upper), 1, +));

    if (upper) reg->upper = tmp;
    else       reg->lower = tmp;
    return 4;
}

int INCHL() {
    uint8_t tmp = mem_read(cpu.HL.full);

    FLAG_SET_Z((uint8_t) (tmp + 1) == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(HALF_CARRY(tmp, 1, +));

    mem_write(cpu.HL.full, tmp + 1);
    return 8;
}

int INC16(uint16_t *reg) {
    *reg += 1;
    return 8;
}

int JP() {
    uint16_t tmp;
    tmp = mem_read(cpu.PC++);
    tmp |=  (mem_read(cpu.PC++) << 8);
    cpu.PC = tmp;
    return 16;
}

int JPCC(bool CC) {
    int cycles = 0;
    if (CC) {
        cycles += JP();
    } else {
        cpu.PC++;
        cpu.PC++;
        cycles += 12;
    }

    return cycles;
}

int JPHL() {
    cpu.PC = cpu.HL.full;
    return 4;
}

int JR() {
    int8_t tmp = mem_read(cpu.PC++);
    cpu.PC += tmp;
    return 12;
}

int JRCC(bool condition) {
    int cycles = 0;
    if (condition) {
        cycles += JR();
    } else {
        cpu.PC++;
        cycles += 8;
    }

    return cycles;
}

int LDr8(reg_u *reg, uint8_t val, bool upper) {
    if (upper) reg->upper = val;
    else       reg->lower = val;
    return 4;
}

int LDr8n8(reg_u *reg, bool upper) {
    if (upper) reg->upper = mem_read(cpu.PC++);
    else       reg->lower = mem_read(cpu.PC++);
    return 8;
}

int LDr16n16(uint16_t *dest) {
    *dest = GET_16_BIT();
    return 12;
}

int LDHLr8(uint8_t val) {
    mem_write(cpu.HL.full, val);
    return 8;
}

int LDHLn8() {
    mem_write(cpu.HL.full, mem_read(cpu.PC++));
    return 8;
}

int LDr8HL(reg_u *reg, bool upper) {
    if (upper) reg->upper = mem_read(cpu.HL.full);
    else       reg->lower = mem_read(cpu.HL.full);
    return 8;
}

int LDr16A(uint16_t addr) {
    mem_write(addr, cpu.AF.upper);
    return 8;
}

int LDn16A() {
    mem_write(GET_16_BIT(), cpu.AF.upper);
    return 12;
}

int LDHn16A() {
    mem_write(0XFF00 + mem_read(cpu.PC++), cpu.AF.upper);
    return 12;
}

int LDHCA() {
    mem_write(0XFF00 + cpu.BC.lower, cpu.AF.upper);
    return 12;
}

int LDAr16(uint16_t addr) {
    cpu.AF.upper = mem_read(addr);
    return 8;
}

int LDAn16() {
    cpu.AF.upper = mem_read(GET_16_BIT());
    return 8;
}

int LDHAn16() {
    cpu.AF.upper = mem_read(0XFF00 + mem_read(cpu.PC++));
    return 12;
}

int LDHAC() {
    cpu.AF.upper = mem_read(0XFF00 + cpu.BC.lower);
    return 12;
}

int LDHLIA() {
    mem_write(cpu.HL.full++, cpu.AF.upper);
    return 8;
}

int LDHLDA() {
    mem_write(cpu.HL.full--, cpu.AF.upper);
    return 8;
}

int LDAHLI() {
    cpu.AF.upper = mem_read(cpu.HL.full++);
    return 8;
}

int LDAHLD() {
    cpu.AF.upper = mem_read(cpu.HL.full--);
    return 8;
}

int LDSPn16() {
    cpu.SP = GET_16_BIT();
    return 12;
}

int LDn16SP() {
    uint16_t addr = GET_16_BIT();
    mem_write(addr + 0, cpu.SP & 0XFF);
    mem_write(addr + 1, cpu.SP >> 8);

    return 20;
}

int LDHLSPe8() {
    int8_t tmp = mem_read(cpu.PC++);

    FLAG_SET_Z(0);
    FLAG_SET_N(0);
    FLAG_SET_H(HALF_CARRY(cpu.SP, tmp, +));
    FLAG_SET_C(CARRY(cpu.SP, tmp, +));

    cpu.HL.full = cpu.SP + tmp;
    return 12;
}

int LDSPHL() {
    cpu.SP = cpu.HL.full;
    return 8;
}

int NOP() {
    return 4;
}

int OR(uint8_t val, bool indirect) {
    uint8_t tmp = cpu.AF.upper | val;

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(0);
    FLAG_SET_C(0);

    cpu.AF.upper = tmp;
    return (indirect) ? 8: 1;
}

int POP(uint16_t *reg) {
    uint16_t tmp;
    tmp = mem_read(cpu.SP++);
    tmp |= mem_read(cpu.SP++) << 8;

    *reg = tmp;
    return 12;
}

int PUSH(uint16_t val) {
    PUSH_TO_STACK16BIT(val);
    return 16;
}

int RES(reg_u *reg, bool upper, uint8_t bit) {
    uint8_t tmp = RESET_BIT(UPPER_LOWER(reg, upper), bit);
    if (upper) reg->upper = tmp;
    else       reg->lower = tmp;
    return 8;
}

int RESHL(uint8_t bit) {
    mem_write(cpu.HL.full, RESET_BIT(mem_read(cpu.HL.full), bit));
    return 16;
}

int RET() {
    POP_FROM_STACK16BIT(cpu.PC);
    return 16;
}

int RETCC(bool condition) {
    int cycles;
    if (condition) {
        cycles = RET() + 4;
    } else {
        cycles = 8;
    }

    return cycles;
}

int RETI() {
    cpu.IME = true;
    return RET();
}

int RL(reg_u *reg, bool upper) {
    uint8_t tmp = (UPPER_LOWER(reg, upper) << 1) | FC;

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(0);
    FLAG_SET_C(TEST_BIT(UPPER_LOWER(reg, upper), 7));

    if (upper) reg->upper = tmp;
    else       reg->lower = tmp;
    return 8;
}

int RLHL() {
    uint8_t val = mem_read(cpu.HL.full);
    uint8_t tmp = (val << 1) | FC;

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(0);
    FLAG_SET_C(TEST_BIT(val, 7));

    mem_write(cpu.HL.full, tmp);
    return 16;
}

int RLA() {
    int cycles = RL(&cpu.AF, true) - 4;

    FLAG_SET_Z(0);
    FLAG_SET_Z(0);
    FLAG_SET_Z(0);

    return cycles;
}

int RLC(reg_u *reg, bool upper) {
    uint8_t val = UPPER_LOWER(reg, upper);
    uint8_t tmp = (val << 1) | TEST_BIT(val, 7);

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(0);
    FLAG_SET_C(TEST_BIT(val, 7));

    if (upper) reg->upper = tmp;
    else       reg->lower = tmp;
    return 8;
}

int RLCHL() {
    uint8_t val = mem_read(cpu.HL.full);
    uint8_t tmp = (val << 1) | TEST_BIT(val, 7);

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(0);
    FLAG_SET_C(TEST_BIT(val, 7));

    mem_write(cpu.HL.full, tmp);
    return 16;
}

int RLCA() {
    int cycles = RLC(&cpu.AF, true) - 4;

    FLAG_SET_Z(0);
    FLAG_SET_N(0);
    FLAG_SET_H(0);

    return cycles;
}

int RR(reg_u *reg, bool upper) {
    uint8_t tmp = (UPPER_LOWER(reg, upper) >> 1) | (FC << 7);

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(0);
    FLAG_SET_C(TEST_BIT(UPPER_LOWER(reg, upper), 0));

    if (upper) reg->upper = tmp;
    else       reg->lower = tmp;
    return 8;
}

int RRHL() {
    uint8_t val = mem_read(cpu.HL.full);
    uint8_t tmp = (val >> 1) | (FC << 7);

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(0);
    FLAG_SET_C(TEST_BIT(val, 0));

    mem_write(cpu.HL.full, tmp);
    return 16;
}

int RRA() {
    int cycles = RR(&cpu.AF, true) - 4;
    FLAG_SET_Z(0);

    return cycles;
}

int RRC(reg_u *reg, bool upper) {
    uint8_t val = UPPER_LOWER(reg, upper);
    uint8_t tmp = (val >> 1) | (TEST_BIT(val, 0) << 7);

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(0);
    FLAG_SET_C(TEST_BIT(val, 0));

    if (upper) reg->upper = tmp;
    else       reg->lower = tmp;
    return 8;
}

int RRCHL() {
    uint8_t val = mem_read(cpu.HL.full);
    uint8_t tmp = (val >> 1) | (TEST_BIT(val, 0) << 7);

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(0);
    FLAG_SET_C(TEST_BIT(val, 0));

    mem_write(cpu.HL.full, tmp);
    return 16;
}

int RRCA() {
    int cycles = RRC(&cpu.AF, true) - 4;

    FLAG_SET_Z(0);
    FLAG_SET_Z(0);
    FLAG_SET_Z(0);

    return cycles;
}

int RST(uint8_t addr) {
    PUSH_TO_STACK16BIT(cpu.PC);

    cpu.PC = 0;
    cpu.PC = addr;
    return 16;
}

int SBC(uint8_t val, bool implied) {
    uint8_t tmp = cpu.AF.upper - val - FC;

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(1);
    FLAG_SET_H(((cpu.AF.upper & 0Xf) - (val & 0Xf) - FC) < 0)
    FLAG_SET_C(((cpu.AF.upper & 0Xff) - (val & 0Xff) - FC) < 0)

    cpu.AF.upper = tmp;
    return (implied) ? 8: 4;
}

int SCF() {
    FLAG_SET_N(0);
    FLAG_SET_H(0);
    FLAG_SET_C(1);
    return 4;
}

int SET(reg_u *reg, bool upper, uint8_t bit) {
    if (upper) reg->upper = SET_BIT(reg->upper, bit);
    else       reg->lower = SET_BIT(reg->lower, bit);

    return 8;
}

int SETHL(uint8_t bit) {
    mem_write(cpu.HL.full, SET_BIT(mem_read(cpu.HL.full), bit));
    return 16;
}

int SLA(reg_u *reg, bool upper) {
    uint8_t val = UPPER_LOWER(reg, upper);
    uint8_t tmp = (val << 1);

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(0);
    FLAG_SET_C(TEST_BIT(val, 7));

    if (upper) reg->upper = tmp;
    else       reg->lower = tmp;
    return 8;
}

int SLAHL() {
    uint8_t val = mem_read(cpu.HL.full);
    uint8_t tmp = (val << 1);

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(0);
    FLAG_SET_C(TEST_BIT(val, 7));

    mem_write(cpu.HL.full, tmp);
    return 8;
}

int SRA(reg_u *reg, bool upper) {
    uint8_t val = UPPER_LOWER(reg, upper);
    uint8_t tmp = (val >> 1) | (TEST_BIT(val, 7) << 7);

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(0);
    FLAG_SET_C(TEST_BIT(val, 0));

    if (upper) reg->upper = tmp;
    else       reg->lower = tmp;
    return 8;
}

int SRAHL() {
    uint8_t val = mem_read(cpu.HL.full);
    uint8_t tmp = (val >> 1) | (TEST_BIT(val, 7) << 7);

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(0);
    FLAG_SET_C(TEST_BIT(val, 0));

    mem_write(cpu.HL.full, tmp);
    return 16;
}

int SRL(reg_u *reg, bool upper) {
    uint8_t val = UPPER_LOWER(reg, upper);
    uint8_t tmp = (val >> 1) & 0b01111111;

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(0);
    FLAG_SET_C(TEST_BIT(val, 0));

    if (upper) reg->upper = tmp;
    else       reg->lower = tmp;
    return 8;
}

int SRLHL() {
    uint8_t val = mem_read(cpu.HL.full);
    uint8_t tmp = (val >> 1) & 0b01111111;

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(0);
    FLAG_SET_C(TEST_BIT(val, 0));

    mem_write(cpu.HL.full, tmp);
    return 16;
}

int STOP() {return 0;}

int SUB(uint8_t val, bool indirect) {
    uint8_t tmp = cpu.AF.upper - val;

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(1);
    FLAG_SET_H(MINHALF_CARRY(cpu.AF.upper, val));
    FLAG_SET_C(MINCARRY(cpu.AF.upper, val));

    cpu.AF.upper = tmp;
    return (indirect) ? 8: 4;
}

int SWAP(reg_u *reg, bool upper) {
    uint8_t val = UPPER_LOWER(reg, upper);
    uint8_t tmp = (val << 4) | (val >> 4);

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(0);
    FLAG_SET_C(0);

    if (upper) reg->upper = tmp;
    else       reg->lower = tmp;

    return 8;
}

int SWAPHL() {
    uint8_t val = mem_read(cpu.HL.full);
    uint8_t tmp = (val << 4) | (val >> 4);

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(0);
    FLAG_SET_C(0);

    mem_write(cpu.HL.full, tmp);
    return 16;
}

int XOR(uint8_t val, bool indirect) {
    uint8_t tmp = cpu.AF.upper ^ val;

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(0);
    FLAG_SET_C(0);

    cpu.AF.upper = tmp;
    return (indirect) ? 8: 4;
}

int opcodes() {
    int cycles = 0;

    switch(cpu.CIR) {
        // ADC
        case(0X88): cycles += ADC(cpu.BC.upper, 0); break;
        case(0X89): cycles += ADC(cpu.BC.lower, 0); break;
        case(0X8A): cycles += ADC(cpu.DE.upper, 0); break;
        case(0X8B): cycles += ADC(cpu.DE.lower, 0); break;
        case(0X8C): cycles += ADC(cpu.HL.upper, 0); break;
        case(0X8D): cycles += ADC(cpu.HL.lower, 0); break;
        case(0X8E): cycles += ADC(mem_read(cpu.HL.full), 1); break;
        case(0X8F): cycles += ADC(cpu.AF.upper, 0); break;

        case(0XCE): cycles += ADC(mem_read(cpu.PC++), 1); break;

        // ADD r8
        case(0X80): cycles += ADD(cpu.BC.upper, 0); break;
        case(0X81): cycles += ADD(cpu.BC.lower, 0); break;
        case(0X82): cycles += ADD(cpu.DE.upper, 0); break;
        case(0X83): cycles += ADD(cpu.DE.lower, 0); break;
        case(0X84): cycles += ADD(cpu.HL.upper, 0); break;
        case(0X85): cycles += ADD(cpu.HL.lower, 0); break;
        case(0X86): cycles += ADD(mem_read(cpu.HL.full), 1); break;
        case(0X87): cycles += ADD(cpu.AF.upper, 0); break;

        case(0XC6): cycles += ADD(mem_read(cpu.PC++), 1); break;

        // ADD HL, r16
        case(0X09): cycles += ADDHL(cpu.BC.full); break;
        case(0X19): cycles += ADDHL(cpu.DE.full); break;
        case(0X29): cycles += ADDHL(cpu.HL.full); break;
        case(0X39): cycles += ADDHL(cpu.SP); break;

        // ADD SP, e8
        case(0XE8): cycles += ADDSP(mem_read(cpu.PC++)); break;

        // AND
        case(0XA0): cycles += AND(cpu.BC.upper, 0); break;
        case(0XA1): cycles += AND(cpu.BC.lower, 0); break;
        case(0XA2): cycles += AND(cpu.DE.upper, 0); break;
        case(0XA3): cycles += AND(cpu.DE.lower, 0); break;
        case(0XA4): cycles += AND(cpu.HL.upper, 0); break;
        case(0XA5): cycles += AND(cpu.HL.lower, 0); break;
        case(0XA6): cycles += AND(mem_read(cpu.HL.full), 1); break;
        case(0XA7): cycles += AND(cpu.AF.upper, 0); break;

        case(0XE6): cycles += AND(mem_read(cpu.PC++), 1); break;

        // CALL
        case(0XCD): cycles += CALL(); break;
        case(0XC4): cycles += CALLCC(!FZ); break;
        case(0XD4): cycles += CALLCC(!FC); break;
        case(0XCC): cycles += CALLCC(FZ); break;
        case(0XDC): cycles += CALLCC(FC); break;

        // CCF
        case(0X3F): cycles += CCF(); break;

        // CP
        case(0XB8): cycles += CP(cpu.BC.upper, 0); break;
        case(0XB9): cycles += CP(cpu.BC.lower, 0); break;
        case(0XBA): cycles += CP(cpu.DE.upper, 0); break;
        case(0XBB): cycles += CP(cpu.DE.lower, 0); break;
        case(0XBC): cycles += CP(cpu.HL.upper, 0); break;
        case(0XBD): cycles += CP(cpu.HL.lower, 0); break;
        case(0XBE): cycles += CP(mem_read(cpu.HL.full), 1); break;
        case(0XBF): cycles += CP(cpu.AF.upper, 0); break;

        case(0XFE): cycles += CP(mem_read(cpu.PC++), 1); break;

        // CPL
        case(0X2F): cycles += CPL(); break;

        // DAA
        case(0X27): cycles += DAA(); break;

        // DEC r8
        case(0X05): cycles += DEC(&cpu.BC, 1); break;
        case(0X0D): cycles += DEC(&cpu.BC, 0); break;
        case(0X15): cycles += DEC(&cpu.DE, 1); break;
        case(0X1D): cycles += DEC(&cpu.DE, 0); break;
        case(0X25): cycles += DEC(&cpu.HL, 1); break;
        case(0X2D): cycles += DEC(&cpu.HL, 0); break;
        case(0X35): cycles += DECHL(); break;
        case(0X3D): cycles += DEC(&cpu.AF, 1); break;

        // DEC r16
        case(0X0B): cycles += DEC16(&cpu.BC.full); break;
        case(0X1B): cycles += DEC16(&cpu.DE.full); break;
        case(0X2B): cycles += DEC16(&cpu.HL.full); break;
        case(0X3B): cycles += DEC16(&cpu.SP); break;

        // DI
        case(0XF3): cycles += DI(); break;

        // EI
        case(0XFB): cycles += EI(); break;

        // HALT
        case(0X76): cycles += HALT(); break;

        // INC
        case(0X04): cycles += INC(&cpu.BC, 1); break;
        case(0X0C): cycles += INC(&cpu.BC, 0); break;
        case(0X14): cycles += INC(&cpu.DE, 1); break;
        case(0X1C): cycles += INC(&cpu.DE, 0); break;
        case(0X24): cycles += INC(&cpu.HL, 1); break;
        case(0X2C): cycles += INC(&cpu.HL, 0); break;
        case(0X34): cycles += INCHL(); break;
        case(0X3C): cycles += INC(&cpu.AF, 1); break;

        // INC r16
        case(0X03): cycles += INC16(&cpu.BC.full); break;
        case(0X13): cycles += INC16(&cpu.DE.full); break;
        case(0X23): cycles += INC16(&cpu.HL.full); break;
        case(0X33): cycles += INC16(&cpu.SP); break;

        // JP
        case(0XC3): cycles += JP(); break;
        case(0XC2): cycles += JPCC(!FZ); break;
        case(0XD2): cycles += JPCC(!FC); break;
        case(0XCA): cycles += JPCC(FZ); break;
        case(0XDA): cycles += JPCC(FC); break;

        case(0XE9): cycles += JPHL(); break;

        // JR
        case(0X18): cycles += JR(); break;
        case(0X20): cycles += JRCC(!FZ); break;
        case(0X30): cycles += JRCC(!FC); break;
        case(0X28): cycles += JRCC(FZ); break;
        case(0X38): cycles += JRCC(FC); break;

        // LD
        case(0X40): cycles += LDr8(&cpu.BC, cpu.BC.upper, 1); break;
        case(0X41): cycles += LDr8(&cpu.BC, cpu.BC.lower, 1); break;
        case(0X42): cycles += LDr8(&cpu.BC, cpu.DE.upper, 1); break;
        case(0X43): cycles += LDr8(&cpu.BC, cpu.DE.lower, 1); break;
        case(0X44): cycles += LDr8(&cpu.BC, cpu.HL.upper, 1); break;
        case(0X45): cycles += LDr8(&cpu.BC, cpu.HL.lower, 1); break;
        case(0X46): cycles += LDr8(&cpu.BC, mem_read(cpu.HL.full), 1); break;
        case(0X47): cycles += LDr8(&cpu.BC, cpu.AF.upper, 1); break;

        case(0X48): cycles += LDr8(&cpu.BC, cpu.BC.upper, 0); break;
        case(0X49): cycles += LDr8(&cpu.BC, cpu.BC.lower, 0); break;
        case(0X4A): cycles += LDr8(&cpu.BC, cpu.DE.upper, 0); break;
        case(0X4B): cycles += LDr8(&cpu.BC, cpu.DE.lower, 0); break;
        case(0X4C): cycles += LDr8(&cpu.BC, cpu.HL.upper, 0); break;
        case(0X4D): cycles += LDr8(&cpu.BC, cpu.HL.lower, 0); break;
        case(0X4E): cycles += LDr8(&cpu.BC, mem_read(cpu.HL.full), 0); break;
        case(0X4F): cycles += LDr8(&cpu.BC, cpu.AF.upper, 0); break;

        case(0X50): cycles += LDr8(&cpu.DE, cpu.BC.upper, 1); break;
        case(0X51): cycles += LDr8(&cpu.DE, cpu.BC.lower, 1); break;
        case(0X52): cycles += LDr8(&cpu.DE, cpu.DE.upper, 1); break;
        case(0X53): cycles += LDr8(&cpu.DE, cpu.DE.lower, 1); break;
        case(0X54): cycles += LDr8(&cpu.DE, cpu.HL.upper, 1); break;
        case(0X55): cycles += LDr8(&cpu.DE, cpu.HL.lower, 1); break;
        case(0X56): cycles += LDr8(&cpu.DE, mem_read(cpu.HL.full), 1); break;
        case(0X57): cycles += LDr8(&cpu.DE, cpu.AF.upper, 1); break;

        case(0X58): cycles += LDr8(&cpu.DE, cpu.BC.upper, 0); break;
        case(0X59): cycles += LDr8(&cpu.DE, cpu.BC.lower, 0); break;
        case(0X5A): cycles += LDr8(&cpu.DE, cpu.DE.upper, 0); break;
        case(0X5B): cycles += LDr8(&cpu.DE, cpu.DE.lower, 0); break;
        case(0X5C): cycles += LDr8(&cpu.DE, cpu.HL.upper, 0); break;
        case(0X5D): cycles += LDr8(&cpu.DE, cpu.HL.lower, 0); break;
        case(0X5E): cycles += LDr8(&cpu.DE, mem_read(cpu.HL.full), 0); break;
        case(0X5F): cycles += LDr8(&cpu.DE, cpu.AF.upper, 0); break;

        case(0X60): cycles += LDr8(&cpu.HL, cpu.BC.upper, 1); break;
        case(0X61): cycles += LDr8(&cpu.HL, cpu.BC.lower, 1); break;
        case(0X62): cycles += LDr8(&cpu.HL, cpu.DE.upper, 1); break;
        case(0X63): cycles += LDr8(&cpu.HL, cpu.DE.lower, 1); break;
        case(0X64): cycles += LDr8(&cpu.HL, cpu.HL.upper, 1); break;
        case(0X65): cycles += LDr8(&cpu.HL, cpu.HL.lower, 1); break;
        case(0X66): cycles += LDr8(&cpu.HL, mem_read(cpu.HL.full), 1); break;
        case(0X67): cycles += LDr8(&cpu.HL, cpu.AF.upper, 1); break;

        case(0X68): cycles += LDr8(&cpu.HL, cpu.BC.upper, 0); break;
        case(0X69): cycles += LDr8(&cpu.HL, cpu.BC.lower, 0); break;
        case(0X6A): cycles += LDr8(&cpu.HL, cpu.DE.upper, 0); break;
        case(0X6B): cycles += LDr8(&cpu.HL, cpu.DE.lower, 0); break;
        case(0X6C): cycles += LDr8(&cpu.HL, cpu.HL.upper, 0); break;
        case(0X6D): cycles += LDr8(&cpu.HL, cpu.HL.lower, 0); break;
        case(0X6E): cycles += LDr8(&cpu.HL, mem_read(cpu.HL.full), 0); break;
        case(0X6F): cycles += LDr8(&cpu.HL, cpu.AF.upper, 0); break;

        case(0X70): cycles += LDHLr8(cpu.BC.upper); break;
        case(0X71): cycles += LDHLr8(cpu.BC.lower); break;
        case(0X72): cycles += LDHLr8(cpu.DE.upper); break;
        case(0X73): cycles += LDHLr8(cpu.DE.lower); break;
        case(0X74): cycles += LDHLr8(cpu.HL.upper); break;
        case(0X75): cycles += LDHLr8(cpu.HL.lower); break;
        case(0X77): cycles += LDHLr8(cpu.AF.upper); break;

        case(0X78): cycles += LDr8(&cpu.AF, cpu.BC.upper, 1); break;
        case(0X79): cycles += LDr8(&cpu.AF, cpu.BC.lower, 1); break;
        case(0X7A): cycles += LDr8(&cpu.AF, cpu.DE.upper, 1); break;
        case(0X7B): cycles += LDr8(&cpu.AF, cpu.DE.lower, 1); break;
        case(0X7C): cycles += LDr8(&cpu.AF, cpu.HL.upper, 1); break;
        case(0X7D): cycles += LDr8(&cpu.AF, cpu.HL.lower, 1); break;
        case(0X7E): cycles += LDr8(&cpu.AF, mem_read(cpu.HL.full), 1); break;
        case(0X7F): cycles += LDr8(&cpu.AF, cpu.AF.upper, 1); break;

        case(0X06): cycles += LDr8n8(&cpu.BC, 1); break;
        case(0X16): cycles += LDr8n8(&cpu.DE, 1); break;
        case(0X26): cycles += LDr8n8(&cpu.HL, 1); break;
        case(0X36): cycles += LDHLn8(); break;

        case(0X0E): cycles += LDr8n8(&cpu.BC, 0); break;
        case(0X1E): cycles += LDr8n8(&cpu.DE, 0); break;
        case(0X2E): cycles += LDr8n8(&cpu.HL, 0); break;
        case(0X3E): cycles += LDr8n8(&cpu.AF, 1); break;

        case(0X02): cycles += LDr16A(cpu.BC.full); break;
        case(0X12): cycles += LDr16A(cpu.DE.full); break;
        case(0X22): cycles += LDHLIA(); break;
        case(0X32): cycles += LDHLDA(); break;

        case(0X0A): cycles += LDAr16(cpu.BC.full); break;
        case(0X1A): cycles += LDAr16(cpu.DE.full); break;
        case(0X2A): cycles += LDAHLI(); break;
        case(0X3A): cycles += LDAHLD(); break;

        case(0X01): cycles += LDr16n16(&cpu.BC.full); break;
        case(0X11): cycles += LDr16n16(&cpu.DE.full); break;
        case(0X21): cycles += LDr16n16(&cpu.HL.full); break;
        case(0X31): cycles += LDSPn16(); break;

        case(0X08): cycles += LDn16SP(); break;

        case(0XE0): cycles += LDHn16A(); break;
        case(0XF0): cycles += LDHAn16(); break;

        case(0XE2): cycles += LDHCA(); break;
        case(0XF2): cycles += LDHAC(); break;

        case(0XF8): cycles += LDHLSPe8(); break;
        case(0XF9): cycles += LDSPHL(); break;

        case(0XEA): cycles += LDn16A(); break;
        case(0XFA): cycles += LDAn16(); break;

        // NOP
        case(0X00): cycles += NOP(); break;

        // OR
        case(0XB0): cycles += OR(cpu.BC.upper, 0); break;
        case(0XB1): cycles += OR(cpu.BC.lower, 0); break;
        case(0XB2): cycles += OR(cpu.DE.upper, 0); break;
        case(0XB3): cycles += OR(cpu.DE.lower, 0); break;
        case(0XB4): cycles += OR(cpu.HL.upper, 0); break;
        case(0XB5): cycles += OR(cpu.HL.lower, 0); break;
        case(0XB6): cycles += OR(mem_read(cpu.HL.full), 1); break;
        case(0XB7): cycles += OR(cpu.AF.upper, 0); break;

        case(0XF6): cycles += OR(mem_read(cpu.PC++), 1); break;

        // POP
        case(0XC1): cycles += POP(&cpu.BC.full); break;
        case(0XD1): cycles += POP(&cpu.DE.full); break;
        case(0XE1): cycles += POP(&cpu.HL.full); break;
        case(0XF1): cycles += POP(&cpu.AF.full); break;

        // PUSH
        case(0XC5): cycles += PUSH(cpu.BC.full); break;
        case(0XD5): cycles += PUSH(cpu.DE.full); break;
        case(0XE5): cycles += PUSH(cpu.HL.full); break;
        case(0XF5): cycles += PUSH(cpu.AF.full); break;

        // RET
        case(0XC9): cycles += RET(); break;
        case(0XC0): cycles += RETCC(!FZ); break;
        case(0XD0): cycles += RETCC(!FC); break;
        case(0XC8): cycles += RETCC(FZ); break;
        case(0XD8): cycles += RETCC(FC); break;

        case(0XD9): cycles += RETI(); break;

        // RRA
        case(0X1F): cycles += RRA(); break;
        case(0X0F): cycles += RRCA(); break;

        // RLA
        case(0X17): cycles += RLA(); break;
        case(0X07): cycles += RLCA(); break;

        // RST
        case(0XC7): cycles += RST(0X00); break;
        case(0XD7): cycles += RST(0X10); break;
        case(0XE7): cycles += RST(0X20); break;
        case(0XF7): cycles += RST(0X30); break;
        case(0XCF): cycles += RST(0X08); break;
        case(0XDF): cycles += RST(0X18); break;
        case(0XEF): cycles += RST(0X28); break;
        case(0XFF): cycles += RST(0X38); break;

        // SBC
        case(0X98): cycles += SBC(cpu.BC.upper, 0); break;
        case(0X99): cycles += SBC(cpu.BC.lower, 0); break;
        case(0X9A): cycles += SBC(cpu.DE.upper, 0); break;
        case(0X9B): cycles += SBC(cpu.DE.lower, 0); break;
        case(0X9C): cycles += SBC(cpu.HL.upper, 0); break;
        case(0X9D): cycles += SBC(cpu.HL.lower, 0); break;
        case(0X9E): cycles += SBC(mem_read(cpu.HL.full), 1); break;
        case(0X9F): cycles += SBC(cpu.AF.upper, 0); break;

        case(0XDE): cycles += SBC(mem_read(cpu.PC++), 1); break;

        // SCF
        case(0X37): cycles += SCF(); break;

        // STOP
        case(0X10): cycles += STOP(); break;

        // SUC
        case(0X90): cycles += SUB(cpu.BC.upper, 0); break;
        case(0X91): cycles += SUB(cpu.BC.lower, 0); break;
        case(0X92): cycles += SUB(cpu.DE.upper, 0); break;
        case(0X93): cycles += SUB(cpu.DE.lower, 0); break;
        case(0X94): cycles += SUB(cpu.HL.upper, 0); break;
        case(0X95): cycles += SUB(cpu.HL.lower, 0); break;
        case(0X96): cycles += SUB(mem_read(cpu.HL.full), 1); break;
        case(0X97): cycles += SUB(cpu.AF.upper, 0); break;

        case(0XD6): cycles += SUB(mem_read(cpu.PC++), 1); break;

        // XOR
        case(0XA8): cycles += XOR(cpu.BC.upper, 0); break;
        case(0XA9): cycles += XOR(cpu.BC.lower, 0); break;
        case(0XAA): cycles += XOR(cpu.DE.upper, 0); break;
        case(0XAB): cycles += XOR(cpu.DE.lower, 0); break;
        case(0XAC): cycles += XOR(cpu.HL.upper, 0); break;
        case(0XAD): cycles += XOR(cpu.HL.lower, 0); break;
        case(0XAE): cycles += XOR(mem_read(cpu.HL.full), 1); break;
        case(0XAF): cycles += XOR(cpu.AF.upper, 0); break;

        case(0XEE): cycles += XOR(mem_read(cpu.PC++), 1); break;

        // prefix on
        case(0XCB): cpu.CIR = mem_read(cpu.PC++); cycles += prefixed(); break;
        default: break;
    }

    return cycles;
}

int prefixed() {
    reg_u *reg;
    bool upper;
    int cycles = 0;

    cpu.prefix = false;

    // determine which register used
    switch(cpu.CIR & 0X0F) {
        case(0X00): case(0X08): reg = &cpu.BC; upper = 1; break; // REG B
        case(0X01): case(0X09): reg = &cpu.BC; upper = 0; break; // REG C
        case(0X02): case(0X0A): reg = &cpu.DE; upper = 1; break; // REG D
        case(0X03): case(0X0B): reg = &cpu.DE; upper = 0; break; // REG E
        case(0X04): case(0X0C): reg = &cpu.HL; upper = 1; break; // REG H
        case(0X05): case(0X0D): reg = &cpu.HL; upper = 0; break; // REG L
        case(0X07): case(0X0F): reg = &cpu.AF; upper = 1; break; // REG A
    }

    switch(cpu.CIR) {
        // RLC
        case(0X00):
        case(0X01):
        case(0X02):
        case(0X03):
        case(0X04):
        case(0X05):
        case(0X07): cycles += RLC(reg, upper); break;
        case(0X06): cycles += RLCHL(); break;

        // RRC
        case(0X08):
        case(0X09):
        case(0X0A):
        case(0X0B):
        case(0X0C):
        case(0X0D):
        case(0X0F): cycles += RRC(reg, upper); break;
        case(0X0E): cycles += RRCHL(); break;

        // RL
        case(0X10):
        case(0X11):
        case(0X12):
        case(0X13):
        case(0X14):
        case(0X15):
        case(0X17): cycles += RL(reg, upper); break;
        case(0X16): cycles += RLHL(); break;

        // RR
        case(0X18):
        case(0X19):
        case(0X1A):
        case(0X1B):
        case(0X1C):
        case(0X1D):
        case(0X1F): cycles += RR(reg, upper); break;
        case(0X1E): cycles += RRHL(); break;


        // SLA
        case(0X20):
        case(0X21):
        case(0X22):
        case(0X23):
        case(0X24):
        case(0X25):
        case(0X27): cycles += SLA(reg, upper); break;
        case(0X26): cycles += SLAHL(); break;

        // SRA
        case(0X28):
        case(0X29):
        case(0X2A):
        case(0X2B):
        case(0X2C):
        case(0X2D):
        case(0X2F): cycles += SRA(reg, upper); break;
        case(0X2E): cycles += SRAHL(); break;

        // SWAP
        case(0X30):
        case(0X31):
        case(0X32):
        case(0X33):
        case(0X34):
        case(0X35):
        case(0X37): cycles += SWAP(reg, upper); break;
        case(0X36): cycles += SWAPHL(); break;

        // SRL
        case(0X38):
        case(0X39):
        case(0X3A):
        case(0X3B):
        case(0X3C):
        case(0X3D):
        case(0X3F): cycles += SRL(reg, upper); break;;
        case(0X3E): cycles += SRLHL(); break;

        // BIT 0
        case(0X40):
        case(0X41):
        case(0X42):
        case(0X43):
        case(0X44):
        case(0X45):
        case(0X47): cycles += BIT(UPPER_LOWER(reg, upper), 0, 0); break;
        case(0X46): cycles += BIT(mem_read(cpu.HL.full), 0, 1); break;

        // BIT 1
        case(0X48):
        case(0X49):
        case(0X4A):
        case(0X4B):
        case(0X4C):
        case(0X4D):
        case(0X4F): cycles += BIT(UPPER_LOWER(reg, upper), 1, 0); break;
        case(0X4E): cycles += BIT(mem_read(cpu.HL.full), 1, 1); break;

        // BIT 2
        case(0X50):
        case(0X51):
        case(0X52):
        case(0X53):
        case(0X54):
        case(0X55):
        case(0X57): cycles += BIT(UPPER_LOWER(reg, upper), 2, 0); break;
        case(0X56): cycles += BIT(mem_read(cpu.HL.full), 2, 1); break;

        // BIT 3
        case(0X58):
        case(0X59):
        case(0X5A):
        case(0X5B):
        case(0X5C):
        case(0X5D):
        case(0X5F): cycles += BIT(UPPER_LOWER(reg, upper), 3, 0); break;
        case(0X5E): cycles += BIT(mem_read(cpu.HL.full), 3, 1); break;

        // BIT 4
        case(0X60):
        case(0X61):
        case(0X62):
        case(0X63):
        case(0X64):
        case(0X65):
        case(0X67): cycles += BIT(UPPER_LOWER(reg, upper), 4, 0); break;
        case(0X66): cycles += BIT(mem_read(cpu.HL.full), 4, 1); break;

        // BIT 6
        case(0X68):
        case(0X69):
        case(0X6A):
        case(0X6B):
        case(0X6C):
        case(0X6D):
        case(0X6F): cycles += BIT(UPPER_LOWER(reg, upper), 5, 0); break;
        case(0X6E): cycles += BIT(mem_read(cpu.HL.full), 5, 1); break;

        // BIT 6
        case(0X70):
        case(0X71):
        case(0X72):
        case(0X73):
        case(0X74):
        case(0X75):
        case(0X77): cycles += BIT(UPPER_LOWER(reg, upper), 6, 0); break;
        case(0X76): cycles += BIT(mem_read(cpu.HL.full), 6, 1); break;

        // BIT 7
        case(0X78):
        case(0X79):
        case(0X7A):
        case(0X7B):
        case(0X7C):
        case(0X7D):
        case(0X7F): cycles += BIT(UPPER_LOWER(reg, upper), 7, 0); break;
        case(0X7E): cycles += BIT(mem_read(cpu.HL.full), 7, 1); break;

        // RES 0
        case(0X80):
        case(0X81):
        case(0X82):
        case(0X83):
        case(0X84):
        case(0X85):
        case(0X87): cycles += RES(reg, upper, 0); break;
        case(0X86): cycles += RESHL(0); break;

        // RES 1
        case(0X88):
        case(0X89):
        case(0X8A):
        case(0X8B):
        case(0X8C):
        case(0X8D):
        case(0X8F): cycles += RES(reg, upper, 1); break;
        case(0X8E): cycles += RESHL(1); break;

        // RES 2
        case(0X90):
        case(0X91):
        case(0X92):
        case(0X93):
        case(0X94):
        case(0X95):
        case(0X97): cycles += RES(reg, upper, 2); break;
        case(0X96): cycles += RESHL(2); break;

        // RES 9
        case(0X98):
        case(0X99):
        case(0X9A):
        case(0X9B):
        case(0X9C):
        case(0X9D):
        case(0X9F): cycles += RES(reg, upper, 3); break;
        case(0X9E): cycles += RESHL(3); break;

        // RES 4
        case(0XA0):
        case(0XA1):
        case(0XA2):
        case(0XA3):
        case(0XA4):
        case(0XA5):
        case(0XA7): cycles += RES(reg, upper, 4); break;
        case(0XA6): cycles += RESHL(4); break;

        // RES A
        case(0XA8):
        case(0XA9):
        case(0XAA):
        case(0XAB):
        case(0XAC):
        case(0XAD):
        case(0XAF): cycles += RES(reg, upper, 5); break;
        case(0XAE): cycles += RESHL(5); break;

        // RES 6
        case(0XB0):
        case(0XB1):
        case(0XB2):
        case(0XB3):
        case(0XB4):
        case(0XB5):
        case(0XB7): cycles += RES(reg, upper, 6); break;
        case(0XB6): cycles += RESHL(6); break;

        // RES B
        case(0XB8):
        case(0XB9):
        case(0XBA):
        case(0XBB):
        case(0XBC):
        case(0XBD):
        case(0XBF): cycles += RES(reg, upper, 7); break;
        case(0XBE): cycles += RESHL(7); break;


        // SET 0
        case(0XC0):
        case(0XC1):
        case(0XC2):
        case(0XC3):
        case(0XC4):
        case(0XC5):
        case(0XC7): cycles += SET(reg, upper, 0); break;
        case(0XC6): cycles += SETHL(0); break;

        // SET 1
        case(0XC8):
        case(0XC9):
        case(0XCA):
        case(0XCB):
        case(0XCC):
        case(0XCD):
        case(0XCF): cycles += SET(reg, upper, 1); break;
        case(0XCE): cycles += SETHL(1); break;

        // SET 2
        case(0XD0):
        case(0XD1):
        case(0XD2):
        case(0XD3):
        case(0XD4):
        case(0XD5):
        case(0XD7): cycles += SET(reg, upper, 2); break;
        case(0XD6): cycles += SETHL(2); break;

        // SET 3
        case(0XD8):
        case(0XD9):
        case(0XDA):
        case(0XDB):
        case(0XDC):
        case(0XDD):
        case(0XDF): cycles += SET(reg, upper, 3); break;
        case(0XDE): cycles += SETHL(3); break;

        // SET 4
        case(0XE0):
        case(0XE1):
        case(0XE2):
        case(0XE3):
        case(0XE4):
        case(0XE5):
        case(0XE7): cycles += SET(reg, upper, 4); break;
        case(0XE6): cycles += SETHL(4); break;

        // SET 5
        case(0XE8):
        case(0XE9):
        case(0XEA):
        case(0XEB):
        case(0XEC):
        case(0XED):
        case(0XEF): cycles += SET(reg, upper, 5); break;
        case(0XEE): cycles += SETHL(5); break;

        // SET 6
        case(0XF0):
        case(0XF1):
        case(0XF2):
        case(0XF3):
        case(0XF4):
        case(0XF5):
        case(0XF7): cycles += SET(reg, upper, 6); break;
        case(0XF6): cycles += SETHL(6); break;

        // SET 7
        case(0XF8):
        case(0XF9):
        case(0XFA):
        case(0XFB):
        case(0XFC):
        case(0XFD):
        case(0XFF): cycles += SET(reg, upper, 7); break;
        case(0XFE): cycles += SETHL(7); break;
    }

    return cycles;
}
