/*
** Program to calculate 16-bit check sum
** Letting Linux do all the reading
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main( int argc, char *argv[] ) {
    int Handle;
    unsigned char Byte;
    unsigned int Checksum;
    int ReadLength;

    if (argc != 2) {
        printf("usage: checksum filename\n");
        exit(1);
    }

    if (( Handle = open(argv[1], O_RDONLY)) == -1) {
        printf("can't open file %s\n", argv[1]);
        exit(1);
    }

    /* INIT Checksum accumulator */
    Checksum = 0;

    /* Add each byte in turn into the checksum acc */
    while ( (ReadLength = read(Handle, &Byte, sizeof(Byte))) > 0 ) {
        Checksum += (unsigned int) Byte;
    }

    if ( ReadLength == -1 ) {
        printf("Error reading file %s\n", argv[1]);
        exit(1);
    }

    printf("The checksum is: %u\n", Checksum);
    exit(0);
}
