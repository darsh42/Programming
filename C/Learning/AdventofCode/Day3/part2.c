#include <stdio.h>
#include <stdlib.h>

#define LINEBUF 50


void items(char* filename, char lines[300][LINEBUF]);

int misplaced_item(char* bp_one, char* bp_two, char* bp_three);

int main(int argc, char* argv[]) {

    if ( argc == 0 ) {
        printf("Please specify input file");
        return -1;
    }

    int total = 0;
    char backpacks[300][LINEBUF];
    items(argv[1], backpacks);

    for (int i = 0; i < 300; i++)
        total += misplaced_item(backpacks[i], backpacks[i++], backpacks[i++]);

    printf("Total value: %d\n", total);

    return 0;
}


int misplaced_item(char* bp_one, char* bp_two, char* bp_three) {
    char item;
    int pos_index = 0;
    char possible_items[LINEBUF];

    for (int i = 0; bp_one[i] != '\0'; i++) {
        for (int k = 0; bp_two[k] != '\0'; k++) {
            if (bp_one[i] == bp_two[k]) {
                possible_items[pos_index++] = bp_one[i];
            }
        }
    }

    possible_items[pos_index++] = '\0';

    for (int i, found = 0 ; possible_items[i] != '\0' && found == 0; i++)
        for (int j = 0; bp_three[j] != '\0'; j++)
            if (bp_three[j] == possible_items[i]) {
                item = bp_three[j];
                found = 1;
                break;
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
