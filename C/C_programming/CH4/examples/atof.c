/* converts a string into a double precison floating point value */
#include <ctype.h>
#include <stdio.h>
#define TEST_VAL "-204.78"
#define TEST_VAL2 "15092"
/* Will convert string to double precision floating point value
 * INPUTS: --> number in string format
 *
 * OUTPUT: --> float representation of string 
 *                                                               */
double atof(char string[]);

int atoi(char s[]); 

int main() {
  char testCase1[] = TEST_VAL;
  char testCase2[] = TEST_VAL2;

  printf("TEST CASE STRING: %s\nCONVERTED VALUE: %d\n", testCase2, atoi(testCase2));
  printf("TEST CASE STRING: %s\nCONVERTED VALUE: %f\n", testCase1, atof(testCase1));
  return 0;
}


double atof(char string[]) {
  double val, power;
  int i, sign;

  for (i = 0; isspace(string[i]); i++)
    ;
  
  sign = (string[i] == '-') ? -1: 1;

  if (string[i] == '+' || string[i] == '-')
    i++;

  for (val = 0.0; isdigit(string[i]); i++)
    val = 10 * val + (string[i] - '0');
  if (string[i] == '.')
    i++;
  for (power = 1.0; isdigit(string[i]); i++) {
    val = 10*val + (string[i] - '0');
    power *= 10;
  }

  return sign * val/power;
}

int atoi(char s[]) {
  int val;
  int i, sign;

  for (i = 0; isspace(s[i]); i++)
    ;
  sign = (s[i] == '-') ? -1: 1;

  if (s[i] == '+' || s[i] == '-')
    i++;

  for (val = 0; isdigit(s[i]); i++) 
    val = 10 * val + (s[i] - '0');

  return sign * val;
}
