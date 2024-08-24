/* illustrates how a function is constructed
 * and executed, it will show attributes unique
 * to functions                                 */

#include <stdio.h>



int power(int base, int exp) {
  int p;

  for (p = 1; exp > 0; --exp)
    p *= base;

  return p;
}

int main() {
  int i;

  for (i = 0; i < 10; ++i) 
    printf("%3d %10d\n", power(2, i), power(-3, i));

  return 0;
}
