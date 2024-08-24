#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STACKSIZE 1000
#define SYMBOL 1
#define FALSE 1
#define TRUE 0
#define MAXCHAR 1000    
#define MAXLINE 200     
#define FILENAME "text24.c"
#define OUTPUTFILE "commentlessEx23.c"


int fileRead(char fileArray[MAXLINE][MAXCHAR], int fileParam[MAXLINE], int maxchar, int maxline);


int checkBalance(char file[MAXLINE][MAXCHAR], int fileParam[MAXLINE], int len);


void fileWrite(char file[MAXLINE][MAXCHAR], int len);


int stackPush(int data);


int stackPop();


void stackView();


int stackTop;  
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


int fileRead(char fileArray[MAXLINE][MAXCHAR], int fileParam[MAXLINE], int maxline, int maxchar){
  int c;                                            
  int lines;                                        
  int chars;                                        

  FILE *filePtr;

  filePtr = fopen(FILENAME, "r");

  if (filePtr == NULL) {
    printf("Error, No file %s found!", FILENAME);
    exit(1);
  }


  for (lines = 0; lines < maxline-1; ++lines) {
    
    for (chars = 0; chars < maxchar-1 && (c = fgetc(filePtr)) != EOF && c != '\n'; ++chars) 
      fileArray[lines][chars] = c;
    
    
    if (c == EOF)
      maxline = lines + 1;
  
    if (c == '\n') {
      
      fileArray[lines][chars] = c;
    }
    
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


int stackPush(int data) {
  
  ++stackTop;
  stack[stackTop] = data;
  return stackTop;
}
