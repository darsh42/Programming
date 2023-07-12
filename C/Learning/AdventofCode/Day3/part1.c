#include <stdio.h>
#include <stdlib.h>

#define LINEBUF 50


void items(char* filename, char lines[300][LINEBUF]);
int misplaced_item(char* items);

int main(int argc, char* argv[]) {

    if ( argc == 0 ) {
        printf("Please specify input file");
        return -1;
    }

    int total = 0;
    char backpacks[300][LINEBUF];
    items(argv[1], backpacks);

    for (int i = 0; i < 300; i++)
        total += misplaced_item(backpacks[i]);

    printf("Total value: %d\n", total);

    return 0;
}


int misplaced_item(char* items) {
    int len = 0;
    while (items[len++] != '\0')
        ;

    char item;

    for (int i = 0; i < len/2 ; i++) {
        for (int j = len/2; j < len; j++) {
            if (items[i] == items[j]) {
                item = items[i];
                break;
            }
        }
    }

    if (item >= 65 && item <= 90) {
        return item - 38;
    }

    if (item >= 97 && item <= 122)
        return item - 96;

    return -1;
}


char* read_file(char* filename);
void items(char* filename, char lines[300][LINEBUF]) {
    char* source = read_file(filename);

    for (int i, j, k = 0; source[i] != '\0'; i++) {
        if (source[i] != '\n') {
            lines[k][j++] = source[i];
        } else {
            lines[k][j++] = '\0';
            j = 0;
            k++;
        }
    }

    free(source);
    return;
}

/* read file line by line
 * PARAMETERS:
 * - Filename
 * RETURN:
 * - char** */
char* read_file(char* filename) {
    char* source = NULL;
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("No file %s is found\n", filename);
        return NULL;
    }

    if ( fseek(fp, 0L, SEEK_END) != 0 ) {
        printf("Cannot find end of file %s\n", filename);
        return NULL;
    }

    size_t buff_size = ftell(fp);

    if (buff_size == -1) {
        printf("buffer size was incorrect!\n");
        return NULL;
    }

    source = malloc(sizeof(char) * (buff_size + 1));

    if ((fseek(fp, 0L, SEEK_SET)) != 0) {
        printf("cannot find start of file %s\n", filename);
        return NULL;
    }

    size_t newLen = fread(source, sizeof(char), buff_size, fp);
    if ( ferror(fp) != 0 ) {
        printf("Error reading file");
    } else {
        source[newLen++] = '\0';
    }

    fclose(fp);

    return source;
}
