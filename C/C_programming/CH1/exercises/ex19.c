#include <stdio.h>
#define MAXLINE 1000

int lineget(char line[], int maxline);
void reverse(char original[],char reversed[], int len);

int main() {
  int len;
  char line[MAXLINE];
  char reversed[MAXLINE];

  while ((len = lineget(line, MAXLINE)) > 2) {
    reverse(line, reversed, len);
    printf("\n%s", reversed);
  }

  return 0;
}

int lineget(char s[], int lim) {
  int c, i;

  for (i = 0; i < lim-1 && ((c = getchar()) != EOF) && c != '\n'; ++i) 
    s[i] = c;

  if (c == '\n') {
    s[i] = '\n';
    ++i;
  }

  s[i] = '\0';
  return i;
}

/* Function will take original string and reverse the output */
void reverse(char original[], char reversed[], int len) {
  int i, j;
  /*  */
  j = 1; 
  /* check for newline character early so it is skipped
   * and added to the end of string if present*/
  if (original[len - 1] == '\n') {
    j = 2;
  }
  
  /* reversing all characters except for terminating character and (if present) newline */

  for (i = len - j; i >= 0; --i)
    reversed[len - (i + j)] = original[i];
  
  
  if (original[len - 1] == '\n') {
    j = 1;
  }
  /* Add terminating character to the end of the string */
  reversed[len] = '\0';
}
