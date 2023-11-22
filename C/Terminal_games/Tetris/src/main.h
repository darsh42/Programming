#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>
#include <time.h>

#define BLK_HGHT 30
#define BLK_WDTH 19
#define ADD_PNT(a, b)                                                          \
  { a.x + b.x, a.y + b.y }
#define MAP_PNT(a, b)                                                          \
  { a.x + b.x, a.y - b.y }
#define CMP_PNT(a, b)                                                          \
  { a.x - b.x, a.y - b.y }
#define MATRIX_MUL(M, a)                                                       \
  {M[0].x * a.x + M[0].y * a.y, M[1].x * a.x + M[1].y * a.y}


/*
** SHAPES:
** - 4 block square
** - 4 block L
** - 4 block line
** - 4 block S/Z
*/
#define SHAPE_VARIANTS 7
#define SHAPE_FILE "shapes.txt"

typedef struct point { int x; int y; } pnt_t;

typedef struct shape {
    int name;
    pnt_t pos;
    pnt_t prev;

    int block_num;
    pnt_t blocks[4];

} shape_t;

#endif // MAIN_H_INCLUDED
