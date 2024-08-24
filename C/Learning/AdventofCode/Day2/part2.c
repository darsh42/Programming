/* Day 2 of Advent of code, now the elves are playing rock paper scissors to 
 * decide on tent placement in the camp                                     
 *
 * read input from input.txt
 *  key:
 *    |-> elf:A, - ROCK    --> 1 pt(s)
 *    |-> elf:B, - PAPER   --> 2 pt(s)
 *    |-> elf:C, - SCISSOR --> 3 pt(s)  
 *                                                                            
 * compute result of match
 *  |-> calulate score
 *
 * compute total score
 *  |-> sum of all matches  
 *  |-> X -  Win  --> 6 pt(s)
 *  |-> Y -  Draw --> 3 pt(s)
 *  |-> Z -  Loss --> 0 pt(s)
 *                                                                              
 *                                                                              */

#include <stdio.h>
#define BUFFSIZE 50
#define FILELEN 3000
#define INPUT "input.txt"

#define PLAYER 2
#define ELF 0

#define ROCK 1
#define PAPER 2
#define SCISSOR 3

#define LOSS 0
#define DRAW 3
#define WIN 6



/* reads file specified and returns length of array */
int readFile(char matches[FILELEN][BUFFSIZE], int fileLen);

/* will compute total score and return it */
int matchResult(char matches[FILELEN][BUFFSIZE], int matchNum);

int computeLoss(char opponentPlay);

int computeDraw(char opponentPlay);

int computeWin(char opponentPlay);


int main() {
  char matches[FILELEN][BUFFSIZE];
  int matchNum = readFile(matches, FILELEN);
  int result = matchResult(matches, matchNum);
  printf("Max score: %d\n", result);

  return 0;
}


int readFile(char matches[FILELEN][BUFFSIZE], int fileLen) {
  int i;
  FILE* fp;

  fp = fopen(INPUT, "r");

  if (fp == NULL) {
    printf("No file %s\n", INPUT);
    return -1;
  }

  for (i = 0; fgets(matches[i], BUFFSIZE, fp) != NULL && i < fileLen; i++)
    ;

  fclose(fp);
  return i;
}

int matchResult(char matches[FILELEN][BUFFSIZE], int matchNum) {
  int score = 0;

  for (int i = 0; i <= matchNum; i++) {
    switch (matches[i][PLAYER]) {
      case('X'):
        score += computeLoss(matches[i][ELF]);
        break;
      case('Y'):
        score += computeDraw(matches[i][ELF]);
        break;
      case('Z'):
        score += computeWin(matches[i][ELF]);
        break;
      
      }
    }

  return score;
}


int computeLoss(char opponentPlay) {
  switch (opponentPlay) {
    case('A'):
      return LOSS + SCISSOR;
    case('B'):
      return LOSS + ROCK;
    case('C'):
      return LOSS + PAPER;
  }
}

int computeDraw(char opponentPlay) {
  switch (opponentPlay) {
    case('A'):
      return DRAW + ROCK;
    case('B'):
      return DRAW + PAPER;
    case('C'):
      return DRAW + SCISSOR;
  }
}

int computeWin(char opponentPlay) {
  switch (opponentPlay) {
    case('A'):
      return WIN + PAPER;
    case('B'):
      return WIN + SCISSOR;
    case('C'):
      return WIN + ROCK;
  }
}
