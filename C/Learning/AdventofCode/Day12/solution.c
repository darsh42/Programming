#include "solution.h"
#define WIDTH  133
#define HEIGHT 41

#define POS(point) WIDTH * point.y + point.x

FILE *fp;
char *source;
size_t bufsize;

typedef struct point {
    int x;
    int y;
} point_t;

int main(int argc, char *argv[]) {

    if (argc != 2) {fprintf(stderr, "[Usage]: ./solution <input_file.txt>\n"); return 1;}
    if ((fp = fopen(*++argv, "r")) == NULL) {fprintf(stderr, "[Error]: Could not open %s\n", *argv); return 1;}

    fseek(fp, 1, SEEK_END);
    bufsize = ftell(fp);
    fseek(fp, 1, SEEK_SET);
    source = malloc(sizeof(char) * bufsize + 1);

    fread(source, sizeof(char), bufsize, fp);
    fclose(fp);

    point_t start = {0, 0};

    for (point_t *p = &start; p->y < HEIGHT; p->x++) {
        if (p->x % WIDTH == 0 && p->x != 0) {
            p->x %= WIDTH;
            p->y++;
            printf("\n");
            continue;
        }
        int inx = POS((*p));

        if (source[inx] == '\n') continue;
        printf("%c", source[inx]);
    }

    start = (point_t) {22, 0};
    printf("%c", source[POS(start)]);

    return 0;
}
