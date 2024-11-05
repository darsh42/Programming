#ifndef EDGE_EQUATION_H
#define EDGE_EQUATION_H

#include "vertex.h"

/* edge equation struct */
struct edge {
    int a;  
    int b;  
    int c;

    int tie;
};

/* creates an edge equation based two verticies */
extern struct edge define_edge
(
    struct vertex v0, 
    struct vertex v1
);

/* evaluates the edge equation */
extern int evaluate_edge
(
    struct edge equation,
    int x,
    int y
);

/* test whether an edge is within polygon */
extern int test_edge
( 
    struct edge equation,
    int value 
);

#endif // EDGE_EQUATION_H
