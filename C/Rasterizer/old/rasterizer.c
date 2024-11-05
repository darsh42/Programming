#include <math.h>
#include <stdio.h>
#include <stdint.h>

uint16_t framebuffer[32][32] = {};

void render(void) {
    for (int i = 0; i < 32; i++) {
        for(int j = 0; j < 32; j++)
            printf("%c ", (framebuffer[i][j]> 0) ? '1': ' ');
        printf("\n");
    }
}

int edge_function(int ax, int ay, int bx, int by, int cx, int cy) {
    return ((cx - ax) * (by - ay) - (cy - ay) * (bx - ax) >= 0);
}

int main(int argc, char **argv) {
    if (argc < 4) {
        fprintf(stderr, "[Usage]: ./triangle <v1.x> <v1.y> <v2.x> <v2.y> <v3.x> <v3.y>\n");
        return -1;
    }

    int verticies[6] = {};
    
    for (int i = 0; i < 6 && argv++; i++) {
         sscanf(*argv, "%d", &verticies[i]);
    }


    uint8_t bbmin_x = INFINITY; uint8_t bbmax_x = -INFINITY;
    uint8_t bbmin_y = INFINITY; uint8_t bbmax_y = -INFINITY;

    for (int i = 0; i < 3; i++) {
        if (verticies[i + 0] < bbmin_x) bbmin_x = verticies[i + 0];
        if (verticies[i + 1] < bbmin_y) bbmin_y = verticies[i + 1];
        if (verticies[i + 0] > bbmax_x) bbmax_x = verticies[i + 0];
        if (verticies[i + 1] > bbmax_y) bbmax_y = verticies[i + 1];
    }

    framebuffer[bbmin_x][bbmin_y] = 1;
    framebuffer[bbmax_x][bbmin_y] = 1;
    framebuffer[bbmin_x][bbmax_y] = 1;
    framebuffer[bbmax_x][bbmax_y] = 1;

    for (int i = bbmin_y; i < bbmax_y; i++) {
        for (int j = bbmin_x; j < bbmax_x; j++) {
            int inside = 1;

            inside &= edge_function(verticies[0 + 0], verticies[0 + 1], verticies[1 + 0], verticies[1 + 1], i, j);
            inside &= edge_function(verticies[1 + 0], verticies[1 + 1], verticies[2 + 0], verticies[2 + 1], i, j);
            inside &= edge_function(verticies[2 + 0], verticies[2 + 1], verticies[0 + 0], verticies[0 + 1], i, j);
            
            if (inside) {
                framebuffer[i][j] = 1;
            }
        }
    }

    // line(verticies[1 + 0], verticies[1 + 1], verticies[0 + 0], verticies[0 + 1]);
    // line(verticies[0 + 0], verticies[0 + 1], verticies[2 + 0], verticies[2 + 1]);
    // line(verticies[1 + 0], verticies[1 + 1], verticies[2 + 0], verticies[2 + 1]);

    render();

    return 0;
}
