#include "solution.h"

FILE *fp;
char *line;
size_t bufsize = 30;

int main(int argc, char *argv[]) {

    if (argc != 2) {fprintf(stderr, "[Usage]: ./solution <input_file.txt>\n"); return 1;}
    if ((line = malloc(bufsize * sizeof(char))) == NULL) {fprintf(stderr, "[Error]: Malloc problem\n"); return 1;}
    if ((fp = fopen(*++argv, "r")) == NULL) {fprintf(stderr, "[Error]: Could not open %s\n", *argv); return 1;}

    while (getline(&line, &bufsize, fp) >= 0) {

    }

    return 0;
}
