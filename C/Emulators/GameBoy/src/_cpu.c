#include "cpu.h"

struct cpu cpu;

int cpu_clocks() {
    return cpu.clock;
}

void cpu_clock_reset() {
    cpu.clock = 0;
}

bool cpu_IME() {
    return cpu.IME;
}

struct cpu *get_cpu() {
    return &cpu;
}

void cpu_ISR_start(uint8_t service_routine_addr) {
    cpu.IME = 0;

    mem_write(--cpu.SP, cpu.PC >> 8);
    mem_write(--cpu.SP, cpu.PC);
    cpu.PC = service_routine_addr;

    cpu.clock += 5;
}

void cpu_ISR_return() {
    cpu.PC = mem_read(cpu.SP++) << 8;
    cpu.PC |= mem_read(cpu.SP++);
}

void cpu_init(uint8_t checksum) {
    cpu.PC = 0X100;
    cpu.SP = 0XFFFE;

    cpu.AF.upper = 0X1;
    cpu.AF.lower = checksum;
    cpu.BC.full = 0X0013;
    cpu.DE.full = 0X00D8;
    cpu.HL.full = 0X014D;

    cpu.IME = 0;
    cpu.prefix = 0;
    cpu.clock = 0;
    cpu.CIR = 0;
}

/* writes to flag reg, mask specifies unchanged flags */
void flag_write(bool Z, bool N, bool H, bool C, uint8_t mask) {
    uint8_t flag = (cpu.AF.lower & (mask << 3)) | ((Z << 7) | (N << 6) | (H << 5) | (C << 4));
    cpu.AF.lower = flag;
}
void opcodes(uint8_t opcode);
void prefixed(uint8_t opcode);

void cpu_exec() {
    cpu.CIR = mem_read(cpu.PC++);
    if (cpu.prefix) {
        prefixed(cpu.CIR);
    } else {
        opcodes(cpu.CIR);
    }
}

void ADC(uint8_t val, bool indirect) {
    uint8_t tmp = cpu.AF.upper +  val + FC;

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(HALF_CARRY(cpu.AF.upper, val + FC, +));
    FLAG_SET_C(CARRY(cpu.AF.upper, val + FC, +));

    cpu.AF.upper = tmp;
    cpu.clock += (indirect) ? 8: 4;
}

void ADD(uint8_t val, bool indirect) {
    uint8_t tmp = cpu.AF.upper +  val;

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(HALF_CARRY(cpu.AF.upper, val, +));
    FLAG_SET_C(CARRY(cpu.AF.upper, val, +));

    cpu.AF.upper = tmp;
    cpu.clock += (indirect) ? 8: 4;
}

void ADDHL(uint16_t val) {
    uint16_t tmp = cpu.HL.full + val;

    FLAG_SET_N(0);
    FLAG_SET_H(HALF_CARRY16BIT(cpu.HL.full, val, +));
    FLAG_SET_C(CARRY16BIT(cpu.HL.full, val, +));

    cpu.HL.full = tmp;
    cpu.clock += 8;
}

void ADDSP(int8_t val) {
    uint8_t tmp = cpu.SP +  val;

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(HALF_CARRY(cpu.SP, val, +));
    FLAG_SET_C(CARRY(cpu.SP, val, +));

    cpu.SP = tmp;
    cpu.clock += 16;
}

void AND(uint8_t val, bool indirect) {
    uint8_t tmp = cpu.AF.upper & val;

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(HALF_CARRY(cpu.AF.upper, val, &));
    FLAG_SET_C(CARRY(cpu.AF.upper, val, &));

    cpu.AF.upper = tmp;
    cpu.clock += (indirect) ? 8: 4;
}

void BIT(uint8_t val, uint8_t bit, bool indirect) {
    bool tmp = TEST_BIT(val, bit);

    if (!FZ) FLAG_SET_Z(tmp);

    cpu.clock += (indirect) ? 8: 4;
}

void CALL(uint16_t addr) {
    PUSH_TO_STACK16BIT(cpu.PC++);

    cpu.PC = addr;

    cpu.clock += 6;
}

void CALLCC(uint16_t addr, bool condition) {
    if (condition) {
        CALL(addr);
    } else {
        cpu.clock += 3;
    }
}

void CCF() {
    FLAG_SET_C(!FC);
    cpu.clock += 4;
}

void CP(uint8_t val, bool indirect) {
    uint8_t tmp = cpu.AF.upper - val;

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(1);
    FLAG_SET_H(MINHALF_CARRY(cpu.AF.upper, val));
    FLAG_SET_C(MINCARRY(cpu.AF.upper, val));

    cpu.clock += (indirect) ? 8: 4;
}

void CPL() {
    uint8_t tmp = ~cpu.AF.upper;

    FLAG_SET_N(1);
    FLAG_SET_H(1);

    cpu.AF.upper = tmp;
    cpu.clock += 4;
}

void DAA() {
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
    cpu.clock += 4;
}

void DEC(reg_u *reg, bool upper) {
    uint8_t tmp = UPPER_LOWER(reg, upper) - 1;

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(1);
    FLAG_SET_H(MINHALF_CARRY(UPPER_LOWER(reg, upper), 1));

    if (upper) reg->upper = tmp;
    else       reg->lower = tmp;

    cpu.clock += 4;
}

void DECHL() {
    mem_write(cpu.HL.full, mem_read(cpu.HL.full) - 1);
    cpu.clock += 8;
}

void DEC16(uint16_t *reg) {
    uint16_t tmp = *reg - 1;

    *reg = tmp;
    cpu.clock += 8;
}

void DI() {
    cpu.IME = false;
    cpu.clock += 4;
}

void EI() {
    cpu.IME = true;
    cpu.clock += 4;
}

void HALT() {cpu.clock += 4;}

