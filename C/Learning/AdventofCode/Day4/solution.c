#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc == 0) {printf("Specify input"); return -1;}

    FILE *fp = fopen(argv[1], "r");
    char line[20];

    int total = 0;
    int total_part_1 = 0;
    int total_part_2 = 0;

    while (fgets(line, sizeof(line), fp) != NULL) {
        size_t line_len = strlen(line);

        int index = 0;
        int sectors[4];
        // find the comma
        for (size_t i = 0; i < line_len; i++) {
            char tmp[10];
            if ((line[i+1] == '-') | (line[i+1] == ',') | (line[i+1] == '\0')) {
                sectors[index++] = line[i] - '0';
                i++;
            } else {
                sectors[index++] = atoi(strncpy(tmp, line + i, 2));
                i += 2;
            }
        }

        if ((sectors[0] <= sectors[2] && sectors[1] >= sectors[3]) | (sectors[2] <= sectors[0] && sectors[3] >= sectors[1]))
            total_part_1++;

        if ((sectors[1] < sectors[2]) | (sectors[3] < sectors[0]))
            total_part_2++;

        total++;

    }

    printf("total part 1: %d\n", total_part_1);
    printf("total part 2: %d\n", total - total_part_2);

    return 0;
}
