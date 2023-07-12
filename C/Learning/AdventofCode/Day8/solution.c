#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINEBUF 102
#define SOURCEBUF 100

int main(int argc, char *argv[]) {
    if (argc != 2) {printf("specify input\n"); return 1;}

    int part1 = 0;
    int part2 = 0;

    // start at 2 since first and last, rows and cols are not processed
    size_t tree_rows = 1;
    size_t tree_cols = 2;

    char source[SOURCEBUF][LINEBUF];
    int hidden[SOURCEBUF][LINEBUF];

    char line[LINEBUF];
    FILE *fp = fopen(argv[1], "r");

    for (int i = 0; i < SOURCEBUF && fgets(line, LINEBUF, fp); i++) {
        strncpy(source[i], line, LINEBUF);
        /* first and last row are always visble */
        if (i == 0 || i == SOURCEBUF - 1) {continue;}

        int index = 0;
        int hidden_index = 0;
        int tmp[LINEBUF];

        /* finding largest value in all prior values, if a value is never the largest
         * the tree is possibly hidden. FORWARD PASS: */
        for (int j = 1, largest = line[0]; j < LINEBUF - 3; j++) {
            if (line[j] > largest) {largest = line[j]; continue;}
            tmp[index++] = j;
        }

        /* finding largest value in all prior values, if a value is never the largest
         * the tree is possibly hidden. BACKWARDS PASS: */
        for (int j = LINEBUF - 3, largest = line[LINEBUF-2]; j > 0; j--) {
            if (line[j] > largest) {largest = line[j]; continue;}
            for(int k = index; k >=0; k--)
                if (tmp[k] == j) {
                    hidden[i][hidden_index++] = j;
                    break;
                }
        }

        hidden[i][hidden_index] = -1;
        tree_rows++;
    }

    for (int col = 1; col < LINEBUF - 1; col++) {
        int tmp_index = 0;
        int tmp[LINEBUF];

        for (int row = 1; row < SOURCEBUF - 1; row++) {
            for (int element = 0; hidden[row][element] != -1; element++) {
                if (hidden[row][element] == col) {tmp[tmp_index++] = row; break;}
            }
        }

        if (tmp_index == 0) {continue;}

        tmp[tmp_index] = -1;
        tmp_index = 0;

        for (int row = 1, largest = source[0][col]; row < SOURCEBUF - 1; row++) {
            if (source[row][col] > largest) {largest = source[row][col]; continue;}
            for (int i = 0; tmp[i] != -1; i++) {
                if (tmp[i] == row) {tmp[tmp_index++] = row;}
            }
        }

        if (tmp_index == 0) {continue;}
        tmp[tmp_index] = -1;

        for (int row = SOURCEBUF - 2, largest = source[SOURCEBUF-1][col]; row > 0; row--) {
            if (source[row][col] > largest) {largest = source[row][col]; continue;}
            for (int i = 0; tmp[i] != -1; i++) {
                if (tmp[i] == row) {part1++;}
            }
        }
        tree_cols++;
    }

    printf("part 1: %ld\n",(tree_rows * tree_cols) - part1);

    fclose(fp);

    return 0;
}
