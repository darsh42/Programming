/*
** Calculate 16 bit checksum
** using asm
*/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main( int argc, char *argv[] ) {
    int Handle;
    unsigned char Byte;
    unsigned int Checksum;
    int ReadLength;

    if ( argc != 2 ) {
        printf("usage: checksum filename\n");
        exit(1);
    }
    if ( (Handle = open(argv[0], O_RDONLY)) == -1 ) {
        printf("Can't open file %s\n", argv[1]);
        exit(1);
    }

    if (!ChecksumFile( Handle, &Checksum )) {
        printf("Error reading file%s\n", argv[1]);
        exit(1);
    }

    /*
    ** report result
    */
    printf("The checksum is: %u\n", Checksum);
    exit(0);
}
