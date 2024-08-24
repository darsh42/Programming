#include "solution.h"

#define ADDX(cpu, V) cpu.reg_x += V
#define NOOP

void addx(char *line);
int signal_strength();
void cpu_init();

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

    while (getline(&line, &bufsize, fp) >= 0) {
        cpu.clock++;
        if ((cpu.clock - 20) % 40 == 0) part1 += signal_strength();
        switch(line[0]) {
            case 'a':
                cpu.clock++;
                if ((cpu.clock - 20) % 40 == 0) part1 += signal_strength();
                addx(&line[5]);
        }

        printf("[CPU] clock -> %d, X -> %d\n", cpu.clock, cpu.reg_x);
        printf("[Part 1]: %d\n", part1);
    }

    printf("[CPU] clock -> %d, X -> %d\n", cpu.clock, cpu.reg_x);
    printf("[Part 1]: %d\n", part1);

    return 0;
}

void cpu_init() {
    cpu.clock = 0;
    cpu.reg_x = 1;
}

int signal_strength() {
    return cpu.clock * cpu.reg_x;
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
