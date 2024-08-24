/* replace bits in x with bits in y based on position and number of bits  */
#include <stdio.h>
#define X 0xF994
#define Y 0x00B6
#define P 4
#define N 3

/* will set the right n bits from pos p in x to the right most n bits in y
 * INPUTS: --> target value x
 *         --> superposed value y
 *         --> position p
 *         --> number of bits to the right of p, n
 * 
 * OUTPUT: --> bitwise manipulated number
 *                                                                        */
unsigned setBits(unsigned x, unsigned y, int p, int n);

/* Will print any number which is 64 bits in size as binary number 
 * INPUTS: --> String location
 *         --> number to convert
 *
 * OUTPUT: --> binary number in string loaction                           */
void printBinary(int number);

int main() {
  unsigned result = setBits(X, Y, P, N);
  printBinary(result);
  return 0;
}

unsigned setBits(unsigned x, unsigned y, int p, int n) {
  //return ~(x >> (p + 1 - n) & (~0 << n)) | (y & ~(~0 << n));
  //return x | (y & ~(~0 << n ));
  return ((x >> (p + 1 - n)) | ~(~0 << n)) | (y & ~(~0 << n));
}


void printBinary(int number) {
  int i = 0;
  char binary[16] = "";
  //int div = 1;
  binary[i++] = '\0';

  for (i = 0; i < 16 && number != 0; ++i) {
    if (number % 2 == 0) 
      binary[i] = '0';
    else 
      binary[i] = '1';

    number = (int) number / 2;

  }
  
  for (; i < 16; ++i) {
    binary[i] = '0';
  }

  for (; i >= 0; i--) {
    printf("%c", binary[i]);
  }
  printf("\n");
}
