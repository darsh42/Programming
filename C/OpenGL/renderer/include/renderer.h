#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../deps/glad/include/glad/glad.h"

#define MAX_TRIANGLES 1024
#define MAX_VERTICIES 3 * MAX_TRIANGLES

typedef struct VERTEX {
    float x, y, z; // position
    float r, g, b; // colour
} vertex_t;

typedef struct RENDERER {
    GLuint vao;
    GLuint vbo;
    GLuint shader;

    vertex_t render_vertcies[MAX_VERTICIES];
    GLuint triangle_count;
} renderer_t;

void renderer_init(renderer_t *r, const char *f);
void renderer_destroy(renderer_t *r);
void renderer_start_frame(renderer_t *r);
void renderer_end_frame(renderer_t *r);
void renderer_push_triangle
(
    renderer_t *r,
    float x1, float y1, float z1,
    float x2, float y2, float z2,
    float x3, float y3, float z3,
    float r1, float b1, float g1,
    float r2, float b2, float g2,
    float r3, float b3, float g3
);
