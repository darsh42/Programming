#include "readSource.c"
#include <stdbool.h>

#define INS_NUM 300
#define INS_FILE "instructions.txt"

struct Flags {
  bool S;  // sign flag
  bool Z;  // zero flag
  bool AR; // half-carry flag
  bool P;  // parity flag
  bool C;  // carry flag
};

struct Ins {
  int opcode;
  char *ins;
  int size;
  struct Flags flags;
};

struct Ins genIns(char *data);

struct Ins InstructionSet[INS_NUM];

int main( int argc, char **argv ) {
  char file[1000][100];
  int fileLen;
  FILE* ptr;

  if ((ptr = fopen(INS_FILE, "r")) == NULL) {
    return -1;
  }

  if (readFile( file, &fileLen, INS_FILE ) != 0)
    return -1;

  for (int i = 0; i < fileLen; i++)
    InstructionSet[i] = genIns(file[i]);
}



struct Ins genIns(char *data) {
  int index = 0;
  char param[10];
  char params[20][11];

  for (int i, j = 0; data[i] != '\0'; i++, j++) {

    if (data[i] != '\t') {
      param[j] = data[i];
      continue;
    }

    param[j] = '\0';
    strcpy(params[index++], param);
  }

  struct Ins ins;
  ins.opcode = atoi(params[0]);
  ins.ins = params[1];
  ins.size = atoi(params[3]);

}
