#include "sdl.h"
#include <SDL2/SDL_render.h>

SDL_Window *win;
SDL_Renderer *rend;
SDL_Texture *screen;

int sdl_init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "[Error]: sdl.c: SDL could not Initialize, SDL error %s\n", SDL_GetError());
        return 1;
    }

    if ((win = SDL_CreateWindow("GAMEBOY", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 160, 144, SDL_WINDOW_SHOWN)) == NULL) {
        fprintf(stderr, "[Error] sdl.c: SDL could not create window, SDL error %s\n", SDL_GetError());
        return 1;
    }

    if ((rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED)) == NULL) {
        fprintf(stderr, "[Error] sdl.c: SDL could not make renderer, SDL_error: %s\n", SDL_GetError());
        return 1;
    }

    if ((screen = SDL_CreateTexture(rend, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, 160, 144)) == NULL) {
        fprintf(stderr, "[Error] sdl.c: SDL could not make texture, SDL_error: %s\n", SDL_GetError());
        return 1;
    }

    return 0;
}

int sdl_kill() {
    SDL_DestroyTexture(screen);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

int sdl_render() {
    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
    SDL_RenderClear(rend);
    // Update the screen with display contents
    SDL_UpdateTexture(screen, NULL, get_display(), 160*3);
    // Copy the texture for rendering to screen
    SDL_RenderCopy(rend, screen, NULL, NULL);
    // Render the texture to the screen
    SDL_RenderPresent(rend);
    return 0;
}
