#include <stdio.h>
#include <stdlib.h>

typedef struct box {
    char value;
    struct box *next;
} box_t;

void make_box(char value, box_t *top) {
    box_t *p;
    for (p = top; p->next != NULL; p = p->next)
        ;
    p->next = malloc(sizeof(box_t));
    *p->next = (box_t) {value, NULL};
    return;
}

void print_stack(box_t *top) {
    box_t *p;
    for (p = top; p->next != NULL; p = p->next)
        printf("[%c]", p->value);

}

int main(int argc, char* argv[]) {
    if ( argc != 2 ) {fprintf(stderr, "Usage: ./solution2 input.txt\n"); return 1;}

    box_t stacks[9];

    char line[30];
    FILE *fp;

    if ((fp = fopen(*++argv, "r")) == NULL) {fprintf(stderr, "Cannot open %s\n", *argv); return 1;}

    for (int i = 0; fgets(line, 30, fp) != NULL; i++) {
        if (i < 9) {
            for (int j = 1; j < 30; j += 3) {
                if (line[j] != ' ') make_box(line[j], &stacks[(j - 1)/3]);
            }
        } else {
            break;
        }
    }

    for (int i = 0; i < 9; i++) {
        print_stack(&stacks[i]);
    }

    return 0;
}
