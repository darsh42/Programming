#ifndef OPTABLE_H_INCLUDED
#define OPTABLE_H_INCLUDED

#include <stdbool.h>

#define N_OPCODES 10

enum opcode {
    LOD,
    STO,
    ADD,
    BZE,
    BNE,
    BRA,
    INP,
    OUT,
    CLA,
    HLT
};

typedef struct OP {
    char        *mnem;
    enum opcode opcode;
    bool        operand;
} OP;


#endif // OPTABLE_H_INCLUDED