void INC(reg_u *reg, bool upper) {
    uint8_t tmp =  UPPER_LOWER(reg, upper) + 1;

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(HALF_CARRY(UPPER_LOWER(reg, upper), 1, +));

    if (upper) reg->upper = tmp;
    else       reg->lower = tmp;
    cpu.clock += 4;
}

void INCHL() {
    mem_write(cpu.HL.full, mem_read(cpu.HL.full) + 1);
    cpu.clock += 8;
}

void INC16(uint16_t *reg) {
    *reg += 1;
    cpu.clock += 8;
}

void JP() {
    cpu.PC = mem_read(cpu.PC++) | (mem_read(cpu.PC++) << 8);
    cpu.clock += 16;
}

void JPCC(bool CC) {
    if (CC) {
        JP();
    } else {
        cpu.clock += 12;
    }
}

void JPHL() {
    cpu.PC = cpu.HL.full;
    cpu.clock += 4;
}

void JR() {
    cpu.PC = (int16_t) GET_16_BIT();
    cpu.clock += 12;
}

void JRCC(bool condition) {
    if (condition) {
        JR();
    } else {
        cpu.PC++;
        cpu.clock += 8;
    }
}

void LDr8(reg_u *reg, uint8_t val, bool upper) {
    if (upper) reg->upper = val;
    else       reg->lower = val;
    cpu.clock += 4;
}

void LDr8n8(reg_u *reg, bool upper) {
    if (upper) reg->upper = mem_read(cpu.PC++);
    else       reg->lower = mem_read(cpu.PC++);
    cpu.clock += 8;
}

void LDr16n16(uint16_t *dest) {
    *dest = GET_16_BIT();
    cpu.clock += 12;
}

void LDHLr8(uint8_t val) {
    mem_write(cpu.HL.full, val);
    cpu.clock += 8;
}

void LDHLn8() {
    mem_write(cpu.HL.full, mem_read(cpu.PC++));
    cpu.clock += 8;
}

void LDr8HL(reg_u *reg, bool upper) {
    if (upper) reg->upper = mem_read(cpu.HL.full);
    else       reg->lower = mem_read(cpu.HL.full);
    cpu.clock += 8;
}

void LDr16A(uint16_t addr) {
    mem_write(addr, cpu.AF.upper);
    cpu.clock += 8;
}

void LDn16A() {
    mem_write(GET_16_BIT(), cpu.AF.upper);
    cpu.clock += 12;
}

void LDHn16A() {
    mem_write(0XFF00 + mem_read(cpu.PC++), cpu.AF.upper);
    cpu.clock += 12;
}

void LDHCA() {
    mem_write(0XFF00 + cpu.BC.lower, cpu.AF.upper);
    cpu.clock += 12;
}

void LDAr16(uint16_t addr) {
    cpu.AF.upper = mem_read(addr);
    cpu.clock += 8;
}

void LDAn16() {
    cpu.AF.upper = mem_read(GET_16_BIT());
    cpu.clock += 8;
}

void LDHAn16() {
    cpu.AF.upper = mem_read(0XFF00 + mem_read(cpu.PC++));
    cpu.clock += 12;
}

void LDHAC() {
    cpu.AF.upper = mem_read(0XFF00 + cpu.BC.lower);
    cpu.clock += 12;
}

void LDHLIA() {
    mem_write(cpu.HL.full++, cpu.AF.lower);
    cpu.clock += 8;
}

void LDHLDA() {
    mem_write(cpu.HL.full--, cpu.AF.lower);
    cpu.clock += 8;
}

void LDAHLI() {
    cpu.AF.lower = mem_read(cpu.HL.full++);
    cpu.clock += 8;
}

void LDAHLD() {
    cpu.AF.lower = mem_read(cpu.HL.full--);
    cpu.clock += 8;
}

void LDSPn16() {
    cpu.SP = GET_16_BIT();
    cpu.clock += 12;
}

void LDn16SP() {
    mem_write(mem_read(cpu.PC++), cpu.SP & 0XFF);
    mem_write(mem_read(cpu.PC++), cpu.SP << 8);

    cpu.clock += 20;
}

void LDHLSPe8() {
    uint16_t tmp = cpu.SP + (int8_t) mem_read(cpu.PC++);

    FLAG_SET_Z(0);
    FLAG_SET_N(0);
    FLAG_SET_H(HALF_CARRY(cpu.SP, mem_read(cpu.PC-1), +));
    FLAG_SET_C(CARRY(cpu.SP, mem_read(cpu.PC-1), +));

    cpu.HL.full = tmp;
    cpu.clock += 12;
}

void LDSPHL() {
    cpu.SP = cpu.HL.full;
    cpu.clock += 8;
}

void NOP() {
    cpu.clock += 4;
}

void OR(uint8_t val, bool indirect) {
    uint8_t tmp = cpu.AF.upper | val;

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(0);
    FLAG_SET_C(0);

    cpu.AF.upper = tmp;
    cpu.clock += (indirect) ? 8: 1;
}

void POP(uint16_t *reg) {
    uint16_t tmp;
    POP_FROM_STACK16BIT(tmp);

    *reg = tmp;
    cpu.clock += 12;
}

void PUSH(uint16_t val) {
    PUSH_TO_STACK16BIT(val);
    cpu.clock += 16;
}

void RES(reg_u *reg, uint8_t bit, bool upper) {
    uint8_t tmp = RESET_BIT(UPPER_LOWER(reg, upper), bit);
    if (upper) reg->upper = tmp;
    else       reg->lower = tmp;
    cpu.clock += 8;
}

void RESHL(uint8_t bit) {
    mem_write(cpu.HL.full, RESET_BIT(mem_read(cpu.HL.full), bit));
    cpu.clock += 16;
}

void RET() {
    POP_FROM_STACK16BIT(cpu.PC);
    cpu.clock += 16;
}

void RETCC(bool condition) {
    if (condition) {
        RET();
    } else {
        cpu.clock += 1;
    }

    cpu.clock += 1;
}

