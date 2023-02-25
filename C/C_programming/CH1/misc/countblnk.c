#include <stdio.h>

int main() {
  int c, ntab, nblank, nnew;
  
  nnew = 0;
  nblank = 0;
  ntab = 0;
  while ((c = getchar()) != EOF) {
    if (c == '\b')
      ++nblank;
    if (c == '\t')
      ++ntab;
    if (c == '\n')
      ++nnew;
  }
  printf("Blanks:%d Tabs:%d NewLines:%d\n", nblank, ntab, nnew);
}
