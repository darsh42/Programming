#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED

#include <assert.h>
#include <SDL2/SDL.h>

#define SDL_CHECK_PTR(expr) {assert(expr != NULL);}
#define SDL_CHECK_RET(expr) {assert(expr ==    0);}

struct Vertex {
    uint32_t x, y;
    uint32_t r, g, b;
    uint32_t tex_x, tex_y;
};

struct System {
    SDL_Window   *window;   /* window         */
    SDL_GLContext context;  /* opengl context */

    uint32_t w, h;          /* window width, height */
    uint32_t x, y;          /* window x pos, y pos  */

    uint32_t flags;         /* window flags */
};

struct Renderer {
    GLuint vbo, vao;
    GLuint 

    uint32_t       vertex_count;
    struct Vertex *verticies;
};

#endif // RENDERER_H_INCLUDED