void RETI() {
    EI();
    RET();

    cpu.clock -= 4;
}

void RL(reg_u *reg, bool upper) {
    uint8_t tmp = (UPPER_LOWER(reg, upper) << 1) | FC;

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(0);
    FLAG_SET_C(TEST_BIT(UPPER_LOWER(reg, upper), 7));

    if (upper) reg->upper = tmp;
    else       reg->lower = tmp;
    cpu.clock += 8;
}

void RLHL() {
    uint8_t val = mem_read(cpu.HL.full);
    uint8_t tmp = (val << 1) | FC;

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(0);
    FLAG_SET_C(TEST_BIT(val, 7));

    mem_write(cpu.HL.full, tmp);
    cpu.clock += 16;
}

void RLA() {
    RL(&cpu.AF, true);
    cpu.clock -= 4;
}

void RLC(reg_u *reg, bool upper) {
    uint8_t val = UPPER_LOWER(reg, upper);
    uint8_t tmp = (val << 7) | TEST_BIT(val, 7);

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(0);
    FLAG_SET_C(TEST_BIT(val, 7));

    if (upper) reg->upper = tmp;
    else       reg->lower = tmp;
    cpu.clock += 8;
}

void RLCHL() {
    uint8_t val = mem_read(cpu.HL.full);
    uint8_t tmp = (val << 1) | TEST_BIT(val, 7);

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(0);
    FLAG_SET_C(TEST_BIT(val, 7));

    mem_write(cpu.HL.full, tmp);
    cpu.clock += 16;
}

void RLCA() {
    RLC(&cpu.AF, true);
    cpu.clock -= 4;
}

void RR(reg_u *reg, bool upper) {
    uint8_t tmp = (UPPER_LOWER(reg, upper) >> 1) | (FC << 7);

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(0);
    FLAG_SET_C(TEST_BIT(UPPER_LOWER(reg, upper), 0));

    if (upper) reg->upper = tmp;
    else       reg->lower = tmp;
    cpu.clock += 8;
}

void RRHL() {
    uint8_t val = mem_read(cpu.HL.full);
    uint8_t tmp = (val >> 1) | (FC << 7);

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(0);
    FLAG_SET_C(TEST_BIT(val, 0));

    mem_write(cpu.HL.full, tmp);
    cpu.clock += 16;
}

void RRA() {
    RR(&cpu.AF, true);
    cpu.clock -= 4;
}

void RRC(reg_u *reg, bool upper) {
    uint8_t val = UPPER_LOWER(reg, upper);
    uint8_t tmp = (val >> 1) | (TEST_BIT(val, 0) << 7);

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(0);
    FLAG_SET_C(TEST_BIT(val, 0));

    if (upper) reg->upper = tmp;
    else       reg->lower = tmp;
    cpu.clock += 8;
}

void RRCHL() {
    uint8_t val = mem_read(cpu.HL.full);
    uint8_t tmp = (val >> 1) | (TEST_BIT(val, 0) << 7);

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(0);
    FLAG_SET_C(TEST_BIT(val, 0));

    mem_write(cpu.HL.full, tmp);
    cpu.clock += 16;
}

void RRCA() {
    RRC(&cpu.AF, true);
    cpu.clock -= 4;
}

void RST(uint8_t addr) {
    PUSH_TO_STACK16BIT(cpu.PC);

    cpu.PC = 0;
    cpu.PC = addr;
    cpu.clock += 16;
}

void SBC(uint8_t val, bool implied) {
    uint8_t tmp = cpu.AF.upper - val - FC;

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(1);
    FLAG_SET_H(MINHALF_CARRY(cpu.AF.upper, val + FC));
    FLAG_SET_C(MINCARRY(cpu.AF.upper, val + FC));

    cpu.AF.upper = tmp;
    cpu.clock += 4;
}

void SCF() {
    FLAG_SET_C(1);
    cpu.clock += 4;
}

void SET(reg_u *reg, uint8_t bit, bool upper) {
    if (upper) reg->upper = SET_BIT(reg->upper, bit);
    else       reg->lower = SET_BIT(reg->lower, bit);

    cpu.clock += 8;
}

void SETHL(uint8_t bit) {
    mem_write(cpu.HL.full, SET_BIT(mem_read(cpu.HL.full), bit));
    cpu.clock += 16;
}

void SLA(reg_u *reg, bool upper) {
    uint8_t val = UPPER_LOWER(reg, upper);
    uint8_t tmp = (val << 1) | TEST_BIT(val, 7);

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(0);
    FLAG_SET_C(TEST_BIT(val, 7));

    if (upper) reg->upper = tmp;
    else       reg->lower = tmp;
    cpu.clock += 8;
}

void SLAHL() {
    uint8_t val = mem_read(cpu.HL.full);
    uint8_t tmp = (val << 1) | TEST_BIT(val, 7);

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(0);
    FLAG_SET_C(TEST_BIT(val, 7));

    mem_write(cpu.HL.full, tmp);
    cpu.clock += 8;
}

void SRA(reg_u *reg, bool upper) {
    uint8_t val = UPPER_LOWER(reg, upper);
    uint8_t tmp = (val >> 1) | (TEST_BIT(val, 7) << 7);

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(0);
    FLAG_SET_C(TEST_BIT(val, 0));

    if (upper) reg->upper = tmp;
    else       reg->lower = tmp;
    cpu.clock += 8;
}

void SRAHL() {
    uint8_t val = mem_read(cpu.HL.full);
    uint8_t tmp = (val >> 1) | (TEST_BIT(val, 7) << 7);

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(0);
    FLAG_SET_C(TEST_BIT(val, 0));

    mem_write(cpu.HL.full, tmp);
    cpu.clock += 16;
}

