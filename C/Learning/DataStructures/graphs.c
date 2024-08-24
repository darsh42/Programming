#include "graphs.h"

// wrapper for each edge of the graph
// - stores the index of a vertex
//   in the graph, in start and end
typedef struct GraphEdge {
    int start;
    int end;
} graph_edge;

// wrapper for each vertex of the graph
typedef struct GraphNode {
    char name;
} graph_node;

int insert_vertex(graph *G, char vertex) {
    // Allocate space for vertex
    G->vertex_count++;
    if ((G->verticies = realloc(G->verticies, G->vertex_count * sizeof(graph_node))) == NULL) {
        fprintf(stderr, "[Error] graph.c: insert_vertex() => Cannot allocate space for new vertex\n");
        return 1;
    }

    // Set new vertex name
    G->verticies[G->vertex_count - 1].name = vertex;
    return 0;
}

int insert_edge(graph *G, int *edge) {
    // Allocate space for new edge
    G->edge_count++;
    if ((G->edges = realloc(G->edges, G->edge_count * sizeof(graph_edge))) == NULL) {
        fprintf(stderr, "[Error] graph.c: insert_vertex() => Cannot allocate space for new edge\n");
        return 1;
    }

    // Set new edge
    G->edges[G->edge_count - 1].start = *(edge + 0);
    G->edges[G->edge_count - 1].end   = *(edge + 1);

    return 0;
}

int delete_vertex(graph *G, char vertex) {
    int vertex_index = -1;

    // loop through each vertex
    for (int i = 0; i < G->vertex_count; i++) {

        // if vertex found
        if (G->verticies[i].name == vertex) {
            vertex_index = i;
            break;
        }
    }

    // vertex not found in graph
    if (vertex_index == -1) return 1;

    memmove(G->verticies + vertex_index, G->verticies + vertex_index + 1, G->vertex_count - vertex_index - 1);

    G->vertex_count--;

    if ((G->verticies = realloc(G->verticies, G->vertex_count * sizeof(graph_node))) == NULL) {
        fprintf(stderr, "[Error] graph.c: delete_vertex() => Cannot de-allocate deleted vertex\n");
        return -1;
    }

    return 0;
}

int delete_edge(graph *G, int *edge) {
    int edge_index = -1;

    for (int i = 0; i < G->edge_count; i++) {
        graph_edge cur_edge = G->edges[i];

        if (cur_edge.start == edge[0] && cur_edge.end == edge[1]) {
            edge_index = i;
            break;
        }
    }

    if (edge_index == -1) return 1;

    memmove(G->edges + edge_index, G->edges + edge_index + 1, 2 * (G->edge_count - edge_index - 1) * sizeof(int));
    G->edge_count--;

    if ((G->edges = realloc(G->edges, G->edge_count * sizeof(graph_edge))) == NULL) {
        fprintf(stderr, "[Error] graph.c: delete_edge() => Cannot de-allocate deleted vertex\n");
        return -1;
    }

    return 0;
}

int make_graph(graph *G, char *verticies, int edges[][2], int vertex_num, int edge_num) {
    for (int i = 0; i < vertex_num; i++)
        insert_vertex(G, verticies[i]);

    for (int i = 0; i < edge_num; i++)
        insert_edge(G, edges[i]);

    return 0;
}

int free_graph(graph *G) {
    free(G->verticies);
    free(G->edges);
    return 0;
}

int print_graph(graph *G) {
    printf("----------------------------------\n");
    printf("Graph display\n");

    printf("Vertex:(name, index)\n");
    for (int i = 0; i < G->vertex_count; i++)
        printf("(%c, %d)\n", G->verticies[i].name, i);

    printf("Edge:([start, end], index)\n");
    for (int i = 0; i < G->edge_count; i++)
        printf("([%d, %d], %d)\n", G->edges[i].start, G->edges[i].end, i);

    printf("----------------------------------\n");
    return 0;
}

int DFS_graph(graph *G, char vertex) {

}
