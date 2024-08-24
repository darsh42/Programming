/* program that removes common chars between 2 strings */

#include <stdio.h>
#define MAXLEN 1000

/* returns string when first common caharacter is
 * found between 2 strings
 *
 * INPUTS: --> targetString
 *         --> comparisionString
 *
 * OUTPUT: --> mutates targetString to only
 *             contain first non-common caharacters    */
int strbrk(char targetString[], char comparisionString[], int targetStringLen, int comparisionStringLen);


/* Reads user input and stores as string,
 * returns length of string
 *
 * INPUTS: --> stringStore[MAXLEN]
 *
 * OUTPUT: --> length of string*/
int getString(char stringStore[MAXLEN]);

int main() {
  
  char targetString[MAXLEN];
  char comparisionString[MAXLEN];
  int targetStringLen = getString(targetString);
  int comparisionStringLen = getString(comparisionString);
  
  int res = strbrk(targetString, comparisionString, targetStringLen, comparisionStringLen);

  printf("%s\n", targetString);
  return res;
}


int getString(char stringStore[MAXLEN]) {
  int c, i;

  for (i = 0; i < MAXLEN - 1 && (c = getchar()) != EOF && c != '\n'; ++i) {
    stringStore[i] = c;
  }

  return i;
}


int strbrk(char targetString[], char comparisionString[], int targetStringLen, int comparisionStringLen) {
  int i, j;
  int index = 0;

  if (targetStringLen <= 1) {
    printf("No target string found!\n");
    return -1;
  }

  if (targetStringLen <= 1) {
    printf("No comparision string found!\n");
    return -1;
  }

  for (i = 0; i < targetStringLen; ++i) {
    for (j = 0; j < comparisionStringLen; ++j) {
      if (targetString[i] == comparisionString[j]) {
        targetString[index] = '\0';
        return -1;
      }
    }
    targetString[index++] = targetString[i];
  }

  targetString[index] = '\0';

  return 0;
}