void SRL(reg_u *reg, bool upper) {
    uint8_t val = UPPER_LOWER(reg, upper);
    uint8_t tmp = (val >> 1) | (TEST_BIT(val, 7) << 7);

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(0);
    FLAG_SET_C(TEST_BIT(val, 0));

    if (upper) reg->upper = tmp;
    else       reg->lower = tmp;
    cpu.clock += 8;
}

void SRLHL() {
    uint8_t val = mem_read(cpu.HL.full);
    uint8_t tmp = (val >> 1) | (TEST_BIT(val, 7) << 7);

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(0);
    FLAG_SET_C(TEST_BIT(val, 0));

    mem_write(cpu.HL.full, tmp);
    cpu.clock += 16;
}

void STOP() {}

void SUB(uint8_t val, bool indirect) {
    uint8_t tmp = cpu.AF.upper - val;

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(1);
    FLAG_SET_H(MINHALF_CARRY(cpu.AF.upper, val));
    FLAG_SET_C(MINCARRY(cpu.AF.upper, val));

    cpu.AF.upper = tmp;
    cpu.clock = (indirect) ? 8: 4;
}

void SWAP(reg_u *reg, bool upper) {
    uint8_t val = UPPER_LOWER(reg, upper);
    uint8_t tmp = (val << 4) & (val >> 4);

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(0);
    FLAG_SET_C(0);

    if (upper) reg->upper = tmp;
    else       reg->lower = tmp;

    cpu.clock += 8;
}

void SWAPHL() {
    uint8_t val = mem_read(cpu.HL.full);
    uint8_t tmp = (val << 4) & (val >> 4);

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(0);
    FLAG_SET_C(0);

    mem_write(cpu.HL.full, tmp);
    cpu.clock += 16;
}

void XOR(uint8_t val, bool indirect) {
    uint8_t tmp = cpu.AF.upper | val;

    FLAG_SET_Z(tmp == 0);
    FLAG_SET_N(0);
    FLAG_SET_H(0);
    FLAG_SET_C(0);

    cpu.AF.upper = tmp;
    cpu.clock += (indirect) ? 8: 4;
}

