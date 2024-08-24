/* re-writing function "lower" using conditional expressions */

#include <stdio.h>

void lower(char string[]);

int main() {
  char string[] = "Hello My Name Is Darsh\0";

  lower(string);

  return 0;
}

void lower(char string[]) {
  int i;

  for(i=0; string[i] != '\0'; i++) {
    printf("%c", (string[i] >= 'A' && string[i] <= 'Z') ? string[i] + 32: string[i]);
  }
  printf("\n");
}
