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

    mem_write(--cpu.SP, cpu.PC);
    mem_write(--cpu.SP, cpu.PC >> 8);
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
    cpu.AF.lower = (cpu.AF.lower & (mask << 4)) | ((Z << 7) & (N << 6) & (H << 5) & (C << 4));
}

/* Fetch Decode Execute of instructions */
void cpu_exec() {
    // general help variables
    uint16_t nn;
    bool Z, N, H, C;

    // load current instruction
    cpu.CIR = mem_read(cpu.PC++);

    if (cpu.prefix) {
        /* Prefix OPcodes */

        reg_u *reg;
        bool upper;
        bool mem;

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

            case(0X06): nn = mem_read(cpu.HL.full); mem = 1; cpu.clock += 16; break;
            case(0X0E): nn = mem_read(cpu.HL.full); mem = 1; cpu.clock += 16; break;
        }

        // prefixed opcode implementation
        if ((cpu.CIR & 0X0F) < 0X08) {
            // Left half of prefix instructions
            switch(cpu.CIR & 0XF0) {
                case(0X00):
                    /* RLC r - 2 bytes, 8 cycles *
                    * Rotate B left             *
                    * FLAGS: Z,0,0,C            */
                    if (mem) {
                        Z = (nn << 1) == 0;
                        N = 0;
                        H = 0;
                        C = CARRY(nn, 1, <<);

                        nn <<= 1;
                        mem_write(cpu.HL.full, nn);
                        flag_write(Z, N, H, C, 0b0000);
                    } else if (upper) {
                        Z = (reg->upper << 1) == 0;
                        N = 0;
                        H = 0;
                        C = CARRY(reg->upper, 1, <<);

                        reg->upper <<= 1;
                        flag_write(Z, N, H, C, 0b0000);
                    } else {
                        Z = (reg->lower << 1) == 0;
                        N = 0;
                        H = 0;
                        C = CARRY(reg->lower, 1, <<);

                        reg->lower <<= 1;
                        flag_write(Z, N, H, C, 0b0000);
                    }
                    break;
                case(0X10):
                    /* RL B - 2 bytes, 8 cycles *
                    * Rotate left though carry *
                    * FLAGS: Z,0,0,C           */
                    if (mem) {
                        Z = ((nn << 1) | FC) == 0;
                        N = 0;
                        H = 0;
                        C = CARRY(nn, 1, <<);

                        nn <<= 1;
                        nn |= FC;
                        mem_write(cpu.HL.full, nn);
                        flag_write(Z, N, H, C, 0b0000);
                    } else if (upper) {
                        Z = ((reg->upper << 1) | FC) == 0;
                        N = 0;
                        H = 0;
                        C = CARRY(reg->upper, 1, <<);

                        reg->upper <<= 1;
                        reg->upper |= FC;
                        flag_write(Z, N, H, C, 0b0000);
                    } else {
                        Z = ((reg->upper << 1) | FC) == 0;
                        N = 0;
                        H = 0;
                        C = CARRY(reg->upper, 1, <<);

                        reg->upper <<= 1;
                        reg->upper |= FC;
                        flag_write(Z, N, H, C, 0b0000);
                    }
                    break;
                case (0X20):
                    /* SLA B - 2 bytes, 8 cycles *
                    * shift left arithmetic     *
                    * FLAGS: Z,0,0,C            */
                    if (mem) {
                        Z = (nn << 1) == 0;
                        N = 0;
                        H = 0;
                        C = CARRY(nn, 1, <<);

                        nn <<= 1;
                        mem_write(cpu.HL.full, nn);
                        flag_write(Z, N, H, C, 0b0000);
                    } else if (upper) {
                        Z = (reg->upper << 1) == 0;
                        N = 0;
                        H = 0;
                        C = CARRY(reg->upper, 1, <<);

                        reg->upper <<= 1;
                        flag_write(Z, N, H, C, 0b0000);
                    } else {
                        Z = (reg->lower << 1) == 0;
                        N = 0;
                        H = 0;
                        C = CARRY(reg->lower, 1, <<);

                        reg->lower <<= 1;
                        flag_write(Z, N, H, C, 0b0000);
                    }
                    break;
                case(0X30):
                    /* SWAP B - 2 bytes, 8 cycles         *
                    * Swap upper and lower 4 bits in reg *
                    * FLAGS: Z,0,0,0                     */
                    if (mem) {
                        Z = ((nn & 0XF << 4) | (nn >> 4)) == 0;
                        N = 0;
                        H = 0;
                        C = 0;

                        nn = (nn & 0XF << 4) | (nn >> 4);
                        mem_write(cpu.HL.full, nn);
                        flag_write(Z, N, H, C, 0b0000);
                    } else if (upper) {
                        Z = ((reg->upper & 0XF << 4) | (reg->upper >> 4)) == 0;
                        N = 0;
                        H = 0;
                        C = 0;

                        reg->upper = (reg->upper & 0XF << 4) | (reg->upper >> 4);
                        flag_write(Z, N, H, C, 0b0000);
                    } else {
                        Z = ((reg->lower & 0XF << 4) | (reg->lower >> 4)) == 0;
                        N = 0;
                        H = 0;
                        C = 0;

                        reg->lower = (reg->lower & 0XF << 4) | (reg->lower >> 4);
                        flag_write(Z, N, H, C, 0b0000);
                    }
                    break;
                case(0X40):
                    /* BIT 0, B - 2 bytes, 8 cycles *
                    * Test if bit 0 is Set         *
                    * FLAGS: Z,0,1,-               */
                    if (mem) {
                        Z = (nn & 0X01) == 0X01;
                        N = 0;
                        H = 1;
                        C = 0;

                        flag_write(Z, N, H, C, 0b0001);
                    } else if (upper) {
                        Z = (reg->upper & 0X01) == 0X01;
                        N = 0;
                        H = 1;
                        C = 0;

                        flag_write(Z, N, H, C, 0b0001);
                    } else {
                        Z = (reg->lower & 0X01) == 0X01;
                        N = 0;
                        H = 1;
                        C = 0;

                        flag_write(Z, N, H, C, 0b0001);
                    }
                    break;
                case(0X50):
                    /* BIT 2, B - 2 bytes, 8 cycles *
                    * Test if bit 2 is Set         *
                    * FLAGS: Z,0,1,-               */
                    if (mem) {
                        Z = (nn & 0X04) == 0X04;
                        N = 0;
                        H = 1;
                        C = 0;

                        flag_write(Z, N, H, C, 0b0001);
                    } else if (upper) {
                        Z = (reg->upper & 0X04) == 0X04;
                        N = 0;
                        H = 1;
                        C = 0;

                        flag_write(Z, N, H, C, 0b0001);
                    } else {
                        Z = (reg->lower & 0X04) == 0X04;
                        N = 0;
                        H = 1;
                        C = 0;

                        flag_write(Z, N, H, C, 0b0001);
                    }
                    break;
                case(0X60):
                    /* BIT 4, B - 2 bytes, 8 cycles *
                    * Test if bit 4 is Set         *
                    * FLAGS: Z,0,1,-               */
                    if (mem) {
                        Z = (nn & 0X10) == 0X10;
                        N = 0;
                        H = 1;
                        C = 0;

                        flag_write(Z, N, H, C, 0b0001);
                    } else if (upper) {
                        Z = (reg->upper & 0X10) == 0X10;
                        N = 0;
                        H = 1;
                        C = 0;

                        flag_write(Z, N, H, C, 0b0001);
                    } else {
                        Z = (reg->lower & 0X10) == 0X10;
                        N = 0;
                        H = 1;
                        C = 0;

                        flag_write(Z, N, H, C, 0b0001);
                    }
                    break;
                case(0X70):
                    /* BIT 6, B - 2 bytes, 8 cycles *
                    * Test if bit 6 is Set         *
                    * FLAGS: Z,0,1,-               */
                    if (mem) {
                        Z = (nn & 0X40) == 0X40;
                        N = 0;
                        H = 1;
                        C = 0;

                        flag_write(Z, N, H, C, 0b0001);
                    } else if (upper) {
                        Z = (reg->upper & 0X40) == 0X40;
                        N = 0;
                        H = 1;
                        C = 0;

                        flag_write(Z, N, H, C, 0b0001);
                    } else {
                        Z = (reg->lower & 0X40) == 0X40;
                        N = 0;
                        H = 1;
                        C = 0;

                        flag_write(Z, N, H, C, 0b0001);
                    }
                    break;
                case(0X80):
                    /* RES 0, R - 2 bytes, 8 cycles *
                    * Set bit 0 to 0                *
                    * FLAGS: -,-,-,-               */
                    if (mem) {
                        nn &= 0b11111110;
                        mem_write(cpu.HL.full, nn);
                    } else if (upper) {
                        reg->upper &= 0b11111110;
                    } else {
                        reg->lower &= 0b11111110;
                    }
                    break;
                case(0X90):
                    /* RES 2, R - 2 bytes, 8 cycles *
                    * Set bit 2 to 0                *
                    * FLAGS: -,-,-,-               */
                    if (mem) {
                        nn &= 0b11111011;
                        mem_write(cpu.HL.full, nn);
                    } else if (upper) {
                        reg->upper &= 0b11111011;
                    } else {
                        reg->lower &= 0b11111011;
                    }
                    break;
                case(0XA0):
                    /* RES 4, R - 2 bytes, 8 cycles *
                    * Set bit 0 to 0                *
                    * FLAGS: -,-,-,-               */
                    if (mem) {
                        nn &= 0b11101111;
                        mem_write(cpu.HL.full, nn);
                    } else if (upper) {
                        reg->upper &= 0b11101111;
                    } else {
                        reg->lower &= 0b11101111;
                    }
                    break;
                case(0XB0):
                    /* RES 6, R - 2 bytes, 8 cycles *
                    * Set bit 0 to 0                *
                    * FLAGS: -,-,-,-               */
                    if (mem) {
                        nn &= 0b10111111;
                        mem_write(cpu.HL.full, nn);
                    } else if (upper) {
                        reg->upper &= 0b10111111;
                    } else {
                        reg->lower &= 0b10111111;
                    }
                    break;
                case(0XC0):
                    /* SET 0, R - 2 bytes, 8 cycles *
                    * Set bit 0 to 1                *
                    * FLAGS: -,-,-,-               */
                    if (mem) {
                        nn |= 0b00000001;
                        mem_write(cpu.HL.full, nn);
                    } else if (upper) {
                        reg->upper |= 0b00000001;
                    } else {
                        reg->lower |= 0b00000001;
                    }
                    break;
                case(0XD0):
                    /* SET 0, R - 2 bytes, 8 cycles *
                    * Set bit 0 to 1                *
                    * FLAGS: -,-,-,-               */
                    if (mem) {
                        nn |= 0b00000100;
                        mem_write(cpu.HL.full, nn);
                    } else if (upper) {
                        reg->upper |= 0b00000100;
                    } else {
                        reg->lower |= 0b00000100;
                    }
                    break;
                case(0XE0):
                    /* SET 0, R - 2 bytes, 8 cycles *
                    * Set bit 0 to 1                *
                    * FLAGS: -,-,-,-               */
                    if (mem) {
                        nn |= 0b00010000;
                        mem_write(cpu.HL.full, nn);
                    } else if (upper) {
                        reg->upper |= 0b00010000;
                    } else {
                        reg->lower |= 0b00010000;
                    }
                    break;
                case(0XF0):
                    /* SET 0, R - 2 bytes, 8 cycles *
                    * Set bit 0 to 1                *
                    * FLAGS: -,-,-,-               */
                    if (mem) {
                        nn |= 0b01000000;
                        mem_write(cpu.HL.full, nn);
                    } else if (upper) {
                        reg->upper |= 0b01000000;
                    } else {
                        reg->lower |= 0b01000000;
                    }
                    break;
            }
        } else {
            // Right half of prefix instructions
            switch(cpu.CIR & 0XF0) {
                case(0X00):
                    /* RRC r - 2 bytes, 8 cycles *
                    * Rotate B left             *
                    * FLAGS: Z,0,0,C            */
                    if (mem) {
                        Z = (nn >> 1) == 0;
                        N = 0;
                        H = 0;
                        C = nn & 0X01;

                        nn <<= 1;
                        mem_write(cpu.HL.full, nn);
                        flag_write(Z, N, H, C, 0b0000);
                    } else if (upper) {
                        Z = (reg->upper >> 1) == 0;
                        N = 0;
                        H = 0;
                        C = reg->upper & 0X01;

                        reg->upper >>= 1;
                        flag_write(Z, N, H, C, 0b0000);
                    } else {
                        Z = (reg->lower >> 1) == 0;
                        N = 0;
                        H = 0;
                        C = reg->lower & 0X01;

                        reg->lower >>= 1;
                        flag_write(Z, N, H, C, 0b0000);
                    }
                    break;
                case(0X10):
                    /* RR B - 2 bytes, 8 cycles *
                    * Rotate right though carry *
                    * FLAGS: Z,0,0,C           */
                    if (mem) {
                        Z = ((nn >> 1) | (FC >> 8)) == 0;
                        N = 0;
                        H = 0;
                        C = nn & 0X01;

                        nn >>= 1;
                        nn |= (FC >> 8);
                        mem_write(cpu.HL.full, nn);
                        flag_write(Z, N, H, C, 0b0000);
                    } else if (upper) {
                        Z = ((reg->upper >> 1) | (FC >> 8)) == 0;
                        N = 0;
                        H = 0;
                        C = reg->upper & 0X01;

                        reg->upper >>= 1;
                        reg->upper |= (FC >> 8);
                        flag_write(Z, N, H, C, 0b0000);
                    } else {
                        Z = ((reg->upper >> 1) | (FC >> 8)) == 0;
                        N = 0;
                        H = 0;
                        C = reg->lower & 0X01;

                        reg->upper >>= 1;
                        reg->upper |= (FC >> 8);
                        flag_write(Z, N, H, C, 0b0000);
                    }
                    break;
                case (0X20):
                    /* SRA B - 2 bytes, 8 cycles *
                    * shift right arithmetic, msb stays same     *
                    * FLAGS: Z,0,0,C            */
                    if (mem) {
                        Z = (nn >> 1) == 0;
                        N = 0;
                        H = 0;
                        C = nn & 0X01;

                        nn = (nn & 0X80) | (nn >> 7);
                        mem_write(cpu.HL.full, nn);
                        flag_write(Z, N, H, C, 0b0000);
                    } else if (upper) {
                        Z = (reg->upper >> 1) == 0;
                        N = 0;
                        H = 0;
                        C = reg->upper & 0X01;

                        reg->upper = (reg->upper & 0X80) | (reg->upper >> 1);
                        flag_write(Z, N, H, C, 0b0000);
                    } else {
                        Z = (reg->lower >> 1) == 0;
                        N = 0;
                        H = 0;
                        C = reg->lower & 0X01;

                        reg->lower = (reg->lower & 0X80) | (reg->lower >> 1);
                        flag_write(Z, N, H, C, 0b0000);
                    }
                    break;
                case(0X30):
                    /* SRL r - 2 bytes, 8 cycles *
                    * Rotate B left             *
                    * FLAGS: Z,0,0,C            */
                    if (mem) {
                        Z = (nn >> 1) == 0;
                        N = 0;
                        H = 0;
                        C = nn & 0X01;

                        nn <<= 1;
                        mem_write(cpu.HL.full, nn);
                        flag_write(Z, N, H, C, 0b0000);
                    } else if (upper) {
                        Z = (reg->upper >> 1) == 0;
                        N = 0;
                        H = 0;
                        C = reg->upper & 0X01;

                        reg->upper >>= 1;
                        flag_write(Z, N, H, C, 0b0000);
                    } else {
                        Z = (reg->lower >> 1) == 0;
                        N = 0;
                        H = 0;
                        C = reg->lower & 0X01;

                        reg->lower >>= 1;
                        flag_write(Z, N, H, C, 0b0000);
                    }
                    break;
                case(0X40):
                    /* BIT 0, B - 2 bytes, 8 cycles *
                    * Test if bit 0 is Set         *
                    * FLAGS: Z,0,1,-               */
                    if (mem) {
                        Z = (nn & 0b00000010) == 0b00000010;
                        N = 0;
                        H = 1;
                        C = 0;

                        flag_write(Z, N, H, C, 0b0001);
                    } else if (upper) {
                        Z = (reg->upper & 0b00000010) == 0b00000010;
                        N = 0;
                        H = 1;
                        C = 0;

                        flag_write(Z, N, H, C, 0b0001);
                    } else {
                        Z = (reg->lower & 0b00000010) == 0b00000010;
                        N = 0;
                        H = 1;
                        C = 0;

                        flag_write(Z, N, H, C, 0b0001);
                    }
                    break;
                case(0X50):
                    /* BIT 2, B - 2 bytes, 8 cycles *
                    * Test if bit 2 is Set         *
                    * FLAGS: Z,0,1,-               */
                    if (mem) {
                        Z = (nn & 0b00001000) == 0b00001000;
                        N = 0;
                        H = 1;
                        C = 0;

                        flag_write(Z, N, H, C, 0b0001);
                    } else if (upper) {
                        Z = (reg->upper & 0b00001000) == 0b00001000;
                        N = 0;
                        H = 1;
                        C = 0;

                        flag_write(Z, N, H, C, 0b0001);
                    } else {
                        Z = (reg->lower & 0b00001000) == 0b00001000;
                        N = 0;
                        H = 1;
                        C = 0;

                        flag_write(Z, N, H, C, 0b0001);
                    }
                    break;
                case(0X60):
                    /* BIT 4, B - 2 bytes, 8 cycles *
                    * Test if bit 4 is Set         *
                    * FLAGS: Z,0,1,-               */
                    if (mem) {
                        Z = (nn & 0b00100000) == 0b00100000;
                        N = 0;
                        H = 1;
                        C = 0;

                        flag_write(Z, N, H, C, 0b0001);
                    } else if (upper) {
                        Z = (reg->upper & 0b00100000) == 0b00100000;
                        N = 0;
                        H = 1;
                        C = 0;

                        flag_write(Z, N, H, C, 0b0001);
                    } else {
                        Z = (reg->lower & 0b00100000) == 0b00100000;
                        N = 0;
                        H = 1;
                        C = 0;

                        flag_write(Z, N, H, C, 0b0001);
                    }
                    break;
                case(0X70):
                    /* BIT 6, B - 2 bytes, 8 cycles *
                    * Test if bit 6 is Set         *
                    * FLAGS: Z,0,1,-               */
                    if (mem) {
                        Z = (nn & 0b10000000) == 0b10000000;
                        N = 0;
                        H = 1;
                        C = 0;

                        flag_write(Z, N, H, C, 0b0001);
                    } else if (upper) {
                        Z = (reg->upper & 0b10000000) == 0b10000000;
                        N = 0;
                        H = 1;
                        C = 0;

                        flag_write(Z, N, H, C, 0b0001);
                    } else {
                        Z = (reg->lower & 0b10000000) == 0b10000000;
                        N = 0;
                        H = 1;
                        C = 0;

                        flag_write(Z, N, H, C, 0b0001);
                    }
                    break;
                case(0X80):
                    /* RES 0, R - 2 bytes, 8 cycles *
                    * Set bit 0 to 0                *
                    * FLAGS: -,-,-,-               */
                    if (mem) {
                        nn &= 0b11111101;
                        mem_write(cpu.HL.full, nn);
                    } else if (upper) {
                        reg->upper &= 0b11111101;
                    } else {
                        reg->lower &= 0b11111101;
                    }
                    break;
                case(0X90):
                    /* RES 2, R - 2 bytes, 8 cycles *
                    * Set bit 2 to 0                *
                    * FLAGS: -,-,-,-               */
                    if (mem) {
                        nn &= 0b11110111;
                        mem_write(cpu.HL.full, nn);
                    } else if (upper) {
                        reg->upper &= 0b11110111;
                    } else {
                        reg->lower &= 0b11110111;
                    }
                    break;
                case(0XA0):
                    /* RES 4, R - 2 bytes, 8 cycles *
                    * Set bit 0 to 0                *
                    * FLAGS: -,-,-,-               */
                    if (mem) {
                        nn &= 0b11011111;
                        mem_write(cpu.HL.full, nn);
                    } else if (upper) {
                        reg->upper &= 0b11011111;
                    } else {
                        reg->lower &= 0b11011111;
                    }
                    break;
                case(0XB0):
                    /* RES 6, R - 2 bytes, 8 cycles *
                    * Set bit 0 to 0                *
                    * FLAGS: -,-,-,-               */
                    if (mem) {
                        nn &= 0b01111111;
                        mem_write(cpu.HL.full, nn);
                    } else if (upper) {
                        reg->upper &= 0b01111111;
                    } else {
                        reg->lower &= 0b01111111;
                    }
                    break;
                case(0XC0):
                    /* SET 0, R - 2 bytes, 8 cycles *
                    * Set bit 0 to 1                *
                    * FLAGS: -,-,-,-               */
                    if (mem) {
                        nn |= 0b00000010;
                        mem_write(cpu.HL.full, nn);
                    } else if (upper) {
                        reg->upper |= 0b00000010;
                    } else {
                        reg->lower |= 0b00000010;
                    }
                    break;
                case(0XD0):
                    /* SET 0, R - 2 bytes, 8 cycles *
                    * Set bit 0 to 1                *
                    * FLAGS: -,-,-,-               */
                    if (mem) {
                        nn |= 0b00001000;
                        mem_write(cpu.HL.full, nn);
                    } else if (upper) {
                        reg->upper |= 0b00001000;
                    } else {
                        reg->lower |= 0b00001000;
                    }
                    break;
                case(0XE0):
                    /* SET 0, R - 2 bytes, 8 cycles *
                    * Set bit 0 to 1                *
                    * FLAGS: -,-,-,-               */
                    if (mem) {
                        nn |= 0b00100000;
                        mem_write(cpu.HL.full, nn);
                    } else if (upper) {
                        reg->upper |= 0b00100000;
                    } else {
                        reg->lower |= 0b00100000;
                    }
                    break;
                case(0XF0):
                    /* SET 0, R - 2 bytes, 8 cycles *
                    * Set bit 0 to 1                *
                    * FLAGS: -,-,-,-               */
                    if (mem) {
                        nn |= 0b10000000;
                        mem_write(cpu.HL.full, nn);
                    } else if (upper) {
                        reg->upper |= 0b10000000;
                    } else {
                        reg->lower |= 0b10000000;
                    }
                    break;
            }
        }
    } else {
        /* Non-Prefix OPcodes */

        switch(cpu.CIR) {
            case(0X00):
                /* NOP - 1 byte, 4 cycles         */
                cpu.clock += 4;
                break;
            case(0X01):
                /* LD BC, NN - 3 bytes, 12 cycles  *
                * Loads 16bit value into register */
                nn = mem_read(cpu.PC++);
                nn |= mem_read(cpu.PC++) << 8;

                cpu.BC.full = nn;

                cpu.clock += 12;
                break;
            case(0X02):
                /* LD (BC), A - 1 byte, 8 cycles *
                    * Load 8 bit A into ABS addr BC */
                mem_write(cpu.BC.full, cpu.AF.upper);
                cpu.clock += 8;
                break;
            case(0X03):
                /* INC BC - 1 byte, 8 cycles *
                    * Increment register BC     */
                cpu.BC.full++;
                cpu.clock += 8;
                break;
            case(0X04):
                /* INC B - 1 byte, 4 cycles *
                    * Increment register B     *
                    * FLAGS: Z,0,H,-           */
                Z = cpu.BC.upper + 1 == 0;
                N = 0;
                H = HALF_CARRY(cpu.BC.upper, 1, +);
                C = 0;

                cpu.BC.upper++;

                flag_write(Z, N, H, C, 0b0001);
                cpu.clock += 4;
                break;
            case(0X05):
                /* DEC B - bytes 1, cycles 4 *
                * Decrement Register B      *
                * FLAGS: Z,1,H,-            */
                Z = cpu.BC.upper - 1 == 0;
                N = 1;
                H = HALF_CARRY(cpu.BC.upper, 1, -);
                C = 0;

                cpu.BC.upper--;

                flag_write(Z, N, H, C, 0b0001);
                cpu.clock += 4;
                break;
            case(0X06):
                /* LD B, d8 - bytes 2, cycles 8 *
                * Load 8bit value into B       *
                * FLAGS: -,-,-,-,              */
                nn = mem_read(cpu.PC++);
                cpu.BC.upper = nn;
                cpu.clock += 8;
                break;
            case(0X07):
                /* RCLA - 1 byte, 4 cycles       *
                    * Rotate(shift) register A left *
                    * FLAGS: 0,0,0,C                */
                C = CARRY(cpu.AF.upper, 1, <<);
                N = 0;
                Z = 0;
                H = 0;

                cpu.AF.upper <<= 1;

                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0X08):
                /* LD (a16), SP - 3 bytes, 20 cycles *
                    * Load to (a16) value in SP         *
                    * FLAGS: -,-,-,-                    */
                cpu.SP = mem_read(cpu.PC++) << 8;
                cpu.SP |= mem_read(cpu.PC++);

                cpu.clock += 20;
                break;
            case(0X09):
                /* ADD HL, BC - 1 byte, 8 cycles *
                    * Add BC to HL                  *
                    * FLAGS: -,0,H,C                */
                Z = 0;
                N = 0;
                H = HALF_CARRY(cpu.HL.full, cpu.BC.full, +);
                C = CARRY(cpu.HL.full, cpu.BC.full, +);

                cpu.HL.full += cpu.BC.full;

                flag_write(Z, N, H, C, 0b1000);
                cpu.clock += 8;
                break;
            case(0X0A):
                /* LD A, (BC) - 1 byte, 8 cycles *
                    * Load (BC) to reg A            *
                    * FLAGS: -,-,-,-                */
                nn = mem_read(cpu.BC.full);
                cpu.AF.upper = nn;
                cpu.clock += 8;
                break;
            case(0X0B):
                /* DEC BC - 1 byte, 8 cycles    *
                    * Decrement register BC        *
                    * FLAGS: -,-,-,-               */
                cpu.BC.full--;
                cpu.clock += 8;
                break;
            case(0X0C):
                /* INC C - 1 byte, 4 cycles *
                    * Increment register C     *
                    * FLAGS: Z,0,H,-           */
                Z = cpu.BC.lower + 1 == 0;
                N = 0;
                H = HALF_CARRY(cpu.BC.lower, 1, +);
                C = 0;

                cpu.BC.lower++;

                flag_write(Z, N, H, C, 0b0001);
                cpu.clock += 4;
                break;
            case(0X0D):
                /* DEC C - 1 byte, 4 cycles *
                    * Decrement Register C     *
                    * FLAGS: Z,0,H,-           */
                Z = cpu.BC.lower - 1 == 0;
                N = 0;
                H = HALF_CARRY(cpu.BC.lower, 1, -);
                C = 0;

                cpu.BC.lower--;

                flag_write(Z, N, H, C, 0b0001);
                cpu.clock += 4;
                break;
            case(0X0E):
                /* LD C, d8 - 2 bytes, 8 cycles *
                * Load byte into C             *
                * FLAGS: -,-,-,-,              */
                nn = mem_read(cpu.PC);
                cpu.PC+=1;
                cpu.BC.lower = nn;
                cpu.clock += 8;
                break;
            case(0X0F): {
                /* RCLA - 1 byte, 4 cycles       *
                    * Rotate(shift) register A left *
                    * FLAGS: 0,0,0,C                */
                C = cpu.AF.upper & 0X01;
                N = 0;
                Z = 0;
                H = 0;

                cpu.AF.upper >>= 1;

                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            }
            case(0X10):
                /* LD DE, NN - 3 bytes, 12 cycles  *
                    * Loads 16bit value into register *
                    * FLAGS: -,-,-,-                  */
                nn = mem_read(cpu.PC++);
                nn |= mem_read(cpu.PC++) << 8;

                cpu.DE.full = nn;

                cpu.clock += 12;
                break;
            case(0X11):
                /* STOP 0 - 2 bytes, ? cycle *
                * low power standby mode    *
                * FLAGS: -,-,-,-            */
                break;
            case(0X12):
                /* LD [DE], A - 1 byte, 8 cycles *
                * Load A into [DE]              *
                * FLAGS: -,-,-,-                */
                mem_write(cpu.DE.full, cpu.AF.upper);
                cpu.clock += 8;
                break;
            case(0X13):
                /* INC DE - 1 byte, 8 cycles *
                    * Increment register DE     */
                cpu.DE.full++;
                cpu.clock += 8;
                break;
            case(0X14):
                /* INC D - 1 byte, 4 cycles *
                    * Increment register D     *
                    * FLAGS: Z,0,H,-           */
                Z = cpu.DE.upper + 1 == 0;
                N = 0;
                H = HALF_CARRY(cpu.DE.upper, 1, +);
                C = 0;

                cpu.DE.upper++;

                flag_write(Z, N, H, C, 0b0001);
                cpu.clock += 4;
                break;
            case(0X15):
                /* DEC D - bytes 1, cycles 4 *
                    * Decrement Register D      *
                    * FLAGS: Z,1,H,-            */
                Z = cpu.DE.upper - 1 == 0;
                N = 1;
                H = HALF_CARRY(cpu.DE.upper, 1, -);
                C = 0;

                cpu.DE.upper--;

                flag_write(Z, N, H, C, 0b0001);
                cpu.clock += 4;
                break;
            case(0X16):
                /* LD D, n8 - 2 bytes, 20 cycles *
                    * Load to D value in n8         *
                    * FLAGS: -,-,-,-                */
                nn = mem_read(cpu.PC);
                cpu.PC+=1;
                cpu.DE.upper = nn;
                cpu.clock += 20;
                break;
            case(0X17): {
                /* RRA - 1 byte, 4 cycles       *
                    * Rotate(shift) register A left *
                    * FLAGS: 0,0,0,C                */
                C = cpu.AF.upper & 0X80;
                N = 0;
                Z = 0;
                H = 0;

                cpu.AF.upper >>= 1;
                cpu.AF.upper |= cpu.AF.lower & 0X10;

                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            }
            case(0X18):
                /* JR e8 - 2bytes, 12 cycles  *
                    * Jump to PC + e8            *
                    * FLAGS: -,-,-,-             */
                nn = mem_read(cpu.PC);
                cpu.PC+=1;
                cpu.PC += nn;
                cpu.clock += 12;
                break;
            case(0X19):
                /* ADD HL, DE - 1 byte, 8 cycles *
                    * Add DE to HL                  *
                    * FLAGS: -,0,H,C                */
                Z = 0;
                N = 0;
                H = HALF_CARRY(cpu.HL.full, cpu.DE.full, +);
                C = CARRY(cpu.HL.full, cpu.DE.full, +);

                cpu.HL.full += cpu.DE.full;

                flag_write(Z, N, H, C, 0b1000);
                cpu.clock += 8;
                break;
            case(0X1A):
                /* LD A, [DE] - 1 byte, 8 cycles *
                    * Load [DE] to reg A            *
                    * FLAGS: -,-,-,-                */
                mem_read(cpu.DE.full);
                cpu.clock += 8;
                break;
            case(0X1B):
                /* DEC DE - 1 byte, 8 cycles    *
                    * Decrement register DE        *
                    * FLAGS: -,-,-,-               */
                cpu.DE.full--;
                cpu.clock += 8;
                break;
            case(0X1C):
                /* INC E - 1 byte, 4 cycles *
                * Increment register E     *
                * FLAGS: Z,0,H,-           */
                Z = cpu.DE.lower + 1 == 0;
                N = 0;
                H = HALF_CARRY(cpu.DE.lower, 1, +);
                C = 0;

                cpu.DE.lower++;

                flag_write(Z, N, H, C, 0b0001);
                cpu.clock += 4;
                break;
            case(0X1D):
                /* DEC E - 1 byte, 4 cycles *
                    * Decrement Register E     *
                    * FLAGS: Z,0,H,-           */
                Z = cpu.DE.lower - 1 == 0;
                N = 0;
                H = HALF_CARRY(cpu.DE.lower, 1, -);
                C = 0;

                cpu.DE.lower--;

                flag_write(Z, N, H, C, 0b0001);
                cpu.clock += 4;
                break;
            case(0X1E):
                /* LD C, d8 - 2 bytes, 8 cycles *
                    * Load byte into C             *
                    * FLAGS: -,-,-,-,              */
                nn = mem_read(cpu.PC);
                cpu.PC+=1;
                cpu.DE.lower = nn;
                cpu.clock += 8;
                break;
            case(0X1F): {
                /* RRA - 1 byte, 4 cycles        *
                    * Rotate(shift) register A left *
                    * FLAGS: 0,0,0,C                */
                C = cpu.AF.upper & 0X01;
                N = 0;
                Z = 0;
                H = 0;

                cpu.AF.upper >>= 1;
                cpu.AF.upper |= (cpu.AF.lower & 0X10) >> 4;

                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            }
            case(0X20):
                /* JR NC, e8 - 2bytes, 12 cycles *
                * Jump to PC + e8 if not C      *
                * FLAGS: -,-,-,-                */
                C = FC;

                if (!C) {
                    nn = mem_read(cpu.PC++);
                    cpu.PC += nn;
                }
                cpu.clock += 12;
                break;
            case(0X21):
                /* LD HL, NN - 3 bytes, 12 cycles  *
                * Loads 16bit value into register */
                nn = mem_read(cpu.PC++);
                nn |= mem_read(cpu.PC++) << 8;

                cpu.HL.full = nn;
                cpu.clock += 12;
                break;
            case(0X22):
                /* LD [HL+], A - 1 byte, 8 cycles *
                    * Load A into [HL+]              *
                    * FLAGS: -,-,-,-                 */
                mem_write(cpu.HL.full + 1, cpu.AF.upper);
                cpu.clock += 8;
                break;
            case(0X23):
                /* INC HL - 1 byte, 8 cycles *
                    * Increment register HL     */
                cpu.HL.full++;
                cpu.clock += 8;
                break;
            case(0X24):
                /* INC H - 1 byte, 4 cycles *
                    * Increment register H     *
                    * FLAGS: Z,0,H,-           */
                Z = cpu.HL.upper + 1 == 0;
                N = 0;
                H = HALF_CARRY(cpu.HL.upper, 1, +);
                C = 0;

                cpu.HL.upper++;

                flag_write(Z, N, H, C, 0b0001);
                cpu.clock += 4;
                break;
            case(0X25):
                /* DEC H - bytes 1, cycles 4 *
                    * Decrement Register H      *
                    * FLAGS: Z,1,H,-            */
                Z = cpu.HL.upper - 1 == 0;
                N = 1;
                H = HALF_CARRY(cpu.HL.upper, 1, -);
                C = 0;

                cpu.HL.upper--;

                flag_write(Z, N, H, C, 0b0001);
                cpu.clock += 4;
                break;
            case(0X26):
                /* LD H, n8 - 2 bytes, 20 cycles *
                    * Load to D value in n8         *
                    * FLAGS: -,-,-,-                */
                nn = mem_read(cpu.PC);
                cpu.PC += 1;
                cpu.HL.upper = nn;
                cpu.clock += 20;
                break;
            case(0X27):
                /* DAA - 1 byte, 4 cycles  *
                * Fixes ops for BCD
                * FLAGS: Z,-,0,C */
                // get flags for ease of use
                N = FN;
                H = FH;
                C = FC;

                if (!N) {
                    if (C || cpu.AF.upper > 0X99) {cpu.AF.upper += 0X60; C = 1;}
                    if (H || (cpu.AF.upper & 0X0F) > 0X09) {cpu.AF.upper += 0X06;}
                } else {
                    if (C) {cpu.AF.upper -= 0X60;}
                    if (H) {cpu.AF.upper -= 0X06;}
                }

                Z = cpu.AF.upper == 0;
                H = 0;
                flag_write(Z, N, H, C, 0b0100);
                cpu.clock += 4;
                break;
            case(0X28):
                /* JR Z, e8 - 2bytes, 12 cycles  *
                    * Jump to PC + e8 if Z          *
                    * FLAGS: -,-,-,-                */
                cpu.PC += 1;

                Z = cpu.AF.lower & 0X80 >> 7;
                nn = mem_read(cpu.PC);

                if (Z) cpu.PC += nn;

                cpu.clock += 12;
                break;
            case(0X29):
                /* ADD HL, HL - 1 byte, 8 cycles *
                    * Add HL to HL                  *
                    * FLAGS: -,0,H,C                */
                Z = 0;
                N = 0;
                H = HALF_CARRY(cpu.HL.full, cpu.HL.full, +);
                C = CARRY(cpu.HL.full, cpu.HL.full, +);

                cpu.HL.full += cpu.HL.full;

                flag_write(Z, N, H, C, 0b1000);
                cpu.clock += 8;
                break;
            case(0X2A):
                /* LD A, [HL+] - 1 byte, 8 cycles *
                * Load [HL+] to reg A            *
                * FLAGS: -,-,-,-                 */
                cpu.AF.upper = mem_read(cpu.HL.full + 1);
                cpu.clock += 8;
                break;
            case(0X2B):
                /* DEC HL - 1 byte, 8 cycles    *
                    * Decrement register HL        *
                    * FLAGS: -,-,-,-               */
                cpu.HL.full--;
                cpu.clock += 8;
                break;
            case(0X2C):
                /* INC L - 1 byte, 4 cycles *
                * Increment register L     *
                * FLAGS: Z,0,H,-           */
                Z = cpu.HL.lower + 1 == 0;
                N = 0;
                H = HALF_CARRY(cpu.HL.lower, 1, +);
                C = 0;

                cpu.HL.lower++;

                flag_write(Z, N, H, C, 0b0001);
                cpu.clock += 4;
                break;
            case(0X2D):
                /* DEC L - 1 byte, 4 cycles *
                    * Decrement Register L     *
                    * FLAGS: Z,0,H,-           */
                Z = cpu.HL.lower - 1 == 0;
                N = 0;
                H = HALF_CARRY(cpu.HL.lower, 1, -);
                C = 0;

                cpu.HL.lower--;

                flag_write(Z, N, H, C, 0b0001);
                cpu.clock += 4;
                break;
            case(0X2E):
                /* LD C, d8 - 2 bytes, 8 cycles *
                    * Load byte into C             *
                    * FLAGS: -,-,-,-,              */
                nn = mem_read(cpu.PC);
                cpu.PC += 1;
                cpu.HL.lower = nn;
                cpu.clock += 8;
                break;
            case(0X2F):
                /* CPL - 1 byte, 4 cycles   *
                    * complement A set N and H *
                    * FLAGS: -,1,1,-           */
                Z = 0;
                N = 1;
                H = 1;
                C = 0;
                cpu.AF.upper = !cpu.AF.upper;

                flag_write(Z, N, H, C, 0b1001);
                cpu.clock += 4;
                break;
            case(0X30):
                /* JR NZ, e8 - 2bytes, 12 cycles *
                    * Jump to PC + e8 if not Z      *
                    * FLAGS: -,-,-,-                */
                Z = cpu.AF.lower & 0X80 >> 7;
                nn = mem_read(cpu.PC);
                cpu.PC += 1;
                if (!Z) cpu.PC += nn;

                cpu.clock += 12;
                break;
            case(0X31):
                /* LD SP, NN - 3 bytes, 12 cycles  *
                * Loads 16bit value into register */
                nn = mem_read(cpu.PC++);
                nn |= mem_read(cpu.PC++) << 8;

                cpu.SP = nn;
                cpu.clock += 12;
                break;
            case(0X32):
                /* LD [HL-], A - 1 byte, 8 cycles *
                * Load A into [HL-]              *
                * FLAGS: -,-,-,-                 */
                mem_write(cpu.HL.full - 1, cpu.AF.upper);
                cpu.clock += 8;
                break;
            case(0X33):
                /* INC SP - 1 byte, 8 cycles *
                    * Increment register SP     */
                cpu.SP++;
                cpu.clock += 8;
                break;
            case(0X34):
                /* INC [HL] - 1 byte, 4 cycles *
                    * Increment location [HL]     *
                    * FLAGS: Z,0,H,-              */
                nn = mem_read(cpu.HL.full);
                mem_write(cpu.HL.full, nn + 1);

                Z = nn + 1 == 0;
                N = 0;
                H = HALF_CARRY(nn, 1, +);
                C = 0;

                flag_write(Z, N, H, C, 0b0001);
                cpu.clock += 4;
                break;
            case(0X35):
                /* DEC [HL] - bytes 1, cycles 4 *
                    * Decrement location [HL]      *
                    * FLAGS: Z,1,H,-               */
                nn = mem_read(cpu.HL.full);
                mem_write(cpu.HL.full, nn - 1);

                Z = nn - 1 == 0;
                N = 1;
                H = HALF_CARRY(nn, 1, -);
                C = 0;

                flag_write(Z, N, H, C, 0b0001);
                cpu.clock += 4;
                break;
            case(0X36):
                /* LD [HL], n8 - 2 bytes, 20 cycles *
                    * Load to [HL] value in n8         *
                    * FLAGS: -,-,-,-                   */
                cpu.PC += 1;

                nn = mem_read(cpu.PC);
                mem_write(cpu.HL.full, nn);

                cpu.clock += 20;
                break;
            case(0X37):
                /* SCF - 1 byte, 4 cycles  *
                    * Set Carry Flag          *
                    * FLAGS: -,0,0,1          */
                Z = 0;
                N = 0;
                H = 0;
                C = 1;

                flag_write(Z, N, H, C, 0b1000);
                cpu.clock += 4;
                break;
            case(0X38):
                /* JR C, e8 - 2bytes, 12 cycles  *
                    * Jump to PC + e8 if C          *
                    * FLAGS: -,-,-,-                */
                cpu.PC += 1;

                C = cpu.AF.lower & 0X40 >> 4;
                nn = mem_read(cpu.PC);

                if (C) cpu.PC += nn;

                cpu.clock += 12;
                break;
            case(0X39):
                /* ADD HL, SP - 1 byte, 8 cycles *
                    * Add SP to HL                  *
                    * FLAGS: -,0,H,C                */
                Z = 0;
                N = 0;
                H = HALF_CARRY(cpu.HL.full, cpu.SP, +);
                C = CARRY(cpu.HL.full, cpu.SP, +);

                cpu.HL.full += cpu.SP;

                flag_write(Z, N, H, C, 0b1000);
                cpu.clock += 8;
                break;
            case(0X3A):
                /* LD A, [HL+] - 1 byte, 8 cycles *
                    * Load [HL+] to reg A            *
                    * FLAGS: -,-,-,-                 */
                mem_read(cpu.HL.full - 1);
                cpu.clock += 8;
                break;
            case(0X3B):
                /* DEC SP - 1 byte, 8 cycles    *
                    * Decrement register SP        *
                    * FLAGS: -,-,-,-               */
                cpu.SP--;
                cpu.clock += 8;
                break;
            case(0X3C):
                /* INC A - 1 byte, 4 cycles *
                    * Increment register A     *
                    * FLAGS: Z,0,H,-           */
                Z = cpu.AF.upper + 1 == 0;
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, 1, +);
                C = 0;

                cpu.AF.upper++;

                flag_write(Z, N, H, C, 0b0001);
                cpu.clock += 4;
                break;
            case(0X3D):
                /* DEC A - 1 byte, 4 cycles *
                    * Decrement Register A     *
                    * FLAGS: Z,0,H,-           */
                Z = cpu.AF.upper - 1 == 0;
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, 1, +);
                C = 0;

                cpu.AF.upper--;

                flag_write(Z, N, H, C, 0b0001);
                cpu.clock += 4;
                break;
            case(0X3E):
                /* LD C, d8 - 2 bytes, 8 cycles *
                * Load byte into C             *
                * FLAGS: -,-,-,-,              */
                cpu.BC.lower = mem_read(cpu.PC++);
                cpu.clock += 8;
                break;
            case(0X3F):
                /* CCF - 1 byte, 4 cycles   *
                    * Clears N, H| Sets C = !C *
                    * FLAGS: -,0,0,C           */
                Z = 0;
                N = 0;
                H = 0;
                C = !((cpu.AF.lower & 0X10) == 0X10);

                flag_write(Z, N, H, C, 0b1000);
                cpu.clock += 4;
                break;
            case(0X40):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load B in to B             *
                    * FLAGS: -,-,-,-             */
                cpu.BC.upper = cpu.BC.upper;
                cpu.clock += 4;
                break;
            case(0X41):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load C in to B             *
                    * FLAGS: -,-,-,-             */
                cpu.BC.upper = cpu.BC.lower;
                cpu.clock += 4;
                break;
            case(0X42):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load D in to B             *
                    * FLAGS: -,-,-,-             */
                cpu.BC.upper = cpu.DE.upper;
                cpu.clock += 4;
                break;
            case(0X43):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load E in to B             *
                    * FLAGS: -,-,-,-             */
                cpu.BC.upper = cpu.DE.lower;
                cpu.clock += 4;
                break;
            case(0X44):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load H in to B             *
                    * FLAGS: -,-,-,-             */
                cpu.BC.upper = cpu.HL.upper;
                cpu.clock += 4;
                break;
            case(0X45):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load L in to B             *
                    * FLAGS: -,-,-,-             */
                cpu.BC.upper = cpu.HL.lower;
                cpu.clock += 4;
                break;
            case(0X46):
                /* LD B, [HL] - 1 byte, 8 cycles *
                    * Load [HL] in to B          *
                    * FLAGS: -,-,-,-             */
                nn = mem_read(cpu.HL.full);
                cpu.BC.upper = nn;
                cpu.clock += 8;
                break;
            case(0X47):
                /* LD B, B - 1 byte, 4 cycles *
                * Load A in to B             *
                * FLAGS: -,-,-,-             */
                cpu.BC.upper = cpu.AF.upper;
                cpu.clock += 4;
                break;
            case(0X48):
                /* LD C, B - 1 byte, 4 cycles *
                    * Load B in to C             *
                    * FLAGS: -,-,-,-             */
                cpu.BC.lower = cpu.BC.upper;
                cpu.clock += 4;
                break;
            case(0X49):
                /* LD C, B - 1 byte, 4 cycles *
                    * Load C in to C             *
                    * FLAGS: -,-,-,-             */
                cpu.BC.lower = cpu.BC.lower;
                cpu.clock += 4;
                break;
            case(0X4A):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load D in to C             *
                    * FLAGS: -,-,-,-             */
                cpu.BC.lower = cpu.DE.upper;
                cpu.clock += 4;
                break;
            case(0X4B):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load E in to C             *
                    * FLAGS: -,-,-,-             */
                cpu.BC.lower = cpu.DE.lower;
                cpu.clock += 4;
                break;
            case(0X4C):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load H in to C             *
                    * FLAGS: -,-,-,-             */
                cpu.BC.lower = cpu.HL.upper;
                cpu.clock += 4;
                break;
            case(0X4D):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load L in to C             *
                    * FLAGS: -,-,-,-             */
                cpu.BC.lower = cpu.HL.lower;
                cpu.clock += 4;
                break;
            case(0X4E):
                /* LD B, B - 1 byte, 8 cycles *
                    * Load [HL] in to C          *
                    * FLAGS: -,-,-,-             */
                nn = mem_read(cpu.HL.full);
                cpu.BC.lower = nn;
                cpu.clock += 8;
                break;
            case(0X4F):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load A in to C             *
                    * FLAGS: -,-,-,-             */
                cpu.BC.lower = cpu.AF.upper;
                cpu.clock += 4;
                break;
            case(0X50):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load B in to D             *
                    * FLAGS: -,-,-,-             */
                cpu.DE.upper = cpu.BC.upper;
                cpu.clock += 4;
                break;
            case(0X51):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load C in to D             *
                    * FLAGS: -,-,-,-             */
                cpu.DE.upper = cpu.BC.lower;
                cpu.clock += 4;
                break;
            case(0X52):
                /* LD B, B - 1 byte, 4 cycles *
                * Load D in to D             *
                * FLAGS: -,-,-,-             */
                cpu.DE.upper = cpu.DE.upper;
                cpu.clock += 4;
                break;
            case(0X53):
                /* LD B, B - 1 byte, 4 cycles *
                * Load B in to B             *
                * FLAGS: -,-,-,-             */
                cpu.DE.upper = cpu.DE.lower;
                cpu.clock += 4;
                break;
            case(0X54):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load H in to D             *
                    * FLAGS: -,-,-,-             */
                cpu.DE.upper = cpu.HL.upper;
                cpu.clock += 4;
                break;
            case(0X55):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load L in to D             *
                    * FLAGS: -,-,-,-             */
                cpu.DE.upper = cpu.HL.lower;
                cpu.clock += 4;
                break;
            case(0X56):
                /* LD B, B - 1 byte, 8 cycles *
                    * Load [HL] in to D          *
                    * FLAGS: -,-,-,-             */
                nn = mem_read(cpu.HL.full);
                cpu.DE.upper = nn;
                cpu.clock += 8;
                break;
            case(0X57):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load A in to D             *
                    * FLAGS: -,-,-,-             */
                cpu.DE.upper = cpu.AF.upper;
                cpu.clock += 4;
                break;
            case(0X58):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load B in to E             *
                    * FLAGS: -,-,-,-             */
                cpu.DE.lower = cpu.BC.upper;
                cpu.clock += 4;
                break;
            case(0X59):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load C in to E             *
                    * FLAGS: -,-,-,-             */
                cpu.DE.lower = cpu.BC.lower;
                cpu.clock += 4;
                break;
            case(0X5A):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load D in to E             *
                    * FLAGS: -,-,-,-             */
                cpu.DE.lower = cpu.DE.upper;
                cpu.clock += 4;
                break;
            case(0X5B):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load E in to E             *
                    * FLAGS: -,-,-,-             */
                cpu.DE.lower = cpu.DE.lower;
                cpu.clock += 4;
                break;
            case(0X5C):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load H in to E             *
                    * FLAGS: -,-,-,-             */
                cpu.DE.lower = cpu.HL.upper;
                cpu.clock += 4;
                break;
            case(0X5D):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load L in to E             *
                    * FLAGS: -,-,-,-             */
                cpu.DE.lower = cpu.HL.lower;
                cpu.clock += 4;
                break;
            case(0X5E):
                /* LD B, B - 1 byte, 8 cycles *
                    * Load [HL] in to E          *
                    * FLAGS: -,-,-,-             */
                nn = mem_read(cpu.HL.full);
                cpu.DE.lower = nn;
                cpu.clock += 8;
                break;
            case(0X5F):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load A in to E             *
                    * FLAGS: -,-,-,-             */
                cpu.DE.lower = cpu.AF.upper;
                cpu.clock += 4;
                break;
            case(0X60):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load B in to H             *
                    * FLAGS: -,-,-,-             */
                cpu.HL.upper = cpu.BC.upper;
                cpu.clock += 4;
                break;
            case(0X61):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load C in to H             *
                    * FLAGS: -,-,-,-             */
                cpu.HL.upper = cpu.BC.lower;
                cpu.clock += 4;
                break;
            case(0X62):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load D in to H             *
                    * FLAGS: -,-,-,-             */
                cpu.HL.upper = cpu.DE.upper;
                cpu.clock += 4;
                break;
            case(0X63):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load E in to H             *
                    * FLAGS: -,-,-,-             */
                cpu.HL.upper = cpu.DE.lower;
                cpu.clock += 4;
                break;
            case(0X64):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load H in to H             *
                    * FLAGS: -,-,-,-             */
                cpu.HL.upper = cpu.HL.upper;
                cpu.clock += 4;
                break;
            case(0X65):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load L in to H             *
                    * FLAGS: -,-,-,-             */
                cpu.HL.upper = cpu.HL.lower;
                cpu.clock += 4;
                break;
            case(0X66):
                /* LD B, B - 1 byte, 8 cycles *
                    * Load [HL] in to H          *
                    * FLAGS: -,-,-,-             */
                nn = mem_read(cpu.HL.full);
                cpu.HL.upper = nn;
                cpu.clock += 8;
                break;
            case(0X67):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load A in to H             *
                    * FLAGS: -,-,-,-             */
                cpu.HL.upper = cpu.AF.upper;
                cpu.clock += 4;
                break;
            case(0X68):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load B in to L             *
                    * FLAGS: -,-,-,-             */
                cpu.HL.lower = cpu.BC.upper;
                cpu.clock += 4;
                break;
            case(0X69):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load C in to L             *
                    * FLAGS: -,-,-,-             */
                cpu.HL.lower = cpu.BC.lower;
                cpu.clock += 4;
                break;
            case(0X6A):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load D in to L             *
                    * FLAGS: -,-,-,-             */
                cpu.HL.lower = cpu.DE.upper;
                cpu.clock += 4;
                break;
            case(0X6B):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load E in to L             *
                    * FLAGS: -,-,-,-             */
                cpu.HL.lower = cpu.DE.lower;
                cpu.clock += 4;
                break;
            case(0X6C):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load H in to L             *
                    * FLAGS: -,-,-,-             */
                cpu.HL.lower = cpu.HL.upper;
                cpu.clock += 4;
                break;
            case(0X6D):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load L in to L             *
                    * FLAGS: -,-,-,-             */
                cpu.HL.lower = cpu.HL.lower;
                cpu.clock += 4;
                break;
            case(0X6E):
                /* LD B, B - 1 byte, 8 cycles *
                    * Load [HL] in to L          *
                    * FLAGS: -,-,-,-             */
                nn = mem_read(cpu.HL.full);
                cpu.HL.lower = nn;
                cpu.clock += 8;
                break;
            case(0X6F):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load A in to L             *
                    * FLAGS: -,-,-,-             */
                cpu.HL.lower = cpu.AF.upper;
                cpu.clock += 4;
                break;
            case(0X70):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load B into [HL]           *
                    * FLAGS: -,-,-,-             */
                mem_write(cpu.HL.full, cpu.BC.upper);
                cpu.clock += 8;
                break;
            case(0X71):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load C in to  [HL]         *
                    * FLAGS: -,-,-,-             */
                mem_write(cpu.HL.full, cpu.BC.lower);
                cpu.clock += 8;
                break;
            case(0X72):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load D in to [HL]          *
                    * FLAGS: -,-,-,-             */
                mem_write(cpu.HL.full, cpu.DE.upper);
                cpu.clock += 8;
                break;
            case(0X73):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load E in to [HL]          *
                    * FLAGS: -,-,-,-             */
                mem_write(cpu.HL.full, cpu.DE.lower);
                cpu.clock += 8;
                break;
            case(0X74):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load H in to [HL]          *
                    * FLAGS: -,-,-,-             */
                mem_write(cpu.HL.full, cpu.HL.upper);
                cpu.clock += 8;
                break;
            case(0X75):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load L in to [HL]          *
                    * FLAGS: -,-,-,-             */
                mem_write(cpu.HL.full, cpu.HL.lower);
                cpu.clock += 8;
                break;
            case(0X76):
                /* HLT - 1 byte, 4 cycles     *
                    * Halts program              *
                    * FLAGS: -,-,-,-             */
                cpu.clock += 4;
                break;
            case(0X77):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load A in to [HL]          *
                    * FLAGS: -,-,-,-             */
                mem_write(cpu.HL.full, cpu.AF.upper);
                cpu.clock += 4;
                break;
            case(0X78):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load B in to A             *
                    * FLAGS: -,-,-,-             */
                cpu.AF.upper = cpu.BC.upper;
                cpu.clock += 4;
                break;
            case(0X79):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load C in to A             *
                    * FLAGS: -,-,-,-             */
                cpu.AF.upper = cpu.BC.lower;
                cpu.clock += 4;
                break;
            case(0X7A):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load D in to A             *
                    * FLAGS: -,-,-,-             */
                cpu.AF.upper = cpu.DE.upper;
                cpu.clock += 4;
                break;
            case(0X7B):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load E in to A             *
                    * FLAGS: -,-,-,-             */
                cpu.AF.upper = cpu.DE.lower;
                cpu.clock += 4;
                break;
            case(0X7C):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load H in to A             *
                    * FLAGS: -,-,-,-             */
                cpu.AF.upper = cpu.HL.upper;
                cpu.clock += 4;
                break;
            case(0X7D):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load L in to A             *
                    * FLAGS: -,-,-,-             */
                cpu.AF.upper = cpu.HL.lower;
                cpu.clock += 4;
                break;
            case(0X7E):
                /* LD A, [HL] - 1 byte, 4 cycles *
                    * Load [HL] into A           *
                    * FLAGS: -,-,-,-             */
                nn = mem_read(cpu.HL.full);
                cpu.AF.upper = nn;
                cpu.clock += 8;
                break;
            case(0X7F):
                /* LD B, B - 1 byte, 4 cycles *
                    * Load A in to A             *
                    * FLAGS: -,-,-,-             */
                cpu.AF.upper = cpu.AF.upper;
                cpu.clock += 4;
                break;
            case(0X80):
                /* ADD A, B - 1 byte, 4 cycles *
                    * Add B to A                  *
                    * FLAGS: Z,0,H,C              */
                Z = (cpu.AF.upper + cpu.BC.upper == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, cpu.BC.upper, +);
                C = CARRY(cpu.AF.upper, cpu.BC.upper, +);

                cpu.AF.upper += cpu.BC.upper;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0X81):
                /* ADD A, C - 1 byte, 4 cycles *
                    * Add C to A                  *
                    * FLAGS: Z,0,H,C              */
                Z = (cpu.AF.upper + cpu.BC.lower == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, cpu.BC.lower, +);
                C = CARRY(cpu.AF.upper, cpu.BC.lower, +);

                cpu.AF.upper += cpu.BC.lower;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0X82):
                /* ADD A, D - 1 byte, 4 cycles *
                    * Add D to A                  *
                    * FLAGS: Z,0,H,C              */
                Z = (cpu.AF.upper + cpu.DE.upper == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, cpu.DE.upper, +);
                C = CARRY(cpu.AF.upper, cpu.DE.upper, +);

                cpu.AF.upper += cpu.DE.upper;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0X83):
                /* ADD A, E - 1 byte, 4 cycles *
                    * Add E to A                  *
                    * FLAGS: Z,0,H,C              */
                Z = (cpu.AF.upper + cpu.DE.lower == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, cpu.DE.lower, +);
                C = CARRY(cpu.AF.upper, cpu.DE.lower, +);

                cpu.AF.upper += cpu.DE.lower;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0X84):
                /* ADD A, H - 1 byte, 4 cycles *
                    * Add H to A                  *
                    * FLAGS: Z,0,H,C              */
                Z = (cpu.AF.upper + cpu.HL.upper == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, cpu.HL.upper, +);
                C = CARRY(cpu.AF.upper, cpu.HL.upper, +);

                cpu.AF.upper += cpu.HL.upper;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0X85):
                /* ADD A, L - 1 byte, 4 cycles *
                    * Add L to A                  *
                    * FLAGS: Z,0,H,C              */
                Z = (cpu.AF.upper + cpu.HL.lower == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, cpu.HL.lower, +);
                C = CARRY(cpu.AF.upper, cpu.HL.lower, +);

                cpu.AF.upper += cpu.HL.lower;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0X86):
                /* ADD A, [HL] - 1 byte, 8 cycles *
                    * Add [HL] to A                  *
                    * FLAGS: Z,0,H,C                 */
                nn = mem_read(cpu.HL.full);
                Z = (cpu.AF.upper + nn == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, nn, +);
                C = CARRY(cpu.AF.upper, nn, +);

                cpu.AF.upper += nn;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 8;
                break;
            case(0X87):
                /* ADD A, A - 1 byte, 4 cycles *
                    * Add A to A                  *
                    * FLAGS: Z,0,H,C              */
                Z = (cpu.AF.upper + cpu.AF.upper == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, cpu.AF.upper, +);
                C = CARRY(cpu.AF.upper, cpu.AF.upper, +);

                cpu.AF.upper += cpu.AF.upper;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0X88):
                /* ADC A, B - 1 byte, 4 cycles  *
                    * Add B and carry to A         *
                    * FLAGS: Z,0,H,C               */
                Z = (cpu.AF.upper + cpu.BC.upper == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, cpu.BC.upper + FC, +);
                C = CARRY(cpu.AF.upper, cpu.BC.upper + FC, +);

                cpu.AF.upper += cpu.BC.upper + FC;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0X89):
                /* ADC A, C - 1 byte, 4 cycles  *
                    * Add C and carry to A         *
                    * FLAGS: Z,0,H,C               */
                Z = (cpu.AF.upper + cpu.BC.lower == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, cpu.BC.lower + FC, +);
                C = CARRY(cpu.AF.upper, cpu.BC.lower + FC, +);

                cpu.AF.upper += cpu.BC.lower + FC;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0X8A):
                /* ADC A, D - 1 byte, 4 cycles  *
                    * Add D and carry to A         *
                    * FLAGS: Z,0,H,C               */
                Z = (cpu.AF.upper + cpu.DE.upper == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, cpu.DE.upper + FC, +);
                C = CARRY(cpu.AF.upper, cpu.DE.upper + FC, +);

                cpu.AF.upper += cpu.DE.upper + FC;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0X8B):
                /* ADC A, E - 1 byte, 4 cycles  *
                    * Add E and carry to A         *
                    * FLAGS: Z,0,H,C               */
                Z = (cpu.AF.upper + cpu.DE.lower == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, cpu.DE.lower + FC, +);
                C = CARRY(cpu.AF.upper, cpu.DE.lower + FC, +);

                cpu.AF.upper += cpu.DE.lower + FC;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0X8C):
                /* ADC A, H - 1 byte, 4 cycles  *
                    * Add H and carry to A         *
                    * FLAGS: Z,0,H,C               */
                Z = (cpu.AF.upper + cpu.HL.upper == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, cpu.HL.upper + FC, +);
                C = CARRY(cpu.AF.upper, cpu.HL.upper + FC, +);

                cpu.AF.upper += cpu.HL.upper + FC;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0X8D):
                /* ADC A, L - 1 byte, 4 cycles  *
                    * Add L and carry to A         *
                    * FLAGS: Z,0,H,C               */
                Z = (cpu.AF.upper + cpu.HL.lower == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, cpu.HL.lower + FC, +);
                C = CARRY(cpu.AF.upper, cpu.HL.lower + FC, +);

                cpu.AF.upper += cpu.HL.lower + FC;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0X8E):
                /* ADC A, [HL] - 1 byte, 4 cycles  *
                    * Add [HL] and carry to A         *
                    * FLAGS: Z,0,H,C               */
                nn = mem_read(cpu.HL.full);
                Z = (cpu.AF.upper + nn + FC == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, nn + FC, +);
                C =      CARRY(cpu.AF.upper, nn + FC, +);

                cpu.AF.upper += nn + FC;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 8;
                break;
            case(0X8F):
                /* ADC A, A - 1 byte, 4 cycles  *
                    * Add A and carry to A         *
                    * FLAGS: Z,0,H,C               */
                Z = (cpu.AF.upper + cpu.AF.upper == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, cpu.AF.upper + FC, +);
                C = CARRY(cpu.AF.upper, cpu.AF.upper + FC, +);

                cpu.AF.upper += cpu.AF.upper + FC;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0X90):
                /* SUB A, B - 1 byte, 4 cycles *
                    * Sub B to A                  *
                    * FLAGS: Z,0,H,C              */
                Z = (cpu.AF.upper - cpu.BC.upper == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, cpu.BC.upper, -);
                C = CARRY(cpu.AF.upper, cpu.BC.upper, -);

                cpu.AF.upper -= cpu.BC.upper;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0X91):
                /* SUB A, C - 1 byte, 4 cycles *
                    * Sub C to A                  *
                    * FLAGS: Z,0,H,C              */
                Z = (cpu.AF.upper - cpu.BC.lower == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, cpu.BC.lower, -);
                C = CARRY(cpu.AF.upper, cpu.BC.lower, -);

                cpu.AF.upper -= cpu.BC.lower;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0X92):
                /* SUB A, D - 1 byte, 4 cycles *
                    * Sub D to A                  *
                    * FLAGS: Z,0,H,C              */
                Z = (cpu.AF.upper - cpu.DE.upper == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, cpu.DE.upper, -);
                C = CARRY(cpu.AF.upper, cpu.DE.upper, -);

                cpu.AF.upper -= cpu.DE.upper;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0X93):
                /* SUB A, E - 1 byte, 4 cycles *
                    * Sub E to A                  *
                    * FLAGS: Z,0,H,C              */
                Z = (cpu.AF.upper - cpu.DE.lower == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, cpu.DE.lower, -);
                C = CARRY(cpu.AF.upper, cpu.DE.lower, -);

                cpu.AF.upper -= cpu.DE.lower;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0X94):
                /* SUB A, H - 1 byte, 4 cycles *
                    * Sub H to A                  *
                    * FLAGS: Z,0,H,C              */
                Z = (cpu.AF.upper - cpu.HL.upper == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, cpu.HL.upper, -);
                C = CARRY(cpu.AF.upper, cpu.HL.upper, -);

                cpu.AF.upper -= cpu.HL.upper;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0X95):
                /* SUB A, L - 1 byte, 4 cycles *
                    * Sub L to A                  *
                    * FLAGS: Z,0,H,C              */
                Z = (cpu.AF.upper - cpu.HL.lower == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, cpu.HL.lower, -);
                C = CARRY(cpu.AF.upper, cpu.HL.lower, -);

                cpu.AF.upper -= cpu.HL.lower;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0X96):
                /* SUB A, [HL] - 1 byte, 4 cycles *
                    * Sub [HL] to A                  *
                    * FLAGS: Z,0,H,C                 */
                nn = mem_read(cpu.HL.full);
                Z = (cpu.AF.upper - nn == 0);
                N = 1;
                H = HALF_CARRY(cpu.AF.upper, nn, -);
                C = CARRY(cpu.AF.upper, nn, -);

                cpu.AF.upper -= nn;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 8;
                break;
            case(0X97):
                /* SUB A, A - 1 byte, 4 cycles *
                    * Sub A to A                  *
                    * FLAGS: Z,0,H,C              */
                Z = (cpu.AF.upper - cpu.BC.upper == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, cpu.AF.upper, -);
                C = CARRY(cpu.AF.upper, cpu.AF.upper, -);

                cpu.AF.upper -= cpu.AF.upper;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0X98):
                /* SBC A, B - 1 byte, 4 cycles  *
                    * Sub B and carry to A         *
                    * FLAGS: Z,0,H,C               */
                Z = (cpu.AF.upper - cpu.BC.upper == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, cpu.BC.upper - FC, -);
                C = CARRY(cpu.AF.upper, cpu.BC.upper - FC, -);

                cpu.AF.upper -= cpu.BC.upper + FC;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0X99):
                /* SBC A, C - 1 byte, 4 cycles  *
                    * Sub C and carry to A         *
                    * FLAGS: Z,0,H,C               */
                Z = (cpu.AF.upper - cpu.BC.lower == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, cpu.BC.lower - FC, -);
                C = CARRY(cpu.AF.upper, cpu.BC.lower - FC, -);

                cpu.AF.upper -= cpu.BC.lower + FC;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0X9A):
                /* SBC A, D - 1 byte, 4 cycles  *
                    * Sub D and carry to A         *
                    * FLAGS: Z,0,H,C               */
                Z = (cpu.AF.upper - cpu.DE.upper == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, cpu.DE.upper - FC, -);
                C = CARRY(cpu.AF.upper, cpu.DE.upper - FC, -);

                cpu.AF.upper -= cpu.DE.upper + FC;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0X9B):
                /* SBC A, E - 1 byte, 4 cycles  *
                    * Sub E and carry to A         *
                    * FLAGS: Z,0,H,C               */
                Z = (cpu.AF.upper - cpu.DE.lower == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, cpu.DE.lower - FC, -);
                C = CARRY(cpu.AF.upper, cpu.DE.lower - FC, -);

                cpu.AF.upper -= cpu.DE.lower + FC;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0X9C):
                /* SBC A, H - 1 byte, 4 cycles  *
                    * Sub H and carry to A         *
                    * FLAGS: Z,0,H,C               */
                Z = (cpu.AF.upper - cpu.HL.upper == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, cpu.HL.upper - FC, -);
                C = CARRY(cpu.AF.upper, cpu.HL.upper - FC, -);

                cpu.AF.upper -= cpu.HL.upper + FC;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0X9D):
                /* SBC A, L - 1 byte, 4 cycles  *
                    * Sub L and carry to A         *
                    * FLAGS: Z,0,H,C               */
                Z = (cpu.AF.upper - cpu.HL.lower == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, cpu.HL.lower - FC, -);
                C = CARRY(cpu.AF.upper, cpu.HL.lower - FC, -);

                cpu.AF.upper -= cpu.HL.lower + FC;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0X9E):
                /* SBC A, [HL] - 1 byte, 4 cycles  *
                    * Sub [HL] and carry to A         *
                    * FLAGS: Z,0,H,C               */
                nn = mem_read(cpu.HL.full);
                Z = (cpu.AF.upper - nn - FC == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, nn - FC, -);
                C =      CARRY(cpu.AF.upper, nn - FC, -);

                cpu.AF.upper -= nn + FC;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 8;
                break;
            case(0X9F):
                /* SBC A, A - 1 byte, 4 cycles  *
                    * Sub A and carry to A         *
                    * FLAGS: Z,0,H,C               */
                Z = (cpu.AF.upper - cpu.AF.upper == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, cpu.AF.upper - FC, -);
                C = CARRY(cpu.AF.upper, cpu.AF.upper - FC, -);

                cpu.AF.upper -= cpu.AF.upper + FC;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0XA0):
                /* AND A, B - 1 byte, 4 cycles *
                    * And A with B                *
                    * FLAGS: Z,0,1,0              */
                Z = ((cpu.AF.upper & cpu.BC.upper) == 0);
                N = 0;
                H = 1;
                C = 0;

                cpu.AF.upper &= cpu.BC.upper;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0XA1):
                /* AND A, C - 1 byte, 4 cycles *
                    * And A with C                *
                    * FLAGS: Z,0,1,0              */
                Z = ((cpu.AF.upper & cpu.BC.lower) == 0);
                N = 0;
                H = 1;
                C = 0;

                cpu.AF.upper &= cpu.BC.lower;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0XA2):
                /* AND A, D - 1 byte, 4 cycles *
                    * And A with D                *
                    * FLAGS: Z,0,1,0              */
                Z = ((cpu.AF.upper & cpu.DE.upper) == 0);
                N = 0;
                H = 1;
                C = 0;

                cpu.AF.upper &= cpu.DE.upper;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0XA3):
                /* AND A, E - 1 byte, 4 cycles *
                    * And A with E                *
                    * FLAGS: Z,0,1,0              */
                Z = ((cpu.AF.upper & cpu.DE.lower) == 0);
                N = 0;
                H = 1;
                C = 0;

                cpu.AF.upper &= cpu.DE.lower;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0XA4):
                /* AND A, H - 1 byte, 4 cycles *
                    * And A with H                *
                    * FLAGS: Z,0,1,0              */
                Z = ((cpu.AF.upper & cpu.HL.upper) == 0);
                N = 0;
                H = 1;
                C = 0;

                cpu.AF.upper &= cpu.HL.upper;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0XA5):
                /* AND A, L - 1 byte, 4 cycles *
                    * And A with L                *
                    * FLAGS: Z,0,1,0              */
                Z = ((cpu.AF.upper & cpu.HL.lower) == 0);
                N = 0;
                H = 1;
                C = 0;

                cpu.AF.upper &= cpu.HL.lower;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0XA6):
                /* AND A, [HL] - 1 byte, 4 cycles *
                    * And A with [HL]                *
                    * FLAGS: Z,0,1,0                 */
                nn = mem_read(cpu.HL.full);
                Z = ((cpu.AF.upper & nn) == 0);
                N = 0;
                H = 1;
                C = 0;

                cpu.AF.upper &= nn;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 8;
                break;
            case(0XA7):
                /* AND A, A - 1 byte, 4 cycles *
                    * And A with A                *
                    * FLAGS: Z,0,1,0              */
                Z = ((cpu.AF.upper & cpu.AF.upper) == 0);
                N = 0;
                H = 1;
                C = 0;

                cpu.AF.upper &= cpu.AF.upper;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0XA8):
                /* XOR A, B - 1 byte, 4 cycles *
                    * XOR A with B                *
                    * FLAGS: Z,0,1,0              */
                Z = ((cpu.AF.upper ^ cpu.BC.upper) == 0);
                N = 0;
                H = 0;
                C = 0;

                cpu.AF.upper ^= cpu.BC.upper;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0XA9):
                /* XOR A, C - 1 byte, 4 cycles *
                    * XOR A with C                *
                    * FLAGS: Z,0,1,0              */
                Z = ((cpu.AF.upper ^ cpu.BC.lower) == 0);
                N = 0;
                H = 0;
                C = 0;

                cpu.AF.upper ^= cpu.BC.lower;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0XAA):
                /* XOR A, D - 1 byte, 4 cycles *
                    * XOR A with D                *
                    * FLAGS: Z,0,1,0              */
                Z = ((cpu.AF.upper ^ cpu.DE.upper) == 0);
                N = 0;
                H = 0;
                C = 0;

                cpu.AF.upper ^= cpu.DE.upper;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0XAB):
                /* XOR A, E - 1 byte, 4 cycles *
                    * XOR A with E                *
                    * FLAGS: Z,0,1,0              */
                Z = ((cpu.AF.upper ^ cpu.DE.lower) == 0);
                N = 0;
                H = 0;
                C = 0;

                cpu.AF.upper ^= cpu.DE.lower;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0XAC):
                /* XOR A, H - 1 byte, 4 cycles *
                    * XOR A with H                *
                    * FLAGS: Z,0,1,0              */
                Z = ((cpu.AF.upper ^ cpu.HL.upper) == 0);
                N = 0;
                H = 0;
                C = 0;

                cpu.AF.upper ^= cpu.HL.upper;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0XAD):
                /* XOR A, L - 1 byte, 4 cycles *
                    * XOR A with L                *
                    * FLAGS: Z,0,1,0              */
                Z = ((cpu.AF.upper ^ cpu.HL.lower) == 0);
                N = 0;
                H = 0;
                C = 0;

                cpu.AF.upper ^= cpu.HL.lower;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0XAE):
                /* XOR A, [HL] - 1 byte, 4 cycles *
                    * XOR A with [HL]                *
                    * FLAGS: Z,0,1,0                 */
                nn = mem_read(cpu.HL.full);
                Z = ((cpu.AF.upper ^ nn) == 0);
                N = 0;
                H = 0;
                C = 0;

                cpu.AF.upper ^= nn;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 8;
                break;
            case(0XAF):
                /* XOR A, A - 1 byte, 4 cycles *
                * XOR A with A                *
                * FLAGS: Z,0,1,0              */
                Z = ((cpu.AF.upper ^ cpu.AF.upper) == 0);
                N = 0;
                H = 0;
                C = 0;

                cpu.AF.upper ^= cpu.AF.upper;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0XB0):
                /* OR A, B - 1 byte, 4 cycles *
                    * Or A with B                *
                    * FLAGS: Z,0,1,0              */
                Z = ((cpu.AF.upper | cpu.BC.upper) == 0);
                N = 0;
                H = 0;
                C = 0;

                cpu.AF.upper |= cpu.BC.upper;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0XB1):
                /* OR A, C - 1 byte, 4 cycles *
                    * Or A with C                *
                    * FLAGS: Z,0,1,0              */
                Z = ((cpu.AF.upper | cpu.BC.lower) == 0);
                N = 0;
                H = 0;
                C = 0;

                cpu.AF.upper |= cpu.BC.lower;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0XB2):
                /* OR A, D - 1 byte, 4 cycles *
                    * Or A with D                *
                    * FLAGS: Z,0,1,0              */
                Z = ((cpu.AF.upper | cpu.DE.upper) == 0);
                N = 0;
                H = 0;
                C = 0;

                cpu.AF.upper |= cpu.DE.upper;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0XB3):
                /* OR A, E - 1 byte, 4 cycles *
                    * Or A with E                *
                    * FLAGS: Z,0,1,0              */
                Z = ((cpu.AF.upper | cpu.DE.lower) == 0);
                N = 0;
                H = 0;
                C = 0;

                cpu.AF.upper |= cpu.DE.lower;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0XB4):
                /* OR A, H - 1 byte, 4 cycles *
                    * Or A with H                *
                    * FLAGS: Z,0,1,0              */
                Z = ((cpu.AF.upper | cpu.HL.upper) == 0);
                N = 0;
                H = 0;
                C = 0;

                cpu.AF.upper |= cpu.HL.upper;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0XB5):
                /* OR A, L - 1 byte, 4 cycles *
                    * Or A with L                *
                    * FLAGS: Z,0,1,0              */
                Z = ((cpu.AF.upper | cpu.HL.lower) == 0);
                N = 0;
                H = 0;
                C = 0;

                cpu.AF.upper |= cpu.HL.lower;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0XB6):
                /* OR A, [HL] - 1 byte, 4 cycles *
                    * Or A with [HL]                *
                    * FLAGS: Z,0,1,0                 */
                nn = mem_read(cpu.HL.full);
                Z = ((cpu.AF.upper | nn) == 0);
                N = 0;
                H = 0;
                C = 0;

                cpu.AF.upper |= nn;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 8;
                break;
            case(0XB7):
                /* OR A, A - 1 byte, 4 cycles *
                    * Or A with A                *
                    * FLAGS: Z,0,1,0              */
                Z = ((cpu.AF.upper | cpu.AF.upper) == 0);
                N = 0;
                H = 0;
                C = 0;

                cpu.AF.upper |= cpu.AF.upper;
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0XB8):
                /* CP B - 1 byte, 4 cycles *
                    * Compare B to A                  *
                    * FLAGS: Z,1,H,C              */
                Z = (cpu.AF.upper - cpu.BC.upper == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, cpu.BC.upper, -);
                C = CARRY(cpu.AF.upper, cpu.BC.upper, -);

                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0XB9):
                /* CP C - 1 byte, 4 cycles *
                    * Compare C to A                  *
                    * FLAGS: Z,1,H,C              */
                Z = (cpu.AF.upper - cpu.BC.lower == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, cpu.BC.lower, -);
                C = CARRY(cpu.AF.upper, cpu.BC.lower, -);

                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0XBA):
                /* CP D - 1 byte, 4 cycles *
                    * Compare D to A                  *
                    * FLAGS: Z,1,H,C              */
                Z = (cpu.AF.upper - cpu.DE.upper == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, cpu.DE.upper, -);
                C = CARRY(cpu.AF.upper, cpu.DE.upper, -);

                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0XBB):
                /* CP E - 1 byte, 4 cycles *
                    * Compare E to A                  *
                    * FLAGS: Z,1,H,C              */
                Z = (cpu.AF.upper - cpu.DE.lower == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, cpu.DE.lower, -);
                C = CARRY(cpu.AF.upper, cpu.DE.lower, -);

                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0XBC):
                /* CP H - 1 byte, 4 cycles *
                    * Compare H to A                  *
                    * FLAGS: Z,1,H,C              */
                Z = (cpu.AF.upper - cpu.HL.upper == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, cpu.HL.upper, -);
                C = CARRY(cpu.AF.upper, cpu.HL.upper, -);

                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0XBD):
                /* CP L - 1 byte, 4 cycles *
                    * Compare L to A                  *
                    * FLAGS: Z,1,H,C              */
                Z = (cpu.AF.upper - cpu.HL.lower == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, cpu.HL.lower, -);
                C = CARRY(cpu.AF.upper, cpu.HL.lower, -);

                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0XBE):
                /* CP [HL] - 1 byte, 8 cycles *
                    * Compare [HL] to A                  *
                    * FLAGS: Z,1,H,C                 */
                nn = mem_read(cpu.HL.full);
                Z = (cpu.AF.upper - nn == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, nn, -);
                C = CARRY(cpu.AF.upper, nn, -);

                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 8;
                break;
            case(0XBF):
                /* CP A - 1 byte, 4 cycles *
                    * Compare A to A                  *
                    * FLAGS: Z,1,H,C              */
                Z = (cpu.AF.upper - cpu.AF.upper == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, cpu.AF.upper, -);
                C = CARRY(cpu.AF.upper, cpu.AF.upper, -);

                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 4;
                break;
            case(0XC0):
                /* RET NZ - 1 byte, 16 cycles  *
                    * if not Z Return from subroutine   *
                    * FLAGS: -,-,-,-           */

                if (!FZ) {
                    cpu.PC = mem_read(cpu.SP++) << 8;
                    cpu.PC |= mem_read(cpu.SP++);
                    cpu.clock += 12;
                }
                cpu.clock += 8;
                break;
            case(0XC1):
                /* POP BC - 1 byte, 12 cycles *
                    * Pop from stack into BC     *
                    * FLAGS: -,-,-,-             */
                cpu.BC.full = mem_read(cpu.SP++) << 8;
                cpu.BC.full |= mem_read(cpu.SP++);
                cpu.clock += 12;
                break;
            case(0XC2):
                /* JP NZ, a16 - 3 bytes, 16 cycles *
                * if not Z Set PC to a16 (JMP)         *
                * FLAGS: -,-,-,-              */
                nn = mem_read(cpu.PC++);
                nn |= mem_read(cpu.PC++) << 8;

                if (!FZ) {
                    cpu.PC = nn;
                    cpu.clock += 4;
                }
                cpu.clock += 12;
                break;
            case(0XC3):
                /* JP a16 - 3 bytes, 16 cycles *
                * Set PC to a16 (JMP)         *
                * FLAGS: -,-,-,-              */
                nn = mem_read(cpu.PC++);
                nn |= mem_read(cpu.PC++) << 8;

                cpu.PC = nn;
                cpu.clock += 16;
                break;
            case(0XC4):
                /* CALL NZ, a16 - 3 bytes, 24/12 cycles       *
                * if not Z (Store PC at [SP], Set PC to a16) *
                * FLAGS: -,-,-,-                             */
                nn = mem_read(cpu.PC++);
                nn |= mem_read(cpu.PC++) << 8;

                if (!FZ) {
                    mem_write(--cpu.SP, cpu.PC);
                    mem_write(--cpu.SP, cpu.PC >> 8);

                    cpu.PC = nn;
                    cpu.clock += 12;
                }
                cpu.clock += 12;
                break;
            case(0XC5):
                /* PUSH BC - 1 byte, 16 cycles *
                    * Push reg BC to stack        *
                    * FLAGS: -,-,-,-              */
                mem_write(--cpu.SP, cpu.BC.lower);
                mem_write(--cpu.SP, cpu.BC.upper);
                cpu.clock += 16;
                break;
            case(0XC6):
                /* ADD A, d8 - 2 byte, 8 cycles *
                    * Add d8 to A                  *
                    * FLAGS: Z,0,H,C               */
                cpu.PC += 1;

                nn = mem_read(cpu.PC);
                Z = (cpu.AF.upper + nn == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, nn, +);
                C = CARRY(cpu.AF.upper, nn, +);

                cpu.AF.upper += nn;
                flag_write(Z, N, H, C, 0b0000);

                cpu.clock += 8;
                break;
            case(0XC7):
                /* RST 38H - 1 byte, 16 cycles *
                    * call abs addr at opcode     *
                    * FLAGS: -,-,-,-              */
                mem_write(--cpu.SP, cpu.PC);
                mem_write(--cpu.SP, cpu.PC >> 8);

                cpu.PC = cpu.CIR & 0XFF;
                cpu.clock += 16;
                break;
            case(0XC8):
                /* RET Z - 1 byte, 16 cycles  *
                    * if  Z Return from subroutine   *
                    * FLAGS: -,-,-,-           */
                if (FZ) {
                    cpu.PC = mem_read(cpu.SP++) << 8;
                    cpu.PC |= mem_read(cpu.SP++);
                    cpu.clock += 12;
                }
                cpu.clock += 8;
                break;
            case(0XC9):
                /* RET - 1 byte, 16 cycles  *
                    * Return from subroutine   *
                    * FLAGS: -,-,-,-           */
                cpu.PC = mem_read(cpu.SP++) << 8;
                cpu.PC |= mem_read(cpu.SP++);
                cpu.clock += 16;
                break;
            case(0XCA):
                /* JP Z, a16 - 3 bytes, 16 cycles *
                    * if Z Set PC to a16 (JMP)         *
                    * FLAGS: -,-,-,-              */
                nn = mem_read(cpu.PC++);
                nn |= mem_read(cpu.PC++) << 8;

                if (FZ) {
                    cpu.PC = nn;
                    cpu.clock += 4;
                }
                cpu.clock += 12;
                break;
            case(0XCB):
                /* Prefixed op */
                cpu.prefix = true;
                cpu.clock += 4;
                break;
            case(0XCC):
                /* CALL Z, a16 - 3 bytes, 24/12 cycles    *
                    * if Z (Store PC at [SP], Set PC to a16) *
                    * FLAGS: -,-,-,-                         */
                nn = mem_read(cpu.PC++);
                nn |= mem_read(cpu.PC++) << 8;

                if (FZ) {
                    mem_write(--cpu.SP, cpu.PC);
                    mem_write(--cpu.SP, cpu.PC >> 8);

                    cpu.PC = nn;
                    cpu.clock += 12;
                }
                cpu.clock += 12;
                break;
            case(0XCD):
                /* CALL a16 - 3 bytes, 24 cycles *
                * Store PC at [SP], Set PC to a16  *
                * FLAGS: -,-,-,-                   */
                nn = mem_read(cpu.PC++);
                nn |= mem_read(cpu.PC++) << 8;

                mem_write(--cpu.SP, cpu.PC);
                mem_write(--cpu.SP, cpu.PC >> 8);

                cpu.PC = nn;
                cpu.clock += 24;
                break;
            case(0XCE):
                /* ADC A, d8 - 1 byte, 8 cycles  *
                * Add d8 and carry to A         *
                * FLAGS: Z,0,H,C                */
                nn = mem_read(cpu.PC++);
                Z = (cpu.AF.upper + nn == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, nn + ((cpu.AF.lower & 0X10) == 0X10), +);
                C = CARRY(cpu.AF.upper, nn + ((cpu.AF.lower & 0X10) == 0X10), +);

                cpu.AF.upper += nn + ((cpu.AF.lower & 0X10) == 0X10);
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 8;
                break;
            case(0XCF):
                /* RST 08H - 1 byte, 16 cycles *
                    * call abs addr at opcode     *
                    * FLAGS: -,-,-,-              */
                mem_write(--cpu.SP, cpu.PC);
                mem_write(--cpu.SP, cpu.PC >> 8);

                cpu.PC = cpu.CIR & 0XFF;
                cpu.clock += 16;
                break;
            case(0XD0):
                /* RET NC - 1 byte, 16 cycles  *
                    * if not C Return from subroutine   *
                    * FLAGS: -,-,-,-           */
                if (!FC) {
                    mem_write(--cpu.SP, cpu.PC);
                    mem_write(--cpu.SP, cpu.PC >> 8);
                    cpu.clock += 12;
                }
                cpu.clock += 8;
                break;
            case(0XD1):
                /* POP DE - 1 byte, 12 cycles *
                    * Pop from stack into DE     *
                    * FLAGS: -,-,-,-             */
                cpu.DE.upper = mem_read(cpu.SP++);
                cpu.DE.lower = mem_read(cpu.SP++);

                cpu.clock += 12;
                break;
            case(0XD2):
                /* JP NC, a16 - 3 bytes, 16 cycles *
                    * if not C Set PC to a16 (JMP)         *
                    * FLAGS: -,-,-,-              */
                nn = mem_read(cpu.PC++);
                nn |= mem_read(cpu.PC++) << 8;

                if (!FC) {
                    cpu.PC = nn;
                    cpu.clock += 4;
                }

                cpu.clock += 12;
                break;
            case(0XD3): break;
            case(0XD4):
                /* CALL NC, a16 - 3 bytes, 24/12 cycles       *
                    * if not C (Store PC at [SP], Set PC to a16) *
                    * FLAGS: -,-,-,-                             */
                nn = mem_read(cpu.PC++);
                nn |= mem_read(cpu.PC++) << 8;

                if (!FC) {
                    mem_write(--cpu.SP, cpu.PC);
                    mem_write(--cpu.SP, cpu.PC >> 8);

                    cpu.PC = nn;
                    cpu.clock += 12;
                }
                cpu.clock += 12;
                break;
            case(0XD5):
                /* PUSH DE - 1 byte, 16 cycles *
                    * Push reg DE to stack        *
                    * FLAGS: -,-,-,-              */
                mem_write(--cpu.SP, cpu.DE.lower);
                mem_write(--cpu.SP, cpu.DE.upper);
                cpu.clock += 16;
                break;
            case(0XD6):
                /* SUB A, d8 - 2 byte, 8 cycles *
                    * Sub d8 to A                  *
                    * FLAGS: Z,0,H,C               */
                nn = mem_read(cpu.PC++);
                Z = (cpu.AF.upper - nn == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, nn, -);
                C = CARRY(cpu.AF.upper, nn, -);

                cpu.AF.upper -= nn;
                flag_write(Z, N, H, C, 0b0000);

                cpu.clock += 8;
                break;
            case(0XD7):
                /* RST 38H - 1 byte, 16 cycles *
                    * call abs addr at opcode     *
                    * FLAGS: -,-,-,-              */
                mem_write(--cpu.SP, cpu.PC);
                mem_write(--cpu.SP, cpu.PC >> 8);

                cpu.PC = cpu.CIR & 0XFF;
                cpu.clock += 16;
                break;
            case(0XD8):
                /* RET C - 1 byte, 16 cycles  *
                    * if  C Return from subroutine   *
                    * FLAGS: -,-,-,-           */
                if (FC) {
                    cpu.PC = mem_read(cpu.SP++) << 8;
                    cpu.PC |= mem_read(cpu.SP++);
                }
                break;
            case(0XD9):
                /* RETI - 1 byte, 16 cycles   *
                    * Return and enable interupt *
                    * FLAGS: -,-,-,-             */
                cpu.PC = mem_read(cpu.SP++) << 8;
                cpu.PC |= mem_read(cpu.SP++);
                cpu.IME = true;
                cpu.clock += 16;
                break;
            case(0XDA):
                /* JP C, a16 - 3 bytes, 16 cycles *
                    * if C Set PC to a16 (JMP)         *
                    * FLAGS: -,-,-,-              */
                nn = mem_read(cpu.PC++);
                nn |= mem_read(cpu.PC++) << 8;

                if (FC) {
                    cpu.PC = nn;
                    cpu.clock += 4;
                }
                cpu.clock += 12;
                break;
            case(0XDB): break;
            case(0XDC):
                /* CALL C, a16 - 3 bytes, 24/12 cycles    *
                    * if C (Store PC at [SP], Set PC to a16) *
                    * FLAGS: -,-,-,-                         */
                nn = mem_read(cpu.PC++);
                nn |= mem_read(cpu.PC++) << 8;

                if (FC) {
                    mem_write(--cpu.SP, cpu.PC);
                    mem_write(--cpu.SP, cpu.PC >> 8);

                    cpu.PC = nn;
                    cpu.clock += 12;
                }
                cpu.clock += 12;
                break;
            case (0XDD): break;
            case(0XDE):
                /* SBC A, d8 - 1 byte, 4 cycles  *
                    * Sub d8 and carry to A         *
                    * FLAGS: Z,0,H,C                */
                nn = mem_read(cpu.PC++);
                Z = (cpu.AF.upper - nn == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, nn - ((cpu.AF.lower & 0X10) == 0X10), -);
                C = CARRY(cpu.AF.upper, nn - ((cpu.AF.lower & 0X10) == 0X10), -);

                cpu.AF.upper -= nn - ((cpu.AF.lower & 0X10) == 0X10);
                flag_write(Z, N, H, C, 0b0000);
                cpu.clock += 8;
                break;
            case(0XDF):
                /* RST 18H - 1 byte, 16 cycles *
                    * call abs addr at opcode     *
                    * FLAGS: -,-,-,-              */
                mem_write(--cpu.SP, cpu.PC);
                mem_write(--cpu.SP, cpu.PC >> 8);

                cpu.PC = cpu.CIR & 0XFF;
                cpu.clock += 16;
                break;
            case(0XE0):
                /* LDH [a8], A - 2 bytes, 12 cycles *
                * Write to I/O-port a8             *
                * FLAGS: -,-,-,-                   */
                nn = mem_read(cpu.PC++);
                mem_write(0XFF00 + nn, cpu.AF.upper);

                cpu.clock += 12;
                break;
            case(0XE1):
                /* POP HL - 1 byte, 12 cycles *
                    * Pop from stack into HL     *
                    * FLAGS: -,-,-,-             */
                cpu.HL.upper = mem_read(cpu.SP++);
                cpu.HL.lower = mem_read(cpu.SP++);
                cpu.clock += 12;
                break;
            case(0XE2):
                /* LD A, C - 2 bytes, 12 cycles *
                    * Read from to I/O-port C2      *
                    * FLAGS: -,-,-,-                */
                mem_write(0XFF00 + cpu.BC.lower, cpu.AF.lower);
                cpu.clock += 8;
                break;
            case(0XE3): break;
            case(0XE4): break;
            case(0XE5):
                /* PUSH HL - 1 byte, 16 cycles *
                    * Push reg HL to stack        *
                    * FLAGS: -,-,-,-              */
                cpu.HL.upper = mem_read(--cpu.SP);
                cpu.HL.lower = mem_read(--cpu.SP);
                cpu.clock += 16;
                break;
            case(0XE6):
                /* AND A, d8 - 2 byte, 8 cycles *
                    * And A with d8                *
                    * FLAGS: Z,0,1,0               */
                cpu.PC += 1;

                nn = mem_read(cpu.PC);
                Z = ((cpu.AF.upper & cpu.BC.upper) == 0);
                N = 0;
                H = 1;
                C = 0;

                cpu.AF.upper &= cpu.BC.upper;
                flag_write(Z, N, H, C, 0b0000);

                cpu.clock += 8;
                break;
            case(0XE7):
                /* RST 38H - 1 byte, 16 cycles *
                    * call abs addr at opcode     *
                    * FLAGS: -,-,-,-              */
                mem_write(--cpu.SP, cpu.PC);
                mem_write(--cpu.SP, cpu.PC >> 8);

                cpu.PC = cpu.CIR & 0XFF;
                cpu.clock += 16;
                break;
            case(0XE8):
                /* ADD SP, r8 - 2 bytes, 16 cycles *
                    * Add 8 bit signed to SP          *
                    * FLAGS: -,0,H,C                  */
                cpu.PC += 1;

                nn = mem_read(cpu.PC);
                Z = 0;
                N = 0;
                H = HALF_CARRY(cpu.SP, (int8_t) nn, +);
                C = CARRY(cpu.SP, (int8_t) nn, +);

                cpu.SP += (int8_t) nn;
                flag_write(Z, N, H, C, 0b1000);
                cpu.clock += 16;
                break;
            case(0XE9):
                /* JP [HL] - 3 bytes, 16 cycles *
                    * Set PC to [HL] (JMP)         *
                    * FLAGS: -,-,-,-               */
                nn = mem_read(cpu.HL.full);
                cpu.PC = nn;
                cpu.clock += 4;
                break;
            case(0XEA):
                /* LD [a16], A - 3 bytes, 16 cycles *
                * Load A into [a16]                *
                * FLAGS -,-,-,-                    */
                nn = mem_read(cpu.PC++);
                nn |= mem_read(cpu.PC++) << 8;

                mem_write(nn, cpu.AF.upper);

                cpu.clock += 16;
                break;
            case(0XEB): break;
            case(0XEC): break;
            case(0XED): break;
            case(0XEE):
                /* XOR A, d8 - 2 byte, 8 cycles  *
                    * XOR A with d8                 *
                    * FLAGS: Z,0,1,0                */
                cpu.PC += 1;

                nn = mem_read(cpu.PC);
                Z = ((cpu.AF.upper ^ cpu.BC.upper) == 0);
                N = 0;
                H = 0;
                C = 0;

                cpu.AF.upper ^= cpu.BC.upper;
                flag_write(Z, N, H, C, 0b0000);

                cpu.clock += 8;
                break;
            case(0XEF):
                /* RST 28H - 1 byte, 16 cycles *
                    * call abs addr at opcode     *
                    * FLAGS: -,-,-,-              */
                mem_write(--cpu.SP, cpu.PC);
                mem_write(--cpu.SP, cpu.PC >> 8);

                cpu.PC = cpu.CIR & 0XFF;
                cpu.clock += 16;
                break;
            case(0XF0):
                /* LDH A, [a8] - 2 bytes, 12 cycles *
                    * Read from to I/O-port a8         *
                    * FLAGS: -,-,-,-                   */
                cpu.PC += 1;

                nn = mem_read(cpu.PC);
                cpu.AF.upper = mem_read(0XFF00 + nn);

                cpu.clock += 12;
                break;
            case(0XF1):
                /* POP AF - 1 byte, 12 cycles *
                    * Pop from stack into AF     *
                    * FLAGS: -,-,-,-             */
                cpu.AF.full = mem_read(cpu.SP++) << 8;
                cpu.AF.full |= mem_read(cpu.SP++);
                cpu.clock += 12;
                break;
            case(0XF2):
                /* LDH A, C - 2 bytes, 12 cycles *
                * Read from to I/O-port C2      *
                * FLAGS: -,-,-,-                */
                cpu.AF.upper = mem_read(0XFF00 + cpu.BC.lower);
                cpu.clock += 8;
                break;
            case(0XF3):
                /* DI - 1 byte, 4 cycles *
                * Disable interupts     *
                * FLAGS: -,-,-,-        */
                cpu.IME = false;
                cpu.clock += 4;
                break;
            case(0XF4): break;
            case(0XF5):
                /* PUSH AF - 1 byte, 16 cycles *
                    * Push reg AF to stack        *
                    * FLAGS: -,-,-,-              */
                cpu.AF.full = mem_read(--cpu.SP) << 8;
                cpu.AF.full |= mem_read(--cpu.SP);
                cpu.clock += 16;
                break;
            case(0XF6):
                /* OR A, d8 - 2 byte, 8 cycles  *
                    * OR A with d8                 *
                    * FLAGS: Z,0,1,0               */
                cpu.PC += 1;

                nn = mem_read(cpu.PC);
                Z = ((cpu.AF.upper | cpu.BC.upper) == 0);
                N = 0;
                H = 0;
                C = 0;

                cpu.AF.upper |= cpu.BC.upper;
                flag_write(Z, N, H, C, 0b0000);

                cpu.clock += 8;
                break;
            case(0XF7):
                /* RST 38H - 1 byte, 16 cycles *
                    * call abs addr at opcode     *
                    * FLAGS: -,-,-,-              */
                mem_write(--cpu.SP, cpu.PC);
                mem_write(--cpu.SP, cpu.PC >> 8);

                cpu.PC = cpu.CIR & 0XFF;
                cpu.clock += 16;
                break;
            case(0XF8):
                /* LD HL, SP + r8 - 2 bytes, 16 cycles *
                    * load 8 bit signed + SP to HL        *
                    * FLAGS: 0,0,H,C                      */
                cpu.PC += 1;

                nn = mem_read(cpu.PC);
                Z = 0;
                N = 0;
                H = HALF_CARRY(cpu.SP, (int8_t) nn, +);
                C = CARRY(cpu.SP, (int8_t) nn, +);

                cpu.HL.full = cpu.SP + (int8_t) nn;
                flag_write(Z, N, H, C, 0b0000);

                cpu.clock += 12;
                break;
            case(0XF9):
                /* LD SP, HL - 1 byte, 8 cycles *
                    * Load HL into SP              *
                    * FLAGS: -,-,-,-               */
                cpu.SP = cpu.HL.full;
                cpu.clock += 8;
                break;
            case(0XFA):
                /* LD A, [a16] - 3 bytes, 16 cycles *
                    * Load [a16] into A                *
                    * FLAGS -,-,-,-                    */
                nn = mem_read(cpu.PC++);
                nn |= mem_read(cpu.PC++) << 8;

                cpu.AF.upper = mem_read(nn);

                cpu.clock += 16;
                break;
            case(0XFB):
                /* EI - 1 byte, 4 cycles *
                    * Enable interupts      *
                    * FLAGS: -,-,-,-        */
                cpu.IME = true;
                cpu.clock += 4;
                break;
            case(0XFC): break;
            case(0XFD): break;
            case(0XFE):
                /* CP d8 - 2 byte, 8 cycles *
                    * Compare A to d8          *
                    * FLAGS: Z,1,H,C           */
                cpu.PC += 1;

                nn = mem_read(cpu.PC);
                Z = (cpu.AF.upper - cpu.AF.upper == 0);
                N = 0;
                H = HALF_CARRY(cpu.AF.upper, cpu.AF.upper, -);
                C = CARRY(cpu.AF.upper, cpu.AF.upper, -);

                flag_write(Z, N, H, C, 0b0000);

                cpu.clock += 8;
                break;
            case(0XFF):
                /* RST 38H - 1 byte, 16 cycles *
                    * call abs addr at opcode     *
                    * FLAGS: -,-,-,-              */
                mem_write(--cpu.SP, cpu.PC);
                mem_write(--cpu.SP, cpu.PC >> 8);

                cpu.PC = cpu.CIR & 0XFF;
                cpu.clock += 16;
                break;
        }
    }
}
