#include <stdio.h>

/* program to count number of occurances of different digits,
 * whitespaces(newlines, tabs, spaces) and other characters */
int main() {
  int c, i, nwhite, nother;             /* define variables */ 
  int ndigits[10];                      /* create array */

  nwhite = nother = 0;                  /* start value of nwhite and nother */
  for (i = 0; i < 10; ++i)              /* set all the values in array to 0 */ 
    ndigits[i] = 0;

  while ((c = getchar()) != EOF) {      /* loop condition */
    if (c >= '0' && c <= '9')           /* if c is between or equal to 0 and 9 */
      ++ndigits[c-'0'];                 /* add 1 to the number which is (ASCII:'c' - ASCII:'0') */
    else if (c == ' ' || c == '\n' || c == '\t')
      ++nwhite;
    else 
      ++nother;
  }

  printf("\nDigits:");
  for (i = 0; i < 10; ++i)
    printf("%d:%d ", i, ndigits[i]);
  printf("\nWhite Spaces:%d\nOther:%d\n", nwhite, nother);
}
