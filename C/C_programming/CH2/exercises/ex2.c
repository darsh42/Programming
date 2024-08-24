/* Exercise 2.2 - write a for loop without && and || */

#include <stdio.h>
#define LIMIT 5

int main() {
  int i = 0;
  int lim = LIMIT;
  int c[LIMIT];

  for (i = 0; i < lim; ++i) {
    if ((c[i] = getchar()) != EOF)
      continue;
    break;
  }

  i = 0;
  while (i < lim) {
    if (c[i] != '\n') {
      putchar(c[i]);
      ++i;
    }
      else
        continue;
    }
}
