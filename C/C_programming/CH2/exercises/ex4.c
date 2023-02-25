/* program that removes common chars between 2 strings */

#include <stdio.h>
#define MAXLEN 1000

/* removes common chars between strings and changes
 * string 1 to store only remaining chars
 *
 * INPUTS: --> targetString
 *         --> comparisionString
 *
 * OUTPUT: --> mutates targetString to only
 *              contain non-common chars            */
void removeCommon(char targetString[], char comparisionString[], int targetStringLen, int comparisionStringLen);


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
  
  removeCommon(targetString, comparisionString, targetStringLen, comparisionStringLen);

  printf("%s\n", targetString);
  return 0;
}


int getString(char stringStore[MAXLEN]) {
  int c, i;

  for (i = 0; i < MAXLEN - 1 && (c = getchar()) != EOF && c != '\n'; ++i) {
    stringStore[i] = c;
  }

  return i;
}


void removeCommon(char targetString[], char comparisionString[], int targetStringLen, int comparisionStringLen) {
  int i;
  int index = 0;

  if (targetStringLen <= 1) {
    printf("No target string found!\n");
    return;
  }

  if (targetStringLen <= 1) {
    printf("No comparision string found!\n");
    return;
  }

  for (i = 0; i < targetStringLen; ++i) {
    if (i < comparisionStringLen && targetString[i] == comparisionString[i])
      continue;
    targetString[index++] = targetString[i];
  }

  targetString[index] = '\0';
}
