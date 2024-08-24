#ifndef ASSEMBLER_H_INCLUDED
#define ASSEMBLER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

struct Instruction {
    char *mneumonic;
    char *name;
    char type;
    uint8_t opcode;
    uint8_t funct3;
    uint8_t funct7;
    char *description;
};

struct Symbol {

};


// Machine Code union
typedef union MachineCode {

    // Register Instructions
    struct {
        uint32_t opcode: 7;
        uint32_t rd: 5;
        uint32_t funct3: 3;
        uint32_t rs1: 5;
        uint32_t rs2: 5;
        uint32_t funct7: 7;
    } R_Type;

    // Immediate Instructions
    struct {
        uint32_t opcode: 7;
        uint32_t rd: 5;
        uint32_t funct3: 3;
        uint32_t rs1: 5;
        uint32_t imm_0_11: 12;
    } I_Type;

    // Store instructions
    struct {
        uint32_t opcode: 7;
        uint32_t imm_0_4: 5;
        uint32_t funct3: 3;
        uint32_t rs1: 5;
        uint32_t rs2: 5;
        uint32_t imm_5_11: 7;
    } S_Type;

    // Branch Instructions
    struct {
        uint32_t opcode: 7;
        uint32_t imm_11: 1;
        uint32_t imm_1_4: 4;
        uint32_t funct3: 3;
        uint32_t rs1: 5;
        uint32_t rs2: 5;
        uint32_t imm_5_10: 6;
        uint32_t imm_12: 1;
    } B_Type;

    // Upper Instructions
    struct {
        uint32_t opcode: 7;
        uint32_t rd: 5;
        uint32_t imm_12_31: 20;
    } U_Type;

    // Jump Instruction
    struct {
        uint32_t opcode: 7;
        uint32_t rd: 5;
        uint32_t imm_12_19: 20;
        uint32_t imm_11: 1;
        uint32_t imm_1_10: 20;
        uint32_t imm_20: 20;
    } J_Type;

    uint32_t code;
} MC;


#endif // ASSEMBLER_H_INCLUDED
