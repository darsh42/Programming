/* Program will invert the last n bits from position p
 * bits in number X
 *
 * */

#include <stdio.h>
#define X 0xf994
#define P 4
#define N 5

/* will invert the last bits in number
 * INPUTS: --> number as x
 *         --> position as p
 *         --> number of bits as n
 *
 * OUTPUT: --> nim with the last n bits from pos p inverted */
unsigned invertLast(unsigned x, int p, int n);

void printBinary(int number);


int main(){
  printBinary(X);
  printBinary(invertLast(X, P, N));
  return 0;
}

unsigned invertLast(unsigned x, int p, int n) {
  return (x << p + 1 - n) | (~x & ~(~0 << n));
}
 
void printBinary(int number) {
  int i = 0;
  char binary[17] = "";
  //int div = 1;
  binary[i++] = '\0';

  for (; i < 17 && number != 0; ++i) {
    if (number % 2 == 0) 
      binary[i] = '0';
    else 
      binary[i] = '1';

    number = (int) number / 2;

  }
  
  for (; i < 17; ++i) {
    binary[i] = '0';
  }

  for (; i >= 0; i--) {
    printf("%c", binary[i]);
  }
  printf("\n");
}

