#include <SDL2/SDL.h>
#include "renderer.h"

struct System   system;
struct Renderer renderer;

int main( void ) {
    /* create window and set opengl attributes */
    SDL_CHECK_RET(SDL_Init(SDL_INIT_VIDEO));
    SDL_CHECK_PTR
    (
        (system.window = SDL_CreateWindow
            (
                "renderer-textured",
                system.x, system.y,
                system.w, system.h,
                system.flags
            )
        )
    );

    SDL_CHECK_RET(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,  1));
    SDL_CHECK_RET(SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,   16));

    SDL_CHECK_RET(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3));
    SDL_CHECK_RET(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3));
    SDL_CHECK_RET(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE));
    
    /* create opengl context based on attributes */
    SDL_CHECK_RET((system.context = SDL_GL_CreateContext(system.window)));
    
    while (window_not_closed()) {
        while (event = new_event()) {
            handle_event(event);
        }

        update_scene();

        draw_graphics();
        present_graphics();
    }

    return 0;
}
