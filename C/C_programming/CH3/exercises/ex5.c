/* will convert integer into a string in the representation of base char */
#include <stdio.h>
#include <string.h>
#define INPUT 235
#define BASE 2
#define MAX_LEN 100

/* Will convert int into string of chars with respect to base given     
 * INPUTS: --> number n
 *         --> string s
 *         --> base b
 *
 * OUTPUT: --> str with coverted values
 *                                                                      */
void itob(int num, char str[], int base);


/* Will reverse any string given to it
 * INPUTS: --> string s
 *
 * OUTPUT: --> reversed string s                                        
 *                                                                      */
void reverse(char s[]);

int main() {
  int num = INPUT;
  int base = BASE;
  char string[MAX_LEN];


  itob(num, string, base);

  printf("%s\n", string);

  return 0;
}



void itob(int num, char str[], int base) {
  int i, sign, tmp;

  if ((sign = num) < 0)
    num = -num;

  do {
    /* - tmp holds the remainder of computation
     * - if the value is less than 10 then char is between 0 - 9 
     * - if the vlaue is greater than 10 then value is between A-Z */
    str[i++] = ((tmp = num % base) < 10) ? tmp + '0': (tmp - 10) + 'A';
  }
  while ((num /= base) > 0);

  if (sign < 0)
    str[i++] = '-';

  str[i] = '\0';
  reverse(str);
}




void reverse(char s[]) {
  int c, i, j;

  for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}
