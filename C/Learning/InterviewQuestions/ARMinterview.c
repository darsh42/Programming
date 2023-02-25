/* program that will print last char in generated array
 * #depending on the number the user provides.
 *
 * string[len]
 *
 * string generation:
 *  |--> 0 --> 1
 *  |--> 1 --> 2
 *  |--> * --> 0
 *
 * string size --> 2^n ==> n is the number provided by the user
 *
 * return string[-1]                                            */

#include <stdio.h>
#include <time.h>
#define ITERNUM 22

/* computes the base ^ power
 * INPUTS: --> base
 *         --> power
 *
 * OUTPUT: --> base ^ power                                     */
long long power(int base, int power);

/* function to time execution time 
 * INPUTS: --> function to be tested
 *
 * OUTPUT: --> execution 15:51                                  */
double timer(int (*func)(char *, int, int));



/* will generate the string based on the parameters specified
 *
 * string generation:
 *  |--> 0 --> 1
 *  |--> 1 --> 2
 *  |--> 2 --> 0
 *
 *  INPUTS: --> string location
 *
 *  OUTPUT: --> mutated string
 *                                                              */
int stringGen(char str[], int stringCurrentLen, int stringMaxLen);

int main() {
  printf("Time taken: %f\n", timer(stringGen));
  return 0;
}

/*==============================================================*/

double timer(int (*func)(char *, int, int)) {
  double start, end;
  start = clock();

  int i;
  long long stringMaxLen;
  int stringCurrentLen;
  char str[(stringMaxLen = power(2, ITERNUM - 1) + 1)];
  
  str[0] = '0';
  str[1] = '\0';


  stringCurrentLen = 2;

  for (i = 1; i < ITERNUM; ++i) 
    stringCurrentLen = func(str, stringCurrentLen, stringMaxLen);

  printf("%c\n", str[stringMaxLen - 2]);

  end = clock();
  return ((double) (end - start))/CLOCKS_PER_SEC;
}


long long power(int base, int power) {
  int i;
  long long result;
  
  result = 1;

  if (power <= -1)
    return -1;

  for (i = 0; i < power; ++i)
    result = result * base;
  
  return result;
}

int stringGen(char str[], int stringCurrentLen, int stringMaxLen) {
  int i;
  char appendStr[stringCurrentLen];

  for (i = 0; i < stringCurrentLen - 1 && i != stringMaxLen; ++i) {
    switch ((int) str[i]) {
      case('0'):
        appendStr[i] = '1';
        break;
      case('1'):
        appendStr[i] = '2';
        break;
      case('2'):
        appendStr[i] = '0';
        break;
      default:
        printf("ERROR!");
        return -1;
    }
  }
  
  for (i = stringCurrentLen - 1; i < 2*(stringCurrentLen - 1); ++i)
    str[i] = appendStr[i - (stringCurrentLen - 1)];
  
  str[i] = '\0';

  return 2*(stringCurrentLen) - 1;
}

