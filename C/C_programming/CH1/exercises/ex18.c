/* Remove any trailing blanks and tabs from each line
 * fo input, remove lines without any text completly  */

#include <stdio.h>
#define MAXLINE 1000
#define STORED 1
#define NOT_STORED 0

/* will process data in each line
 * and change them so only desired
 * elements are included in array */
int lineget(char line[], int maxlinelen);

int main() {
  char line[MAXLINE];

  while (lineget(line, MAXLINE) > 2) {
    printf("\n%s", line);
  }

  //printf("\n");
  return 0;
}

int lineget(char s[], int lim) {
  int c, i;
  int status;
  int term_index;

  status = NOT_STORED;
  term_index = 0;
  i = 0;

  for (i = 0; i<lim-1 && ((c = getchar()) != EOF) && c != '\n'; ++i) {
    /* When c is tab OR spc AND status is NOT STORED 
     * - the terminator will record the index
     * - the status will indicate the terminator is stored */
    if ((c == '\t' || c == ' ') && (status != STORED)) {
      term_index = i;
      status = STORED;
    }
    /* When c is NOT tab OR spc AND status is STORED
     * - will reset the terminator to 0
     * - will change stored to NOT STORED                  */

    if ((c != '\t' && c != ' ') && (status == STORED)) {
      term_index = i;
      status = NOT_STORED;
      ++term_index;
    }

    /* Every read character is loaded into the array */
    s[i] = c;
  }

  /* if the line ends with \n */
  if (c == '\n') {
    s[term_index] = '\n';
    ++term_index;
  }
  

  /* if the line has been formatted */
  s[term_index] = '\0';
  ++term_index;

  return term_index;
} 
