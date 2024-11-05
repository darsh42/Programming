#include "edge.h"
#include "vertex.h"

/* ax + by + c line calculation */
struct edge define_edge( struct vertex v0 , struct vertex v1 ) 
{
    struct edge edge;

    edge.a = (int) v0.y - v1.y;
    edge.b = (int) v1.x - v0.x;
    edge.c = (int) -(edge.a * (v0.x + v1.x) + edge.b * (v0.y +v1.y)) / 2;
    
    edge.tie = edge.a != 0 ? edge.a > 0 : edge.b > 0;
}

/* evaluate point relative to triangle */
int evaluate_edge( struct edge equation , int x , int y )
{
    return equation.a * x + equation.b * y + equation.c;
}

/* test for if point is within triangle */
int test_edge( struct edge equation , int value )
{
    return (value > 0 || value == 0 && equation.tie);
}
