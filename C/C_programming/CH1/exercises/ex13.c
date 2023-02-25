/* Create histogram for the length of characters in a word */

#include <stdio.h>
#define IN 1
#define OUT 0
#define START 0
#define MAXNUM 15

int main() {
  int c, i, index, nc, status;                                  /* variable definition */
  int wordlen[MAXNUM];                                          /* array definition */

  status = OUT;                                                 /* set starting status to OUT*/
  nc = 0;                                                       /* set element in array tracker to 0 */

  for (index = 0; index < MAXNUM; ++index)                      /* create for loop to set all values in array to 0 */
    wordlen[index] = 0;

  while (((c = getchar()) != EOF) && (nc < MAXNUM)) {           /* while loop terminate if EOF or array size is exeeded */

    if (c == ' ' || c == '\n' || c == '\t') {                   /* word separator definition */
      status = OUT;
      ++nc;
    }

    else if (status == OUT) {                                   /* when word is started */
      ++wordlen[nc];
      status = IN;
    }

    else                                                        /* when inside a word */
      ++wordlen[nc];
  }
 
  printf("\n");                                                 /* printing to terminal */
  for (index = 0; index < MAXNUM; ++index) {
    for (i = 0; i <= wordlen[index]; ++i)
      printf("-");
    printf("\n");
  }
  printf("\n");

}
