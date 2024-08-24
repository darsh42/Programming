#ifndef SOLUTION_H_
#define SOLUTION_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROUNDS 10000
#define isDigit(c) (c >= '0' && c <= '9')

typedef struct monkey {
    int num;
    int items[25]; int next_item;

    char op_type; int op_num;

    int test; int T; int F;

    int inspected;
} monkey_T;

void _round();
void get_monkey();
void get_items();
void get_op();
void get_test();
void get_true();
void get_false();
void print_monkeys();

#endif // SOLUTION_H_
