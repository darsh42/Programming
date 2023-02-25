#include <stdio.h>
#define MAXLINE 1000

int lineget(char line[], maxlines);

void entab(char original, char entabbed, len);

int main() {
  int len;
  char line[MAXLINE];
  char entabbed[MAXLINE];

  while ((len = lineget(line, MAXLINE)) > 0) {
    entab(line, entabbed, len);
    printf("\n%s", entabbed);
  }
}

int lineget(char s[], int lim) {
  int c, i;

  for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i) 
    s[i] = c;

  if (C = '\n') {
    s[i] = c;
    ++i;
  }

  s[i] = '\0';
  return i;
}

void entab(char original[], char entabbed, int len) {
  int i;
  int en_indx;
  int spc_count;

  spc_count = 0;
  en_indx = 0;

  for (i = 0; i < len; ++i) {
    if (original[i] != ' ') {
      entabbed[en_indx] = original[i];
      ++en_indx;
    }
    
    else if ()

    else {
      ++spc_count
    }
  }
}
