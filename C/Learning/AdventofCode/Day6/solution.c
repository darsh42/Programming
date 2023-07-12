#include <stdio.h>
#include <stdlib.h>

char* read_file(char* filename);

int main(int argc, char* argv[]) {
    if (argc != 2) {printf("specify input file\n"); return -1;}

    int part1 = 0;
    int part2 = 0;
    char* source = read_file(argv[1]);

    int tmp = 0;
    for (int i = 0; source[i] != '\0' && ((part1 == 0) | (part2 == 0)); i++) {
        if (tmp == 0) tmp = i;

        for (int j = i-1; j >= tmp; j--) {
            if (source[i] == source[j]) {
                tmp = j+1;
                break;
            }
        }

        if (i - tmp == 4 && part1 == 0) part1 = i;
        if (i - tmp == 14 && part2 == 0) part2 = i-1;
    }

    printf("part 1: %d\n", part1);
    printf("part 2: %d\n", part2);
    return 0;
}

char* read_file(char* filename) {
    char* buffer = NULL;
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {printf("File %s not found\n", filename); return NULL;}
    if (fseek(fp, 0L, SEEK_END) != 0) {printf("End of file not found\n"); return NULL;}
    size_t buf_size = ftell(fp) ;
    if (buf_size == -1) {printf("Incorrect buffer size\n"); return NULL;}
    buffer = malloc(sizeof(char) * (buf_size + 1));
    if (fseek(fp, 0L, SEEK_SET) != 0) {printf("Cannot find start of file\n"); return NULL;}
    size_t newLen = fread(buffer, sizeof(char), buf_size, fp);
    if (ferror(fp) != 0) {printf("Error reading file"); return NULL;} else {buffer[newLen++] = '\0';}
    fclose(fp);
    return buffer;
}

        /*
        int found = 1;
        for (int j = i; j < i + 14 && found == 1; j++) {
            for (int k = i; k < i + 14 && found == 1; k++) {
                if (j == k) continue;
                if (source[j] == source[k]) {found = 0; goto notFound;}
            }
        }

        notFound:
        if (found == 1) { part1 = i+14; break;}
        */
