#include "solution.h"
#include <stdio.h>

#define ADDX(cpu, V) cpu.reg_x += V
#define NOOP

void addx(char *line);
int signal_strength(int offset);
void cpu_init();
void screen();
void render();

struct cpu cpu;

FILE *fp;

char *line;
size_t bufsize = 10;

int part1 = 0;

int main(int argc, char *argv[]) {

    if (argc != 2) {fprintf(stderr, "[Usage]: ./solution <input_file.txt>\n"); return 1;}
    if ((fp = fopen(*++argv, "r")) == NULL) {fprintf(stderr, "[Error]: File not found <%s>\n", *argv); return 1;}
    if ((line = malloc(bufsize * sizeof(char))) == NULL) {fprintf(stderr, "[Error]: Malloc error\n"); return 1;}

    cpu_init();

    int offset = 0;

    while (getline(&line, &bufsize, fp) >= 0) {
        screen();
        cpu.clock++;
        switch(line[0]) {
            case 'a':
                screen();
                cpu.clock++;
                offset = 1;
        }

        part1 += signal_strength(offset);


        switch (offset) {
            case 1:
                addx(&line[5]);
            default:
                offset = 0;
        }

    }

    render();
    printf("[CPU] clock -> %d, X -> %d\n", cpu.clock, cpu.reg_x);
    printf("[Part 1]: %d\n", part1);

    return 0;
}

void cpu_init() {
    cpu.clock = 0;
    cpu.reg_x = 1;
}

int signal_strength(int offset) {
    if ((cpu.clock - 20) % 40 >= 0 && (cpu.clock - 20) % 40 <= offset)
        return cpu.clock * cpu.reg_x;
    return 0;
}

void addx(char *line) {
    int num = 0;
    int neg = 0;

    if (line[0] == '-') neg = 1;

    for (int i = 0 + neg, j = 0; line[i] >= '0' && line[i] <= '9' && line[i] != '\0'; i++) {
        num = (line[i] - '0') + 10 * num;
    }

    cpu.reg_x += (neg > 0) ? -num: num;
}

char CRT[240];

void screen() {
    int clock = cpu.clock % 40;
    if (clock >= cpu.reg_x - 1 && clock <= cpu.reg_x + 1 && clock < 240) CRT[cpu.clock] = '#';
    else CRT[cpu.clock] = '.';
}

void render() {
    for (int i = 0; i < 240; i++) {
        if (i % 40 == 0) printf("\n");
        printf("%c", CRT[i]);
    }
    printf("\n");
}
