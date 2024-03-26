#ifndef ONE_PASS_H_INCLUDED
#define ONE_PASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "optable.h"
#include "lexer.h"

extern void printTokens();
extern void lex(char *source, int source_len);

struct INS {
       uint16_t opcode:   6;
       uint16_t operand: 10;
};

#endif // ONE_PASS_H_INCLUDED
