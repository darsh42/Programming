#include "graphs.h"

int main() {
    graph G = {0, 0, NULL, NULL};
    char *verticies = "ABCDE";
    int vertex_count = 5;
    int edges[][2] = {{0, 1}, {1, 2}, {2, 3}, {4, 1}};
    int edge_count = 4;

    make_graph(&G, verticies, edges, vertex_count, edge_count);
    print_graph(&G);
    delete_vertex(&G, 'e');
    delete_edge(&G, edges[2]);
    print_graph(&G);
    free_graph(&G);
    return 0;
}
