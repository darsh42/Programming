#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include "../include/renderer.h"

#define SDL_CHECK(expr) {assert(expr != NULL);}

SDL_Window *window;
SDL_GLContext context;
renderer_t renderer;

int main(int argc, char **argv) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CHECK((window = SDL_CreateWindow("renderer", 0, 0, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL)));

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    context = SDL_GL_CreateContext(window);
    gladLoadGLLoader(SDL_GL_GetProcAddress);
    glViewport(0, 0, 640, 480);

    renderer_init(&renderer, "generic");

    for(;;) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        renderer_start_frame(&renderer);
        renderer_push_triangle
        (
                &renderer,
                -0.9313123f, -0.95f, 0.0f,
                 0.5f, -0.5f, 0.0f,
                 0.0f,  0.5f, 0.0f,
                 1.0f,  1.0f, 1.0f,
                 1.0f,  1.0f, 1.0f,
                 1.0f,  1.0f, 1.0f
        );
        renderer_end_frame(&renderer);
        SDL_GL_SwapWindow(window);
    }

    renderer_destroy(&renderer);
}
