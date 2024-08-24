#include "solution.h"
#include <stdio.h>
#include <stdlib.h>

box_t *stacks[9];
char *line;
FILE *fp;
int cmd[3];
size_t bufsize = LINEBUF;

int main(int argc, char* argv[]) {
    if ( argc != 2 ) {fprintf(stderr, "[Usage]: ./solution2 input.txt\n"); return 1;}

    if ((fp = fopen(*++argv, "r")) == NULL) {fprintf(stderr, "[Error]: Cannot open %s\n", *argv); return 1;}

    char *line = malloc(bufsize * sizeof(char));
    for (int i = 0; i < 9; i++) {
        stacks[i] = malloc(sizeof(box_t));
        stacks[i]->next = NULL;
    }

    for (int line_num = 0; getline(&line, &bufsize, fp) > 0; line_num++) {
        if (line_num < 9) {
            for (int j = 1; j < LINEBUF; j += 4) {
                if (line[j] >= 'A' && line[j] <= 'Z')
                    append_box(line[j], stacks[(j - 1)/4]);
            }
        }

        if (line[0] != 'm') continue;

        for (size_t i = 0, j = 0; line[i] != '\0'; i++) {
            if (line[i - 1] >= '1' && line[i - 1] <= '9') continue;
            if (line[i] >= '1' && line[i] <= '9') cmd[j++] = _atoi(&line[i]) - 1;
        }

        //for (int i = 0; i < cmd[0]+1; i++) {
            //char box = pop(&stacks[cmd[1]]);
            //if (box == '\0') {
                //fprintf(stderr, "[Error]: reached the end of stack <stack %d>\n", cmd[1]);
                //return 1;
            //}
            //prepend_box(box, &stacks[cmd[2]]);
        //}

        move_n_box(&stacks[cmd[1]], &stacks[cmd[2]], cmd[0] + 1);

        printf("source line: %d\n", line_num);
        printf("command    : %s\n", line);
        for (int i = 0; i < 9; i++) {
            printf("tower: %d ->", i + 1);
            print_stack(stacks[i]);
        }
        printf("\n");
    }

    for (int i = 0; i < 9; i++) {
        printf("%c", stacks[i]->value);
    }
    printf("\n");

    return 0;
}

void append_box(char value, box_t *top) {
    box_t *current = top;
    while (current->next != NULL)
        current = current->next;

    current->next = malloc(sizeof(box_t));
    current->next->next = NULL;
    current->value = value;
    return;
}

void prepend_box(char value, box_t **top) {
    box_t *new_top;
    new_top = malloc(sizeof(box_t));

    new_top->value = value;
    new_top->next = *top;
    *top = new_top;
}

char pop(box_t **top) {
    if (top == NULL) {
        return '\0';
    }

    char return_val = (*top)->value;
    box_t *new_top = (*top)->next;
    free(*top);
    *top = new_top;

    return return_val;
}

void move_n_box(box_t **source, box_t **dest, int n) {
    char move[20] = "";

    for (int i = 0; i < n; i++) {
        char tmp = pop(source);
        strcat(move, &tmp);
        move[i+1] = '\0';
    }

    for (int i = n - 1; i >= 0; i--) {
        prepend_box(move[i], dest);
    }
}

void print_stack(box_t *top) {
    box_t *p;
    for (p = top; p->next != NULL; p = p->next)
        printf("%c", p->value);
    printf("\n");
}

int _atoi(char *line) {
    int num = 0;

    for (int i = 0, j = 0; line[i] >= '0' && line[i] <= '9' && line[i] != '\0'; i++) {
        num = (line[i] - '0') + 10 * num;
    }

    return num;
}