void opcodes(uint8_t opcode) {
    switch(opcode) {
        // ADC
        case(0X88): ADC(cpu.BC.upper, 0); break;
        case(0X89): ADC(cpu.BC.lower, 0); break;
        case(0X8A): ADC(cpu.DE.upper, 0); break;
        case(0X8B): ADC(cpu.DE.lower, 0); break;
        case(0X8C): ADC(cpu.HL.upper, 0); break;
        case(0X8D): ADC(cpu.HL.lower, 0); break;
        case(0X8E): ADC(mem_read(cpu.HL.full), 1); break;
        case(0X8F): ADC(cpu.AF.lower, 0); break;

        // ADD r8
        case(0X80): ADD(cpu.BC.upper, 0); break;
        case(0X81): ADD(cpu.BC.lower, 0); break;
        case(0X82): ADD(cpu.DE.upper, 0); break;
        case(0X83): ADD(cpu.DE.lower, 0); break;
        case(0X84): ADD(cpu.HL.upper, 0); break;
        case(0X85): ADD(cpu.HL.lower, 0); break;
        case(0X86): ADD(mem_read(cpu.HL.full), 1); break;
        case(0X87): ADD(cpu.AF.lower, 0); break;

        // ADD HL, r16
        case(0X09): ADDHL(cpu.BC.full); break;
        case(0X19): ADDHL(cpu.DE.full); break;
        case(0X29): ADDHL(cpu.HL.full); break;
        case(0X39): ADDHL(cpu.SP); break;

        // ADD SP, e8
        case(0XE8): ADDSP(mem_read(cpu.PC++)); break;

        // CALL
        case(0XCD): CALL(mem_read(cpu.PC++) | (mem_read(cpu.PC) << 8)); break;
        case(0XC4): CALLCC(mem_read(cpu.PC++) | (mem_read(cpu.PC) << 8), !FZ); break;
        case(0XD4): CALLCC(mem_read(cpu.PC++) | (mem_read(cpu.PC) << 8), !FC); break;
        case(0XCC): CALLCC(mem_read(cpu.PC++) | (mem_read(cpu.PC) << 8), FZ); break;
        case(0XDC): CALLCC(mem_read(cpu.PC++) | (mem_read(cpu.PC) << 8), FC); break;

        // CCF
        case(0X3F): CCF(); break;

        // CP
        case(0XB8): CP(cpu.BC.upper, 0); break;
        case(0XB9): CP(cpu.BC.lower, 0); break;
        case(0XBA): CP(cpu.DE.upper, 0); break;
        case(0XBB): CP(cpu.DE.lower, 0); break;
        case(0XBC): CP(cpu.HL.upper, 0); break;
        case(0XBD): CP(cpu.HL.lower, 0); break;
        case(0XBE): CP(mem_read(cpu.HL.full), 1); break;
        case(0XBF): CP(cpu.AF.upper, 0); break;

        case(0XFE): CP(mem_read(cpu.PC++), 1); break;

        // CPL
        case(0X2F): CPL(); break;

        // DAA
        case(0X27): DAA(); break;

        // DEC r8
        case(0X05): DEC(&cpu.BC, 1); break;
        case(0X0D): DEC(&cpu.BC, 0); break;
        case(0X15): DEC(&cpu.DE, 1); break;
        case(0X1D): DEC(&cpu.DE, 0); break;
        case(0X25): DEC(&cpu.HL, 1); break;
        case(0X2D): DEC(&cpu.HL, 0); break;
        case(0X35): DECHL(); break;
        case(0X3D): DEC(&cpu.AF, 1); break;

        // DEC r16
        case(0X0B): DEC16(&cpu.BC.full); break;
        case(0X1B): DEC16(&cpu.DE.full); break;
        case(0X2B): DEC16(&cpu.HL.full); break;
        case(0X3B): DEC16(&cpu.SP); break;

        // DI
        case(0XF3): DI(); break;

        // EI
        case(0XFB): EI(); break;

        // HALT
        case(0X76): HALT(); break;

        // INC
        case(0X04): INC(&cpu.BC, 1); break;
        case(0X0C): INC(&cpu.BC, 0); break;
        case(0X14): INC(&cpu.DE, 1); break;
        case(0X1C): INC(&cpu.DE, 0); break;
        case(0X24): INC(&cpu.HL, 1); break;
        case(0X2C): INC(&cpu.HL, 0); break;
        case(0X34): INCHL(); break;
        case(0X3C): INC(&cpu.AF, 1); break;

        // INC r16
        case(0X03): INC16(&cpu.BC.full); break;
        case(0X13): INC16(&cpu.DE.full); break;
        case(0X23): INC16(&cpu.HL.full); break;
        case(0X33): INC16(&cpu.SP); break;

        // JP
        case(0XC3): JP(); break;
        case(0XC2): JPCC(!FZ);
        case(0XD2): JPCC(!FC);
        case(0XCA): JPCC(FZ);
        case(0XDA): JPCC(FC);

        case(0XE9): JPHL(); break;

        // JR
        case(0X20): JRCC(!FZ); break;
        case(0X30): JRCC(!FC); break;

        case(0X18): JR(); break;
        case(0X28): JRCC(FZ); break;
        case(0X38): JRCC(FC); break;

        // LD
        case(0X40): LDr8(&cpu.BC, cpu.BC.upper, 1); break;
        case(0X41): LDr8(&cpu.BC, cpu.BC.lower, 1); break;
        case(0X42): LDr8(&cpu.BC, cpu.DE.upper, 1); break;
        case(0X43): LDr8(&cpu.BC, cpu.DE.lower, 1); break;
        case(0X44): LDr8(&cpu.BC, cpu.HL.upper, 1); break;
        case(0X45): LDr8(&cpu.BC, cpu.HL.lower, 1); break;
        case(0X46): LDr8(&cpu.BC, mem_read(cpu.HL.full), 1); break;
        case(0X47): LDr8(&cpu.BC, cpu.AF.upper, 1); break;

        case(0X48): LDr8(&cpu.BC, cpu.BC.upper, 0); break;
        case(0X49): LDr8(&cpu.BC, cpu.BC.lower, 0); break;
        case(0X4A): LDr8(&cpu.BC, cpu.DE.upper, 0); break;
        case(0X4B): LDr8(&cpu.BC, cpu.DE.lower, 0); break;
        case(0X4C): LDr8(&cpu.BC, cpu.HL.upper, 0); break;
        case(0X4D): LDr8(&cpu.BC, cpu.HL.lower, 0); break;
        case(0X4E): LDr8(&cpu.BC, mem_read(cpu.HL.full), 0); break;
        case(0X4F): LDr8(&cpu.BC, cpu.AF.upper, 0); break;

        case(0X50): LDr8(&cpu.DE, cpu.BC.upper, 1); break;
        case(0X51): LDr8(&cpu.DE, cpu.BC.lower, 1); break;
        case(0X52): LDr8(&cpu.DE, cpu.DE.upper, 1); break;
        case(0X53): LDr8(&cpu.DE, cpu.DE.lower, 1); break;
        case(0X54): LDr8(&cpu.DE, cpu.HL.upper, 1); break;
        case(0X55): LDr8(&cpu.DE, cpu.HL.lower, 1); break;
        case(0X56): LDr8(&cpu.DE, mem_read(cpu.HL.full), 1); break;
        case(0X57): LDr8(&cpu.DE, cpu.AF.upper, 1); break;

        case(0X58): LDr8(&cpu.DE, cpu.BC.upper, 0); break;
        case(0X59): LDr8(&cpu.DE, cpu.BC.lower, 0); break;
        case(0X5A): LDr8(&cpu.DE, cpu.DE.upper, 0); break;
        case(0X5B): LDr8(&cpu.DE, cpu.DE.lower, 0); break;
        case(0X5C): LDr8(&cpu.DE, cpu.HL.upper, 0); break;
        case(0X5D): LDr8(&cpu.DE, cpu.HL.lower, 0); break;
        case(0X5E): LDr8(&cpu.DE, mem_read(cpu.HL.full), 0); break;
        case(0X5F): LDr8(&cpu.DE, cpu.AF.upper, 0); break;

        case(0X68): LDr8(&cpu.HL, cpu.BC.upper, 1); break;
        case(0X69): LDr8(&cpu.HL, cpu.BC.lower, 1); break;
        case(0X6A): LDr8(&cpu.HL, cpu.DE.upper, 1); break;
        case(0X6B): LDr8(&cpu.HL, cpu.DE.lower, 1); break;
        case(0X6C): LDr8(&cpu.HL, cpu.HL.upper, 1); break;
        case(0X6D): LDr8(&cpu.HL, cpu.HL.lower, 1); break;
        case(0X6E): LDr8(&cpu.HL, mem_read(cpu.HL.full), 1); break;
        case(0X6F): LDr8(&cpu.HL, cpu.AF.upper, 1); break;

        case(0X60): LDr8(&cpu.HL, cpu.BC.upper, 0); break;
        case(0X61): LDr8(&cpu.HL, cpu.BC.lower, 0); break;
        case(0X62): LDr8(&cpu.HL, cpu.DE.upper, 0); break;
        case(0X63): LDr8(&cpu.HL, cpu.DE.lower, 0); break;
        case(0X64): LDr8(&cpu.HL, cpu.HL.upper, 0); break;
        case(0X65): LDr8(&cpu.HL, cpu.HL.lower, 0); break;
        case(0X66): LDr8(&cpu.HL, mem_read(cpu.HL.full), 0); break;
        case(0X67): LDr8(&cpu.HL, cpu.AF.upper, 0); break;

        case(0X70): LDHLr8(cpu.BC.upper); break;
        case(0X71): LDHLr8(cpu.BC.lower); break;
        case(0X72): LDHLr8(cpu.DE.upper); break;
        case(0X73): LDHLr8(cpu.DE.lower); break;
        case(0X74): LDHLr8(cpu.HL.upper); break;
        case(0X75): LDHLr8(cpu.HL.lower); break;
        case(0X77): LDHLr8(cpu.AF.upper); break;

        case(0X78): LDr8(&cpu.HL, cpu.BC.upper, 1); break;
        case(0X79): LDr8(&cpu.HL, cpu.BC.lower, 1); break;
        case(0X7A): LDr8(&cpu.HL, cpu.DE.upper, 1); break;
        case(0X7B): LDr8(&cpu.HL, cpu.DE.lower, 1); break;
        case(0X7C): LDr8(&cpu.HL, cpu.HL.upper, 1); break;
        case(0X7D): LDr8(&cpu.HL, cpu.HL.lower, 1); break;
        case(0X7E): LDr8(&cpu.HL, mem_read(cpu.HL.full), 1); break;
        case(0X7F): LDr8(&cpu.HL, cpu.AF.upper, 1); break;

        case(0X06): LDr8n8(&cpu.BC, 1); break;
        case(0X16): LDr8n8(&cpu.DE, 1); break;
        case(0X26): LDr8n8(&cpu.HL, 1); break;
        case(0X36): LDHLn8(); break;

        case(0X0E): LDr8n8(&cpu.BC, 0); break;
        case(0X1E): LDr8n8(&cpu.DE, 0); break;
        case(0X2E): LDr8n8(&cpu.HL, 0); break;
        case(0X3E): LDr8n8(&cpu.AF, 1); break;

        case(0X02): LDr16A(cpu.BC.full); break;
        case(0X12): LDAr16(cpu.DE.full); break;
        case(0X22): LDHLIA(); break;
        case(0X32): LDHLDA(); break;

        case(0X0A): LDAr16(cpu.BC.full); break;
        case(0X1A): LDAr16(cpu.DE.full); break;
        case(0X2A): LDAHLI(); break;
        case(0X3A): LDAHLD(); break;

        case(0X01): LDr16n16(&cpu.BC.full); break;
        case(0X11): LDr16n16(&cpu.DE.full); break;
        case(0X21): LDr16n16(&cpu.HL.full); break;
        case(0X31): LDSPn16(); break;

        case(0X08): LDn16SP(); break;

        case(0XE0): LDHn16A(); break;
        case(0XF0): LDHAn16(); break;

        case(0XE2): LDHCA(); break;
        case(0XF2): LDHAC(); break;

        case(0XF8): LDHLSPe8(); break;
        case(0XF9): LDSPHL(); break;

        case(0XEA): LDn16A(); break;
        case(0XFA): LDAn16(); break;

        // NOP
        case(0X00): NOP(); break;

        // OR
        case(0XB0): OR(cpu.BC.upper, 0); break;
        case(0XB1): OR(cpu.BC.lower, 0); break;
        case(0XB2): OR(cpu.DE.upper, 0); break;
        case(0XB3): OR(cpu.DE.lower, 0); break;
        case(0XB4): OR(cpu.HL.upper, 0); break;
        case(0XB5): OR(cpu.HL.lower, 0); break;
        case(0XB6): OR(mem_read(cpu.HL.full), 1); break;
        case(0XB7): OR(cpu.AF.upper, 0); break;

        case(0XF6): OR(mem_read(cpu.PC), 1); break;

        // POP
        case(0XC1): POP(&cpu.BC.full); break;
        case(0XD1): POP(&cpu.DE.full); break;
        case(0XE1): POP(&cpu.HL.full); break;
        case(0XF1): POP(&cpu.AF.full); break;

        // PUSH
        case(0XC5): PUSH(cpu.BC.full); break;
        case(0XD5): PUSH(cpu.DE.full); break;
        case(0XE5): PUSH(cpu.HL.full); break;
        case(0XF5): PUSH(cpu.AF.full); break;

        // RET
        case(0XC9): RET(); break;
        case(0XC0): RETCC(!FZ); break;
        case(0XD0): RETCC(!FZ); break;
        case(0XC8): RETCC(FZ); break;
        case(0XD8): RETCC(FZ); break;

        case(0XD9): RETI(); break;

        // RRA
        case(0X1F): RRA(); break;
        case(0X0F): RRCA(); break;

        // RLA
        case(0X17): RLA(); break;
        case(0X07): RLCA(); break;

        // RST
        case(0XC7): RST(0X00); break;
        case(0XD7): RST(0X10); break;
        case(0XE7): RST(0X20); break;
        case(0XF7): RST(0X30); break;
        case(0XCF): RST(0X08); break;
        case(0XDF): RST(0X18); break;
        case(0XEF): RST(0X28); break;
        case(0XFF): RST(0X38); break;

        // SBC
        case(0X98): SBC(cpu.BC.upper, 0); break;
        case(0X99): SBC(cpu.BC.lower, 0); break;
        case(0X9A): SBC(cpu.DE.upper, 0); break;
        case(0X9B): SBC(cpu.DE.lower, 0); break;
        case(0X9C): SBC(cpu.HL.upper, 0); break;
        case(0X9D): SBC(cpu.HL.lower, 0); break;
        case(0X9E): SBC(mem_read(cpu.HL.full), 1); break;
        case(0X9F): SBC(cpu.AF.upper, 0); break;

        case(0XDE): SBC(mem_read(cpu.PC++), 1); break;

        // SCF
        case(0X37): SCF(); break;

        // STOP
        case(0X10): STOP(); break;

        // SUC
        case(0X90): SUB(cpu.BC.upper, 0); break;
        case(0X91): SUB(cpu.BC.lower, 0); break;
        case(0X92): SUB(cpu.DE.upper, 0); break;
        case(0X93): SUB(cpu.DE.lower, 0); break;
        case(0X94): SUB(cpu.HL.upper, 0); break;
        case(0X95): SUB(cpu.HL.lower, 0); break;
        case(0X96): SUB(mem_read(cpu.HL.full), 1); break;
        case(0X97): SUB(cpu.AF.upper, 0); break;

        case(0XD6): SUB(mem_read(cpu.PC++), 1); break;

        // XOR
        case(0XA8): XOR(cpu.BC.upper, 0); break;
        case(0XA9): XOR(cpu.BC.lower, 0); break;
        case(0XAA): XOR(cpu.DE.upper, 0); break;
        case(0XAB): XOR(cpu.DE.lower, 0); break;
        case(0XAC): XOR(cpu.HL.upper, 0); break;
        case(0XAD): XOR(cpu.HL.lower, 0); break;
        case(0XAE): XOR(mem_read(cpu.HL.full), 1); break;
        case(0XAF): XOR(cpu.AF.upper, 0); break;

        // prefix on
        case(0XEE): XOR(mem_read(cpu.PC), 1); break;

        case(0XCB): cpu.prefix = true; break;
        default: break;
    }
}

