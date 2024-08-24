#include "Assembler.h"

struct Symbol *symbol_table;
struct Instruction optable[38];

MC assemble_R_Type(struct Instruction op, char *line);
MC assemble_I_Type(struct Instruction op, char *line);
MC assemble_S_Type(struct Instruction op, char *line);
MC assemble_U_Type(struct Instruction op, char *line);
MC assemble_B_Type(struct Instruction op, char *line);
MC assemble_J_Type(struct Instruction op, char *line);

char *get_mneumonic(char *line);

void init() {
        optable[0]  = (struct Instruction) {  "add", "ADD,                         ", 'R', 0b0110011, 0x0,           0x00, "rd = rs1 + rs2"};
        optable[1]  = (struct Instruction) {  "sub", "SUB,                         ", 'R', 0b0110011, 0x0,           0x20, "rd = rs1 - rs2"};
        optable[2]  = (struct Instruction) {  "xor", "XOR,                         ", 'R', 0b0110011, 0x4,           0x00, "rd = rs1 ˆ rs2"};
        optable[3]  = (struct Instruction) {   "or", "OR,                          ", 'R', 0b0110011, 0x6,           0x00, "rd = rs1 | rs2"};
        optable[4]  = (struct Instruction) {  "and", "AND,                         ", 'R', 0b0110011, 0x7,           0x00, "rd = rs1 & rs2"};
        optable[5]  = (struct Instruction) {  "sll", "Shift Left Logical,          ", 'R', 0b0110011, 0x1,           0x00, "rd = rs1 << rs2"};
        optable[6]  = (struct Instruction) {  "srl", "Shift Right Logical,         ", 'R', 0b0110011, 0x5,           0x00, "rd = rs1 >> rs2"};
        optable[7]  = (struct Instruction) {  "sra", "Shift Right Arith*,          ", 'R', 0b0110011, 0x5,           0x20, "rd = rs1 >> rs2 msb-extends"};
        optable[8]  = (struct Instruction) {  "slt", "Set Less Than,               ", 'R', 0b0110011, 0x2,           0x00, "rd = (rs1 < rs2)?1:0"};
        optable[9]  = (struct Instruction) { "sltu", "Set Less Than, (U),          ", 'R', 0b0110011, 0x3,           0x00, "rd = (rs1 < rs2)?1:0 zero-extends"};
        optable[10] = (struct Instruction) { "addi", "ADD Immediate,               ", 'I', 0b0010011, 0x0,           0xff, "rd = rs1 + imm"};
        optable[11] = (struct Instruction) { "xori", "XOR Immediate,               ", 'I', 0b0010011, 0x4,           0xff, "rd = rs1 ˆ imm"};
        optable[12] = (struct Instruction) {  "ori", "OR Immediate,                ", 'I', 0b0010011, 0x6,           0xff, "rd = rs1 | imm"};
        optable[13] = (struct Instruction) { "andi", "AND Immediate,               ", 'I', 0b0010011, 0x7,           0xff, "rd = rs1 & imm"};
        // {" slli", "Shift Left Logical, Imm,     ", 'I', 0010011, 0x1, imm[5:11]=0x00, "rd = rs1 << imm[0:4]"};
        // {" srli", "Shift Right Logical Imm,     ", 'I', 0010011, 0x5, imm[5:11]=0x00, "rd = rs1 >> imm[0:4]"};
        // {" srai", "Shift Right Arith Imm,       ", 'I', 0010011, 0x5, imm[5:11]=0x20, "rd = rs1 >> imm[0:4] msb-extends"};
        optable[14] = (struct Instruction) { "slli", "Shift Left Logical, Imm,     ", 'I', 0b0010011, 0x1,           0x00, "rd = rs1 << imm[0:4]"};
        optable[15] = (struct Instruction) { "srli", "Shift Right Logical Imm,     ", 'I', 0b0010011, 0x5,           0x00, "rd = rs1 >> imm[0:4]"};
        optable[16] = (struct Instruction) { "srai", "Shift Right Arith Imm,       ", 'I', 0b0010011, 0x5,           0x20, "rd = rs1 >> imm[0:4] msb-extends"};
        optable[17] = (struct Instruction) { "slti", "Set Less Than Imm,           ", 'I', 0b0010011, 0x2,           0xff, "rd = (rs1 < imm)?1:0"};
        optable[18] = (struct Instruction) {"sltiu", "Set Less Than Imm (U),       ", 'I', 0b0010011, 0x3,           0xff, "rd = (rs1 < imm)?1:0 zero-extends"};
        optable[19] = (struct Instruction) {   "lb", "Load Byte,                   ", 'I', 0b0000011, 0x0,           0xff, "rd = M[rs1+imm][0:7]"};
        optable[20] = (struct Instruction) {   "lh", "Load Half,                   ", 'I', 0b0000011, 0x1,           0xff, "rd = M[rs1+imm][0:15]"};
        optable[21] = (struct Instruction) {   "lw", "Load Word,                   ", 'I', 0b0000011, 0x2,           0xff, "rd = M[rs1+imm][0:31]"};
        optable[22] = (struct Instruction) {  "lbu", "Load Byte, (U),              ", 'I', 0b0000011, 0x4,           0xff, "rd = M[rs1+imm][0:7] zero-extends"};
        optable[23] = (struct Instruction) {  "lhu", "Load Half, (U),              ", 'I', 0b0000011, 0x5,           0xff, "rd = M[rs1+imm][0:15] zero-extends"};
        optable[24] = (struct Instruction) {   "sb", "Store Byte,                  ", 'S', 0b0100011, 0x0,           0xff, "M[rs1+imm][0:7] = rs2[0:7]"};
        optable[25] = (struct Instruction) {   "sh", "Store Half,                  ", 'S', 0b0100011, 0x1,           0xff, "M[rs1+imm][0:15] = rs2[0:15]"};
        optable[26] = (struct Instruction) {   "sw", "Store Word,                  ", 'S', 0b0100011, 0x2,           0xff, "M[rs1+imm][0:31] = rs2[0:31]"};
        optable[28] = (struct Instruction) {  "beq", "Branch ==,                   ", 'B', 0b1100011, 0x0,           0xff, "if(rs1 == rs2) PC += imm"};
        optable[29] = (struct Instruction) {  "bne", "Branch !=,                   ", 'B', 0b1100011, 0x1,           0xff, "if(rs1 != rs2) PC += imm"};
        optable[30] = (struct Instruction) {  "blt", "Branch <,                    ", 'B', 0b1100011, 0x4,           0xff, "if(rs1 < rs2) PC += imm"};
        optable[31] = (struct Instruction) {  "bge", "Branch ≥,                    ", 'B', 0b1100011, 0x5,           0xff, "if(rs1 >= rs2) PC += imm"};
        optable[32] = (struct Instruction) { "bltu", "Branch < (U),                ", 'B', 0b1100011, 0x6,           0xff, "if(rs1 < rs2) PC += imm zero-extends"};
        optable[33] = (struct Instruction) { "bgeu", "Branch ≥ (U),                ", 'B', 0b1100011, 0x7,           0xff, "if(rs1 >= rs2) PC += imm zero-extends"};
        optable[34] = (struct Instruction) {  "jal", "Jump And Link,               ", 'J', 0b1101111, 0xf,           0xff, "rd = PC+4; PC += imm"};
        optable[35] = (struct Instruction) { "jalr", "Jump And Link Reg,           ", 'I', 0b1100111, 0x0,           0xff, "rd = PC+4; PC = rs1 + imm"};
        optable[36] = (struct Instruction) {  "lui", "Load Upper Imm,              ", 'U', 0b0110111, 0xf,           0xff, "rd = imm << 12"};
        optable[37] = (struct Instruction) {"auipc", "Add Upper Imm to PC,         ", 'U', 0b0010111, 0xf,           0xff, "rd = PC + (imm << 12"};
}

