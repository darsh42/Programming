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
#define FALSE 0
#define TRUE 1
#define MAXCHAR 1000    // maximum number of characters in a line
#define MAXLINE 200     // maximum number of lines in a file
#define FILENAME "ex24.c"
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
void removeComments(char file[MAXLINE][MAXCHAR], char commentlessFile[MAXLINE][MAXCHAR], int fileParam[MAXLINE], int len);


/* Writes contents of array to newfile
 * INPUT --> array containing data
 *       --> fileParam specifying the layout
 *
 * OUTPUT --> new file                       */
void fileWrite(char file[MAXLINE][MAXCHAR], int len);


int main() {
  char fileArray[MAXLINE][MAXCHAR];
  char commentlessFile[MAXLINE][MAXCHAR];
  int fileParam[MAXLINE];
  int len;

  len =fileRead(fileArray, fileParam, MAXCHAR, MAXLINE);
  removeComments(fileArray, commentlessFile, fileParam, len);
  fileWrite(commentlessFile, len);
  return 0;
}

int fileRead(char fileArray[MAXLINE][MAXCHAR], int fileParam[MAXLINE], int maxchar, int maxline){
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

void removeComments(char file[MAXLINE][MAXCHAR], char commentlessFile[MAXLINE][MAXCHAR], int fileParam[MAXLINE], int len) {
  int multiline;
  int lines;
  int chars;

  multiline = FALSE;
  
  for (lines = 0; lines < len ; ++lines) {
    for (chars = 0; chars < fileParam[lines] ; ++chars) {
      if (file[lines][chars] == *"/" && file[lines][chars + 1] == *"/") {
        break;
      }
      else if (file[lines][chars] == *"/" && file[lines][chars + 1] == *"*") {
        multiline = TRUE;
        continue;
      }
      else if (file[lines][chars] == *"*" && file[lines][chars + 1] == *"/") {
        multiline = FALSE;
        ++chars;
      }
      else if (multiline == TRUE) {
        continue;
      }
      
      commentlessFile[lines][chars] = file[lines][chars];
    }
    commentlessFile[lines][chars] = '\n';
  }
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
