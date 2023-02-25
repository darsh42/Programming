#include <stdio.h>
#define IN 1
#define OUT 0

int main() {
  int c, state;                                 /* define the starting variables*/
  
  state = IN;                                   /* the word starts here*/
  while ((c = getchar()) != EOF) {              /* The loop condition is defined */
    if (c == ' ' || c == '\n' || c == '\t') {   /* if the character read is a blank, newline or tab*/
      putchar('\n');                            /* put the newline character*/
      state = OUT;                              /* change the state to OUT to start a new word*/
    }
    else if (state == OUT) {                    /* if the state is set to OUT*/
      state = IN;                               /* change the state to IN as it is start of word*/
      putchar(c);                               /* put the character to the screen*/
    }
    else {                                      /* if none of the above conditions are satisfied*/
      putchar(c);                               /* put the character to the screen*/
    }
  }
}
