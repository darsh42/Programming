/* replace all tabs in line with equivelent number of spaces */
#include <stdio.h>
#include <stdlib.h>
#define MAXLINE 1000
#define SPACE_IN_TAB "    "

/* read the line  */
int lineget(char line[], int maxline);
/* replace all tabs with equivelent spaces */
void replace(char original[], char replaced[], int len);

int main() {
  int len;
  char line[MAXLINE];
  char replaced[MAXLINE];

  while ((len = lineget(line, MAXLINE)) > 0) {
    replace(line, replaced, len);
    printf("\n%s", replaced);
  }

  return 0;
}

/* read line and return as a string in line[] */
int lineget(char line[], int lim) {
  int c, i;

  for (i = 0; i < lim-1 && (c = getchar()) != EOF && c != '\n'; ++i)
    line[i] = c;

  if (c == '\n') {
    line[i] = '\n';
    ++i;
  }

  line[i] = '\0';
  return i;
}

void replace(char orig[], char repl[], int len) {
  int i;
  int temp;
  int num_spaces;
  int repl_indx;

  repl_indx = 0;
  for (i = 0; i < len; ++i) {
    
    if (orig[i] != '\t') {
      repl[repl_indx] = orig[i];
      ++repl_indx;
      continue;
    }
    
    num_spaces = abs(8 - (repl_indx % 8));
    
    for (temp = repl_indx; repl_indx < MAXLINE && repl_indx < temp + num_spaces; ++repl_indx) 
      repl[repl_indx] = ' ';
  }
}
