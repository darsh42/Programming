#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>
#include <time.h>

#define MAP_H 25
#define MAP_W 50

#define MAP_COORDS(p) p.y * MAP_W + p.x
#define ADD_PNT(a, b) (pnt_t) {a.x + b.x, a.y + b.y}

#define APP_SYM 2
#define SEG_SYM 1
#define SPC_SYM 0

typedef struct point {
    int x, y;
} pnt_t;

typedef struct segment {
    struct segment *next;
    pnt_t pos;
    pnt_t prev;
} seg_t;

#endif // MAIN_H_INCLUDED
