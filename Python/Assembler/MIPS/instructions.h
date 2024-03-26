#ifndef INSTRUCTIONS_H_INCLUDED
#define INSTRUCTIONS_H_INCLUDED

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define OPCOUNT 53

typedef struct ins {
    char *mneumonic;
    char type;
    // opcode structure
    char *dest; // includes imm
    char *arg1;
    char *arg2;
    int fn_op;
} ins_t;

typedef union MachineCode {

    /* R-Type Machine code */
    struct {
        uint32_t fn: 6;
        uint32_t sa: 5;
        uint32_t rd: 5;
        uint32_t rt: 5;
        uint32_t rs: 5;
        uint32_t op: 6;
    } R_Type;

    /* I-Type Machine code */
    struct {
        uint32_t imm: 16;
        uint32_t rt: 5;
        uint32_t rs: 5;
        uint32_t op: 6;
    } I_Type;

    /* J-Type Machine code */
    struct {
        uint32_t target: 26;
        uint32_t op: 6;
    } J_Type;

    uint32_t code;
} MC;

ins_t optable[OPCOUNT];

#endif // INSTRUCTIONS_H_INCLUDED
