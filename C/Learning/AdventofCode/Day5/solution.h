#ifndef SOLUTION_H_
#define SOLUTION_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINEBUF 34

typedef struct box {
    char value;
    struct box *next;
} box_t;

void append_box(char value, box_t *top);
void prepend_box(char value, box_t **top);

char pop(box_t **top);
void move_n_box(box_t **source, box_t **dest, int n);

void print_stack(box_t *top);
int _atoi(char *line);

#endif // SOLUTION_H_
