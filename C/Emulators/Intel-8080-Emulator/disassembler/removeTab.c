#include "readSource.c"
#include <errno.h>

#define FILEBUFF 1000
#define LINEBUFF 1000
#define SPCNUM 4


int main(int argc, char **argv) {

  if (argc == 0) {
    printf("No tab file and destiantion file specified\n");
    return -1;
  }

  FILE* ptr;
  if ((ptr = fopen(argv[2], "w+")) == NULL) {
    fprintf(stderr, "fopen: %s\n", strerror(errno));
    return -1;
  }

  char resFile[FILEBUFF][LINEBUFF];

  int fileLen;
  int currentCharNum = 0;
  char c;
  char file[FILEBUFF][LINEBUFF];

  readFile(file, &fileLen, argv[1]);

  for (int i = 0; i < fileLen; i++) {
    for (int origCharNum = 0; (c = file[i][origCharNum]) != '\n'; origCharNum++, currentCharNum++) {
      if (c != '\t') {
        resFile[i][currentCharNum] = c;
        continue;
      }

      for (int tmp = currentCharNum; currentCharNum < tmp + SPCNUM; currentCharNum++)
        resFile[i][currentCharNum] = ' ';
    }
    resFile[i][currentCharNum] = '\n';
  }


  for (int i = 0; i< fileLen; i++)
    printf("%s", resFile[i]);

  fclose(ptr);

  return 0;
}
