#include "edge.h"
#include "vertex.h"
#include "parameter.h"

struct parameter define_parameter
(
    int p0,
    int p1,
    int p2,
    struct edge e0,
    struct edge e1,
    struct edge e2,
    int area
)
{
    struct parameter equation;

    double factor = 1.0f / (2.0f * area);

    equation.a = (int) factor * (p0 * e0.a + p1 * e1.a + p2 * e2.a);
    equation.b = (int) factor * (p0 * e0.b + p1 * e1.b + p2 * e2.b);
    equation.c = (int) factor * (p0 * e0.c + p1 * e1.c + p2 * e2.c);
}

int evaluate_parameter
(
    struct parameter equation,
    int x,
    int y
)
{
    return equation.a * x + equation.b * y + equation.c * 255;
}
