/* Day 2 of Advent of code, now the elves are playing rock paper scissors to 
 * decide on tent placement in the camp                                     
 *
 * read input from input.txt
 *  key:
 *    |-> elf:A, you:X - ROCK    --> 1 pt(s)
 *    |-> elf:B, you:Y - PAPER   --> 2 pt(s)
 *    |-> elf:C, you:Z - SCISSOR --> 3 pt(s)  
 *                                                                            
 * compute result of match
 *  |-> calulate score
 *
 * compute total score
 *  |-> sum of all matches  
 *  |-> Win  --> 6 pt(s)
 *  |-> Draw --> 3 pt(s)
 *  |-> Loss --> 0 pt(s)
 *                                                                              
 *           E     P     
 * ROCK      0     7     
 * PAPER     1     8     
 * SCISSOR   2     9     
 *         
 *                   S
 *                   C
 *               P   I
 *           R   A   S
 *           O   P   S
 *           C   E   O
 *           K   R   R
 *        | ==========
 * ROCK   | 23  24* 25 
 * PAPER  | 24  23  22*
 * SCISSOR| 25* 22  23
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


  /* check test.txt for reasoning for computations */
  for (int i = 0; i <= matchNum; i++) {
    switch (matches[i][PLAYER] - matches[i][ELF]) {
      // computes when it is a tie
      case(23):
        // check if ROCK, if not checks if PAPER, if not assumes SCISSOR, then adds score for each case
        score += (matches[i][PLAYER] == 'X') ? DRAW + ROCK: (matches[i][PLAYER] == 'Y') ? DRAW + PAPER: DRAW + SCISSOR;
        break;

      case(21):
        score += WIN + ROCK;
        break;
      // computes when match is between PAPER and SCISSOR
      case(22):
        // if player is scissor then they win, else they lose
        score += (matches[i][PLAYER] == 'X') ? LOSS + ROCK: LOSS + PAPER;
        break;
      // computes when match is PAPER and ROCK
      case(24):
        // if player is PAPER they win else they lose
      score += (matches[i][PLAYER] == 'Y') ? WIN + PAPER: WIN + SCISSOR;
        break;
      // computes when match is ROCK and SCISSOR
      case(25):
        // if player is ROCK they win, else they lose
        score += LOSS + SCISSOR;
        break;
    }
  }

  return score;
}