void prefixed(uint8_t opcode) {
    reg_u *reg;
    bool upper, mem;
    uint16_t addr;

    // determine which register/mem location is being affected
    switch(cpu.CIR & 0X0F) {
        case(0X00): reg = &cpu.BC; upper = 1; mem = 0; cpu.clock += 8; break;
        case(0X01): reg = &cpu.BC; upper = 0; mem = 0; cpu.clock += 8; break;
        case(0X02): reg = &cpu.DE; upper = 1; mem = 0; cpu.clock += 8; break;
        case(0X03): reg = &cpu.DE; upper = 0; mem = 0; cpu.clock += 8; break;
        case(0X04): reg = &cpu.HL; upper = 1; mem = 0; cpu.clock += 8; break;
        case(0X05): reg = &cpu.HL; upper = 0; mem = 0; cpu.clock += 8; break;
        case(0X07): reg = &cpu.AF; upper = 1; mem = 0; cpu.clock += 8; break;

        case(0X08): reg = &cpu.BC; upper = 1; mem = 0; cpu.clock += 8; break;
        case(0X09): reg = &cpu.BC; upper = 0; mem = 0; cpu.clock += 8; break;
        case(0X0A): reg = &cpu.DE; upper = 1; mem = 0; cpu.clock += 8; break;
        case(0X0B): reg = &cpu.DE; upper = 0; mem = 0; cpu.clock += 8; break;
        case(0X0C): reg = &cpu.HL; upper = 1; mem = 0; cpu.clock += 8; break;
        case(0X0D): reg = &cpu.HL; upper = 0; mem = 0; cpu.clock += 8; break;
        case(0X0F): reg = &cpu.AF; upper = 1; mem = 0; cpu.clock += 8; break;

        case(0X06): addr = mem_read(cpu.HL.full); mem = 1; cpu.clock += 16; break;
        case(0X0E): addr = mem_read(cpu.HL.full); mem = 1; cpu.clock += 16; break;
    }

    switch(cpu.CIR) {
        // RLC
        case(0X00):
        case(0X01):
        case(0X02):
        case(0X03):
        case(0X04):
        case(0X05): RLC(reg, upper); break;
        case(0X06): RLCHL(); break;
        case(0X07): RLCA(); break;

        // RRC
        case(0X08):
        case(0X09):
        case(0X0A):
        case(0X0B):
        case(0X0C):
        case(0X0D): RRC(reg, upper); break;
        case(0X0E): RRCHL(); break;
        case(0X0F): RRCA(); break;

        // RL
        case(0X10):
        case(0X11):
        case(0X12):
        case(0X13):
        case(0X14):
        case(0X15): RL(reg, upper); break;
        case(0X16): RLHL(); break;
        case(0X17): RLA(); break;

        // RR
        case(0X18):
        case(0X19):
        case(0X1A):
        case(0X1B):
        case(0X1C):
        case(0X1D): RR(reg, upper); break;
        case(0X1E): RRHL(); break;
        case(0X1F): RRA(); break;


        // SLA
        case(0X20):
        case(0X21):
        case(0X22):
        case(0X23):
        case(0X24):
        case(0X25):
        case(0X27): SLA(reg, upper); break;
        case(0X26): SLAHL(); break;

        // SRA
        case(0X28):
        case(0X29):
        case(0X2A):
        case(0X2B):
        case(0X2C):
        case(0X2D):
        case(0X2F): SRA(reg, upper); break;;
        case(0X2E): SRAHL(); break;

        // SWAP
        case(0X30):
        case(0X31):
        case(0X32):
        case(0X33):
        case(0X34):
        case(0X35):
        case(0X37): SWAP(reg, upper); break;
        case(0X36): SWAPHL(); break;

        // SRL
        case(0X38):
        case(0X39):
        case(0X3A):
        case(0X3B):
        case(0X3C):
        case(0X3D):
        case(0X3F): SRL(reg, upper); break;;
        case(0X3E): SRLHL(); break;

        // BIT 0
        case(0X40):
        case(0X41):
        case(0X42):
        case(0X43):
        case(0X44):
        case(0X45):
        case(0X47): BIT(UPPER_LOWER(reg, upper), 0, 0); break;
        case(0X46): BIT(mem_read(cpu.HL.full), 0, 1); break;

        // BIT 1
        case(0X48):
        case(0X49):
        case(0X4A):
        case(0X4B):
        case(0X4C):
        case(0X4D):
        case(0X4F): BIT(UPPER_LOWER(reg, upper), 1, 0); break;
        case(0X4E): BIT(mem_read(cpu.HL.full), 1, 1); break;

        // BIT 2
        case(0X50):
        case(0X51):
        case(0X52):
        case(0X53):
        case(0X54):
        case(0X55):
        case(0X57): BIT(UPPER_LOWER(reg, upper), 2, 0); break;
        case(0X56): BIT(mem_read(cpu.HL.full), 2, 1); break;

        // BIT 3
        case(0X58):
        case(0X59):
        case(0X5A):
        case(0X5B):
        case(0X5C):
        case(0X5D):
        case(0X5F): BIT(UPPER_LOWER(reg, upper), 3, 0); break;
        case(0X5E): BIT(mem_read(cpu.HL.full), 3, 1); break;

        // BIT 4
        case(0X60):
        case(0X61):
        case(0X62):
        case(0X63):
        case(0X64):
        case(0X65):
        case(0X67): BIT(UPPER_LOWER(reg, upper), 4, 0); break;
        case(0X66): BIT(mem_read(cpu.HL.full), 4, 1); break;

        // BIT 6
        case(0X68):
        case(0X69):
        case(0X6A):
        case(0X6B):
        case(0X6C):
        case(0X6D):
        case(0X6F): BIT(UPPER_LOWER(reg, upper), 5, 0); break;
        case(0X6E): BIT(mem_read(cpu.HL.full), 5, 1); break;

        // BIT 6
        case(0X70):
        case(0X71):
        case(0X72):
        case(0X73):
        case(0X74):
        case(0X75):
        case(0X77): BIT(UPPER_LOWER(reg, upper), 6, 0); break;
        case(0X76): BIT(mem_read(cpu.HL.full), 6, 1); break;

        // BIT 7
        case(0X78):
        case(0X79):
        case(0X7A):
        case(0X7B):
        case(0X7C):
        case(0X7D):
        case(0X7F): BIT(UPPER_LOWER(reg, upper), 7, 0); break;
        case(0X7E): BIT(mem_read(cpu.HL.full), 7, 1); break;

        // RES 0
        case(0X80):
        case(0X81):
        case(0X82):
        case(0X83):
        case(0X84):
        case(0X85):
        case(0X87): RES(reg, upper, 0); break;
        case(0X86): RESHL(0); break;

        // RES 1
        case(0X88):
        case(0X89):
        case(0X8A):
        case(0X8B):
        case(0X8C):
        case(0X8D):
        case(0X8F): RES(reg, upper, 1); break;
        case(0X8E): RESHL(1); break;

        // RES 2
        case(0X90):
        case(0X91):
        case(0X92):
        case(0X93):
        case(0X94):
        case(0X95):
        case(0X97): RES(reg, upper, 2); break;
        case(0X96): RESHL(2); break;

        // RES 9
        case(0X98):
        case(0X99):
        case(0X9A):
        case(0X9B):
        case(0X9C):
        case(0X9D):
        case(0X9F): RES(reg, upper, 3); break;
        case(0X9E): RESHL(3); break;

        // RES 4
        case(0XA0):
        case(0XA1):
        case(0XA2):
        case(0XA3):
        case(0XA4):
        case(0XA5):
        case(0XA7): RES(reg, upper, 4); break;
        case(0XA6): RESHL(4); break;

        // RES A
        case(0XA8):
        case(0XA9):
        case(0XAA):
        case(0XAB):
        case(0XAC):
        case(0XAD):
        case(0XAF): RES(reg, upper, 5); break;
        case(0XAE): RESHL(5); break;

        // RES 6
        case(0XB0):
        case(0XB1):
        case(0XB2):
        case(0XB3):
        case(0XB4):
        case(0XB5):
        case(0XB7): RES(reg, upper, 6); break;
        case(0XB6): RESHL(6); break;

        // RES B
        case(0XB8):
        case(0XB9):
        case(0XBA):
        case(0XBB):
        case(0XBC):
        case(0XBD):
        case(0XBF): RES(reg, upper, 7); break;
        case(0XBE): RESHL(7); break;


        // SET 0
        case(0XC0):
        case(0XC1):
        case(0XC2):
        case(0XC3):
        case(0XC4):
        case(0XC5):
        case(0XC7): SET(reg, upper, 0); break;
        case(0XC6): SETHL(0); break;

        // SET 1
        case(0XC8):
        case(0XC9):
        case(0XCA):
        case(0XCB):
        case(0XCC):
        case(0XCD):
        case(0XCF): SET(reg, upper, 1); break;
        case(0XCE): SETHL(1); break;

        // SET 2
        case(0XD0):
        case(0XD1):
        case(0XD2):
        case(0XD3):
        case(0XD4):
        case(0XD5):
        case(0XD7): SET(reg, upper, 2); break;
        case(0XD6): SETHL(2); break;

        // SET 3
        case(0XD8):
        case(0XD9):
        case(0XDA):
        case(0XDB):
        case(0XDC):
        case(0XDD):
        case(0XDF): SET(reg, upper, 3); break;
        case(0XDE): SETHL(3); break;

        // SET 4
        case(0XE0):
        case(0XE1):
        case(0XE2):
        case(0XE3):
        case(0XE4):
        case(0XE5):
        case(0XE7): SET(reg, upper, 4); break;
        case(0XE6): SETHL(4); break;

        // SET 5
        case(0XE8):
        case(0XE9):
        case(0XEA):
        case(0XEB):
        case(0XEC):
        case(0XED):
        case(0XEF): SET(reg, upper, 5); break;
        case(0XEE): SETHL(5); break;

        // SET 6
        case(0XF0):
        case(0XF1):
        case(0XF2):
        case(0XF3):
        case(0XF4):
        case(0XF5):
        case(0XF7): SET(reg, upper, 6); break;
        case(0XF6): SETHL(6); break;

        // SET 7
        case(0XF8):
        case(0XF9):
        case(0XFA):
        case(0XFB):
        case(0XFC):
        case(0XFD):
        case(0XFF): SET(reg, upper, 7); break;
        case(0XFE): SETHL(7); break;
    }
}
