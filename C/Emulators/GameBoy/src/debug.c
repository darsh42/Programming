#include "GBC.h"

uint8_t memory[0X10000];

int main(int argc, char **argv) {
    FILE *rom;
    if ( (rom = fopen(*(argv+=1), "rb")) == NULL) {
        fprintf(stderr, "[Error] debug.c: Cannot read rom <%s>\n", *argv);
        return 1;
    }
    int read_status = 0;
    if ((read_status = fread(memory, 1, sizeof(memory), rom)) == 0) {
        fprintf(stderr, "[Error] debug.c: Could not read <%s>, bytes read: %d\n", *argv, read_status);
        return 1;
    }
    fclose(rom);



    return 0;
}
