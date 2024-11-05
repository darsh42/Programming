#ifndef PARAMETER_H
#define PARAMETER_H

#include "edge.h"
#include "vertex.h"

struct parameter {
    int a;
    int b;
    int c;
};

extern struct parameter
define_parameter
(
    int p0,
    int p1,
    int p2,
    struct edge e0,
    struct edge e1,
    struct edge e2,
    int area
);

extern int
evaluate_parameter
(
    struct parameter,
    int x,
    int y
);


#endif // PARAMETER_H
