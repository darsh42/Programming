#include <stdio.h>
#define MAXLINE 1000

/* function declarations */
int getline2(char line[], int maxline);
void copy(char to[], char from[]);

/* main function */
int main(){
  int len;                            /* Current line length */
  int max;                            /* Current maximum length */
  char line[MAXLINE];                 /* Current input line */
  char longest[MAXLINE];              /* Current longest line */

  max = 0;
  while ((len = getline2(line, MAXLINE)) > 0) {

    if (len > max) {
      max = len;
      copy(longest, line);
    }

  if (max > 0)
    printf("\n%s\n", longest);
  }

  return 0;
}

/* getline will read input and store into array 
 * INPUT  ---> Array in which to hold, MAXLINE number
 * OUTPUT ---> Array of chars                  
*/
int getline2(char s[], int lim) {
  int c, i;

  for (i = 0; i < lim-1 && (c = getchar()) != EOF && c != '\n'; ++i)
      s[i] = c;
  
  if (c == '\n') {
    s[i] = c;
    ++i;
  }
  s[i] = '\0';
  return i;
}

/* copy the contents of one char array to another 
 * assuming it is large enough to accomodate
*/
void copy(char to[], char from[]) {
  int i;

  i = 0;
  while ((to[i] = from[i]) != '\0') {
    ++i;
  }
}
