/* Program to convert hex values to decimal values */
#include <stdio.h>

/* converts the string into decimals
 * INPUTS: -> string in format 0x?? or 0X??
 * OUTPUT: -> integer value corresponding to hex */
int htoi(char s[4]);

int main() {
  char string[4] = "0x32";
  int result = htoi(string);
  
  printf("%d\n", result);
  return 0;
}

int pow2(int base, int power) {
  int i;
  int result = 1;

  if (power < 0)
    return -1;

  for (i = 0; i < power; ++i)
    result *= base;

  return result;
}


int htoi(char s[4]) {
  int i;
  int result = 0;
  int n[2];

  for (i = 2; i < 4; ++i) {
    if (s[i] >= '0' && s[i] <= '9')
      n[i - 2] = s[i] - '0';
    else if (s[i] >= 'a' && s[i] <= 'f') 
      n[i - 2] = s[i] - 'a' + 10;
    else if (s[i] >= 'A' && s[i] <= 'F')
      n[i - 2] = s[i] - 'A' + 10;
    else 
      printf("num Not in range\n");
  }

  for (i = 0; i < 2; ++i) {
    result += pow2(16, 1 - i) * n[i];
  }
  
  return result;
}
