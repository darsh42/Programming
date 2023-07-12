#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINEBUF 50

int priority(char item);

int main(int argc, char* argv[]) {

    if (argc == 1) {printf("Specify input\n"); return -1;}

    int elfnum = 0;
    char group[3][LINEBUF];

    char line[LINEBUF];
    FILE *fp = fopen(argv[1], "r");

    int total_part_1 = 0;
    int total_part_2 = 0;
    while (fgets(line, LINEBUF, fp) != NULL) {
        size_t len = strlen(line);

        for (size_t comp1 = 0; comp1 < len/2; comp1++) {
            for (size_t comp2 = len/2; comp2 < len+1; comp2++) {
                if (line[comp1] == line[comp2]) {
                    total_part_1 += priority(line[comp1]);
                    goto part2;
                }
            }
        }

        part2:

        elfnum++;

        if (elfnum % 3 == 0 && elfnum != 0 ) {
            for (int bp1 = 0; group[0][bp1] != '\0'; bp1++) {
                for (int bp2 = 0; group[1][bp2] != '\0'; bp2++) {
                    for (int bp3 = 0; group[2][bp3] != '\0'; bp3++) {
                        if (group[0][bp1] == group[1][bp2] && group[0][bp1] == group[2][bp3]) {
                            total_part_2 += priority(group[0][bp1]);
                            goto found;
                        }
                    }
                }
            }
        }
        found:

        strncpy(group[elfnum%3], line, LINEBUF);
    }

    printf("total part 1: %d\n", total_part_1);
    printf("total part 2: %d\n", total_part_2);

    return 0;
}

int priority(char item) {
    return (item > 'Z') ? item - 96: item - 38;
}
