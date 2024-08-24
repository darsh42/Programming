#include <errno.h>
#include <CUnit/CUnit.h>
#include <string.h>
#include <stdio.h>

#define LINEBUFF 100
#define FILEBUFF 1000
#define FILELOCATION "../ROM/spaceInvaders.txt"


/*
   Read a line from a text file containing space invaders source code
   INPUTS: -> location

   OUTPUT: -> contents in a buffer
                                                                      */
int readFile( char buffer[FILEBUFF][LINEBUFF], int *maxFileLen, char *fileLocation ) {
  int lineLen = 0;
  FILE* ptr;

  ptr = fopen(fileLocation, "r");

  if (ptr == NULL) {
    fprintf(stderr, "fopen: %s", strerror(errno));
    return -1;
  }

  for(int i = 0; fgets(buffer[i], sizeof(buffer[i]), ptr) != NULL && i < FILEBUFF; i++, lineLen++)
    ;

  fclose(ptr);

  *maxFileLen = lineLen;

  return 0;
}


//    void readTest( void ) {
//      int fileLen;
//      char program[FILELEN][LINEBUFFER];
//
//      if ( readFile( program, &fileLen, FILELOCATION ) ) {
//
//      }
//
//      for (int i = 0; i < fileLen; i++)
//        printf("%s\n", program[i]);
//
//      CU_PASS("readFile() succeeded");
//    }
