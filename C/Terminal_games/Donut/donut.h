#ifndef DONUT_H_
#define DONUT_H_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ncurses.h>


typedef struct point {
    double x;
    double y;
    double z;
} point;

typedef struct shape {
    point vertex;
    point *next_vertex;
    point *prev_vertex;
} shape;

#endif // DONUT_H_
