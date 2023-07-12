/*
** Program to calculate 16-bit check sum
** Using getc() for every byte
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main( int argc, char *argv[] ) {
    FILE *CheckFile;
    int Byte;
    unsigned int Checksum;

    if (argc != 2) {
        printf("usage: checksum filename\n");
        exit(1);
    }

    if (( CheckFile = fopen(argv[1], "rb")) == NULL) {
        printf("can't open file %s\n", argv[1]);
        exit(1);
    }

    /* INIT Checksum accumulator */
    Checksum = 0;

    /* Add each byte in turn into the checksum acc */
    while ( (Byte = getc(CheckFile)) != EOF) {
        Checksum += (unsigned int) Byte;
    }

    printf("The checksum is: %u\n", Checksum);
    exit(0);
}
