#ifndef RENDERER_H_
#define RENDERER_H_

struct vertex {
    int x, y, z;
};

struct triangle {
    struct vertex verticies[3];
};

struct vertex_matrix {
    int            size;
    struct vertex *rows;
};

#define W      640
#define H      480
#define ASPECT H/W

#endif // RENDERER_H_
