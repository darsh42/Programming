/* Advent of Code: Day 1
 *
 * Calorie counter for elves on an expedition
 *
 * INPUTS:
 *  Data from the input.txt file in this directory
 *
 *  OUTPUT:
 *   find the elf carrying the most calories
 *
 *
 * NOTES:
 *  - data separated by newline
 *  - data not separated by newline corresponds to
 *    a single elves calories
 *  - keep check on the maximum number of calories
 *    an elf posseses
 *                                                    */

#include <stdio.h>
#include <stdlib.h>
#define ELFMAX 300
#define BUFFSIZE 100
#define INPUTFILE "input.txt"


/*
 * Will read data and transform into suitable format
 * and store data in array, mutate the maximum
 * calories variable and return the number of elves
 *                                                    */
int dataRead(int elves[]);



/* function will find elf who carries maximum number 
 * of calories and return how many calories they 
 * carried                                            */
int findMax(int list[], int listLen);


int main() {
  int elves[ELFMAX];
  int elfNum = dataRead(elves);
  int maximum = findMax(elves, elfNum);

  printf("Elf number: %d\nMaximum calories: %d\n", maximum, elves[maximum]);

  return 0;
}

int dataRead(int elves[]) {
  int elfNum = 0;
  int result = 0;
  char currentNum[BUFFSIZE];

  FILE *fp;
  if ((fp = fopen(INPUTFILE, "r")) == NULL) {
    printf("No file %s found!", INPUTFILE);
    exit(1);
  }


  while (fgets(currentNum, BUFFSIZE, fp) != NULL) {
    if ((currentNum[0] != '\n')) {
      result += atoi(currentNum);
      continue;
    }

    elves[elfNum] = result;
    result = 0;
    elfNum++;
  }

  fclose(fp);
  return elfNum;
}


int findMax(int list[], int listLen) {
  int currentMax = 0;
  int index;

  for (int i = 0; i<listLen; i++) {
    if (currentMax < list[i]) {
      currentMax = list[i];
      index = i;
    }
  }

  return index;
}
