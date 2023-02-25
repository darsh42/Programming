/* a rudementry version of the UNIX command grep
 * PARTS:
 *      
 *      WHILE (there is another line):
 *          IF (line contains pattern):
 *              print(line);
 *
 *                                                */
#include <stdio.h>
#define MAXLINE 1000

int lineget(char line[], int max);
int strindex(char source[], char searchFor[]);

char pattern[] = "ould";

int main(){
  char line[MAXLINE];
  int found = 0;

  while (lineget(line, MAXLINE)) {
    if (strindex(line, pattern) >= 0) {
      printf("%s", line);
      ++found;
    }
  }

  return found;
}


/* will load line into line[] and return the length
 * INPUTS: --> line string
 *         --> MAXLINE length
 *
 * OUTPUT: --> length of line                     */
int lineget(char line[], int max) {
  int c, i;

  for (i = 0; i < max && (c = getchar()) != EOF && c != '\n'; i++) 
    line[i] = c;
  
  if (c == '\n')
    line[i++] = c;
  line[i] = '\0';
  return i;
}

/* will match the pattern given in line and will
 * print line if the pattern is found            
 * INPUTS: --> source line
 *         --> pattern to match
 *
 * OUTPUT: --> print matching lines
 *              |-> exit 0 if found
 *              |-> exit -1 if not found*/
int strindex(char source[], char searchFor[]) {
  int i, j, k;
  int rightMost = -1;

  for (i = 0; source[i] != '\0'; i++) {
    for (j=i, k=0; searchFor[k] != '\0' && searchFor[k] == source[j]; j++, k++)
      ;
    if (k > 0 && searchFor[k] == '\0') 
      rightMost =  i;
  }

  return rightMost;
}
