/* Print lines with more than 80 characters */
#include <stdio.h>
#define MAXLINELEN 1000 /* maximum array size */
#define MAXLEN 8       /* Print limit */

int getline2(char line[], int limit);

int main() {

  int len;
  char line[MAXLINELEN];

  while ((len = getline2(line, MAXLINELEN)) > 0) {

    if (len > MAXLEN) 
      printf("\n%s", line);

  }
  printf("\n");
  return 0;
}

int getline2(char str[], int lim) {
  int c, i;

  for (i = 0; i<lim-1 && (c = getchar()) != EOF && c != '\n'; ++i) 
    str[i] = c;

  if (c == '\n') {
    str[i] = '\n';
    ++i;
  }
  str[i] = '\0';
  return i;
}
