#include <stdio.h>
#include <stdlib.h>
#define  SIZE 10


int main() {
  int n, t, array[SIZE], getint(int *);

  for (n = 0; n < SIZE && getint(&t) != EOF; n++)
    printf("%d: %d\n", n, array[n]);
}
