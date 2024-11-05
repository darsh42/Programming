#include "one-pass.h"

struct {
    int LC;

    
} Assembler;

OP TABLE[] = {
    (OP) {"LOD", LOD, true},
    (OP) {"STO", STO, true},
    (OP) {"ADD", ADD, true},
    (OP) {"BZE", BZE, false},
    (OP) {"BNE", BNE, false},
    (OP) {"BRA", BRA, false},
    (OP) {"INP", INP, true},
    (OP) {"OUT", OUT, false},
    (OP) {"CLA", CLA, false},
    (OP) {"HLT", HLT, false}
};

int main(int argc, char **argv) {
    if (argc != 2) {
      fprintf(stderr, "[Usage]: ./one-pass <assembly_source>\n");
      return -1;
    }
    char *source;
    size_t source_len;
    FILE *fp = fopen(argv[1], "r");
    fseek(fp, 0, SEEK_END);
    source_len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    if ((source = malloc(sizeof(char) * source_len)) == NULL) {
        fprintf(stderr, "[Error]: Cannot allocate space for source file\n");
        return -1;
    }
    if (fread(source, source_len, 1, fp) == 0) {
        fprintf(stderr, "[Error]: Cannot read source file\n");
        return -1;
    }
    lex(source, source_len);
    printTokens();
    free(source);
    fclose(fp);
    return 0;
}
