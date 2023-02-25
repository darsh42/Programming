/* program converts integers to strings and will pad the output */
#include <stdio.h>
#include <string.h>
#define NUM 123
#define MIN_WIDTH 8
#define MAX_LEN 100

/* will convert the num into string and pad based on min width 
 * INPUTS: --> number n
 *         --> string s
 *         --> width minWidth
 *
 * OUTPUT: --> padded to min width string version of number n
 *                                                              */
void itoa(int n, char str[], int minWidth);

/* will reverse any string given
 * INPUTS: --> string s
 *
 * OUTPUT: --> reversed s
 *                                                              */
void reverse(char s[]);

int main() {
  int num = NUM;
  int minWidth = MIN_WIDTH;
  char string[MAX_LEN];
  
  itoa(num, string, minWidth);
  printf("%s\n", string);

  return 0;
}


void reverse(char s[]) {
  int c, i, j;

  for (i = 0, j = strlen(s)-1; i<j; i++, j--){
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}


void itoa(int n, char str[], int minWidth) {
  int i, sign;

  if ((sign = n) < 0)
    n = -n;

  do {
    str[i++] = n % 10 + '0';
  } while (n /= 10);
  
  if (sign < 0)
    str[i++] = '-';

  for (; i < minWidth; i++)
    str[i] = ' ';

  str[i] = '\0';

  reverse(str);
}