int main( int argc, char **argv ) {

    if (argc != 2) {
        printf("Usage: ./as <riskV.asm>");
        return 1;
    }

    FILE *source = fopen(*(++argv), "r");

    char line[100];
    size_t buffsize = 100;

    while (fgets(line, buffsize, source) != NULL) {
        int opnum;

        // find instruction
        for (opnum = 0; strcmp(get_mneumonic(line), optable[opnum].mneumonic) != 0 && opnum < 38; opnum++)
            ;

        MC code;
        switch(optable[opnum].type) {
            case('R'): code = assemble_R_Type(optable[opnum], line); break;
            case('I'): code = assemble_I_Type(optable[opnum], line); break;
            case('S'): code = assemble_S_Type(optable[opnum], line); break;
            case('U'): code = assemble_U_Type(optable[opnum], line); break;
            case('B'): code = assemble_B_Type(optable[opnum], line); break;
            case('J'): code = assemble_J_Type(optable[opnum], line); break;
            default: fprintf(stderr, "Un-implemented Instruction Type\n");
        }


        // write to file?
    }

    return 0;
}

MC assemble_R_Type(struct Instruction op, char *line) {
    MC assembly;
    int *regs;
    get_operands(line);

    assembly.R_Type.opcode = op.opcode;
    assembly.R_Type.funct3 = op.funct3;
    assembly.R_Type.funct7 = op.funct7;
    assembly.R_Type.rd = regs[0];
    assembly.R_Type.rs1 = regs[1];
    assembly.R_Type.rs2 = regs[2];

    return assembly;
}

MC assemble_I_Type(struct Instruction op, char *line) {

}
