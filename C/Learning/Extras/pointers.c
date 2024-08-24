#include <stdio.h>

int main() {
  int x[] = {1,2,3,4,5};
  int *pX = x;

  printf("val pointed to by pX:%d\n", *pX);
  printf("Value of pX: %p\n", pX);
}
