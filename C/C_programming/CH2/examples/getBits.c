
#include <stdio.h>
#define X 0xf994
#define P 4
#define N 3

unsigned getBits(unsigned x, int p, int n);

int main() {
  printf("\n%d\n",getBits(X, P, N));

  return 0;
}


unsigned getBits(unsigned x, int p, int n) {
  return (x >> (p + 1 - n)) & ~(~0 << n);
}
