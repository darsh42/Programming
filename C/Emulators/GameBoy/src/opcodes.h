#ifndef OPCODES_H_INCLUDED
#define OPCODES_H_INCLUDED

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

void ADDHL(uint8_t val) {
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

void INC16(uint16_t *reg) {
    *reg += 1;
    cpu.clock += 8;
}

void JP(uint16_t addr) {
    cpu.PC = addr;
    cpu.clock += 16;
}

void JPCC(uint16_t addr, bool CC) {
    if (CC) {
        JP(addr);
    } else {
        cpu.clock += 12;
    }
}

void JPHL() {
    cpu.PC = cpu.HL.full;
    cpu.clock += 4;
}

// TODO: LOAD instructions
void LDr8(uint8_t *dest, uint8_t *src) {
    *dest = *src;
    cpu.clock += 4;
}

void LDr8n8(uint8_t *dest, uint8_t val) {
    *dest = val;
    cpu.clock += 8;
}

void LDr16n16(uint16_t *dest, uint16_t *src) {
    *dest = *src;
    cpu.clock += 12;
}

void LDHLr8(uint8_t *src) {
    uint8_t tmp = mem_read(cpu.HL.full) + *src;
    mem_write(cpu.HL.full, tmp);
    cpu.clock += 8;
}
void LDHLn8(uint8_t val) {
    uint8_t tmp = mem_read(cpu.HL.full) + val;
    mem_write(cpu.HL.full, tmp);
    cpu.clock += 8;
}

void LDr8HL(uint8_t *dest) {
    *dest = mem_read(cpu.HL.full);
    cpu.clock += 8;
}

void LDr16A(uint16_t addr) {
    mem_write(addr, cpu.AF.upper);
    cpu.clock += 8;
}

void LDn16A(uint16_t addr) {
    mem_write(addr, cpu.AF.upper);
    cpu.clock += 12;
}

void LDHn16(uint8_t addr) {
    mem_write(0XFF00 + addr, cpu.AF.upper);
    cpu.clock += 12;
}

void LDHC() {
    mem_write(0XFF00 + cpu.BC.lower, cpu.AF.upper);
    cpu.clock += 8;
}
// TODO: LOAD instrs

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

void POPAF() { POP(&cpu.AF.full); }
void PUSHAF() { PUSH(cpu.AF.full); }

void RESr8(reg_u *reg, uint8_t bit, bool upper) {
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

#endif // OPCODES_H_INCLUDED
