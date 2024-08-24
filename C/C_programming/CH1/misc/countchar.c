#include <stdio.h>

/*
int main() {
  long nc;

  while (getchar() != EOF)
    ++nc;
  printf("%ld\n", nc);

}
*/

int main() {
  double nc;
  for (nc = 0; getchar() != EOF; nc++)
    ;
  printf("%.0f\n",nc);
}
