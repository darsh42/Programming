#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct GraphEdge graph_edge;
typedef struct GraphNode graph_node;

// main Graph wrapper
typedef struct Graph {
    int vertex_count;
    int edge_count;

    graph_node *verticies;
    graph_edge *edges;
} graph;

int insert_vertex(graph *G, char vertex);
int delete_vertex(graph *G, char vertex);
int insert_edge(graph *G, int *edge);
int delete_edge(graph *G, int *edge);
int make_graph(graph *G, char *verticies, int edges[][2], int vertex_num, int edge_num);
int free_graph(graph *G);
int print_graph(graph *G);

#endif // MAIN_H_INCLUDED
