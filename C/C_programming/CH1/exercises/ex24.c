/* Read a c program file and remove all comments from that file 
 *
 * PARTS:
 *  - READ file
 *  - COPY file into iterable (e.g array)
 *  - REMOVE comments 
 *  - OUTPUT to a file
 *
 *                                                                */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STACKSIZE 1000
#define SYMBOL 1
#define FALSE 1
#define TRUE 0
#define MAXCHAR 1000    // maximum number of characters in a line
#define MAXLINE 200     // maximum number of lines in a file
#define FILENAME "text24.c"
#define OUTPUTFILE "commentlessEx23.c"


/* Read file and store in array 'file_array'
 * - INPUT --> storage array 
 *         --> maximum char in line
 *         --> maximum num of lines         
 *
 * - OUTPUT --> Array fileParam 
 *          --> fileParam[0] = num hcar in each line
 *          --> fileParam[1] = num of lines in file  
 *          --> RETURNS length of fileArray         */
int fileRead(char fileArray[MAXLINE][MAXCHAR], int fileParam[MAXLINE], int maxchar, int maxline);


/* Remove all comments from file
 * - INPUT --> file
 *         --> fileParam (defined in fileRead) 
 *         --> location of commentless file
 *
 * - OUTPUT --> Array with commentless file  */
int checkBalance(char file[MAXLINE][MAXCHAR], int fileParam[MAXLINE], int len);


/* Writes contents of array to newfile
 * INPUT --> array containing data
 *       --> fileParam specifying the layout
 *
 * OUTPUT --> new file                       */
void fileWrite(char file[MAXLINE][MAXCHAR], int len);


/* Will push data onto the stack 
 * INPUTS: --> Stack 
 *         --> stackTop
 *         --> data             
 *
 * OUTPUT: --> adds data to stack  
 *         --> int RETURN status  
 *            |--> 0 normal return
 *            |--> 1 error return*/
int stackPush(int data);


/* Will pop(return top value) from stack
 * INPUTS: --> STACK 
 *         --> stackTop
 *
 * OUTPUT: --> top value in stack   */
int stackPop();


/* Will display contents in the stack
 * INPUTS: --> STACK
 *         --> stcakTop
 *
 * OUTPUT: --> print stack          */
void stackView();


int stackTop;  // Top of the stack
int stack[STACKSIZE];

int main() {
  int len;
  char fileArray[MAXLINE][MAXCHAR];
  int fileParam[MAXLINE];
  
  len = fileRead(fileArray, fileParam, MAXLINE, MAXCHAR);

  if (checkBalance(fileArray, fileParam, len) != TRUE) 
    return 1;

  return 0;
}

/* ====================================FUNCTION=========================================== */

int fileRead(char fileArray[MAXLINE][MAXCHAR], int fileParam[MAXLINE], int maxline, int maxchar){
  int c;                                            // contains each character of file
  int lines;                                        // will count number of lines in file
  int chars;                                        // will count number of chars in each file

  FILE *filePtr;

  filePtr = fopen(FILENAME, "r");

  if (filePtr == NULL) {
    printf("Error, No file %s found!", FILENAME);
    exit(1);
  }

  for (lines = 0; lines < maxline-1; ++lines) {
    // iterates over each char in line
    for (chars = 0; chars < maxchar-1 && (c = fgetc(filePtr)) != EOF && c != '\n'; ++chars) 
      fileArray[lines][chars] = c;
    
    // when the EOF is reached terminates the loop 
    if (c == EOF)
      maxline = lines + 1;
  
    if (c == '\n') {
      // places back the semicolons into the output
      fileArray[lines][chars] = c;
    }
    // adds the char num for each corresponding line
    fileParam[lines] = chars;
  
  }
  
  ++lines;
  fileArray[lines][0] = '\0';
  
  fclose(filePtr);
  return lines;
}

void fileWrite(char file[MAXLINE][MAXCHAR], int len) {
  int lines;

  FILE *filePtr;

  filePtr = fopen(OUTPUTFILE, "w");

  if (filePtr == NULL) {
    printf("Error!");
    exit(1);
  }
  
  for (lines = 0; lines < len; ++lines)
    fprintf(filePtr, "%s", file[lines]);

  fclose(filePtr);
}

/*============================SYNTAX CHECK================================*/

int checkBalance(char file[MAXLINE][MAXCHAR], int fileParam[MAXLINE], int len) {
  int lines;
  int chars;
  int data;
  int nextData;
  int currentStackTop;

  currentStackTop = stackTop;

  for (lines = 0; lines < len ; ++lines) {

    for (chars = 0; chars < fileParam[lines] ; ++chars) {

      data = file[lines][chars];
      nextData = file[lines][chars + 1];

      if ((data == '{') ^ (data == '[') ^ (data == '(') ^ (data == *"'") ^ (data == '"') ^ (data == '/' &&  nextData == '*'))
        stackPush(SYMBOL);
      else if ((data == '}') ^ (data == ']') ^ (data == ')') ^ (data == *"'") ^ (data == '"') ^ (data == '*' && nextData == '/'))
        stackPop();
      else if (data)

      else 
        continue;

    }
  }
  
  if (stackTop != currentStackTop)
    return 1;

  return 0;
}

/*=================STACK====================*/


int stackPush(int data) {
  // under normal conditions
  ++stackTop;
  stack[stackTop] = data;
  return stackTop;
}


int stackPop() {
  int out;

  out = stack[stackTop];
  stack[stackTop] = 0;
  --stackTop;
  return out;
}


void stackView() {
  int i;

  for (i = 0; i < stackTop + 1; ++i) {
    if (i == stackTop) {
      printf("%d <-- stackTop\n", stack[i]);
      continue;
    }
    printf("%d\n", stack[i]);
  }
}

