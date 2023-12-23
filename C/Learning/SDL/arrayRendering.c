#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdint.h>

SDL_Window *win;
SDL_Renderer *rend;
SDL_Texture *screen;

int INIT() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "[Error] arrayRendering.c => INIT(), cannot create SDL context, SDL_Error: %s", SDL_GetError());
        return 1;
    }

    if ((win = SDL_CreateWindow("ArrayRender", SDL_WINDOWPOS_CENTERED_MASK, SDL_WINDOWPOS_CENTERED_MASK, 640, 480, SDL_WINDOW_SHOWN)) == NULL) {
        fprintf(stderr, "[Error] arrayRendering.c => INIT(), cannot create SDL window, SDL_Error: %s", SDL_GetError());
        return 1;
    }

    if((rend = SDL_CreateRenderer(win, SDL_RENDERER_ACCELERATED, 0)) == NULL) {
        fprintf(stderr, "[Error] arrayRendering.c => INIT(), cannot create SDL renderer, SDL_Error: %s", SDL_GetError());
        return 1;
    }

    if ((screen = SDL_CreateTexture(rend, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, 160, 144)) == NULL) {
        fprintf(stderr, "[Error] arrayRendering.c => INIT(), cannot create SDL texture, SDL_Error: %s", SDL_GetError());
        return 1;
    }

    return 0;
}

int KILL() {
    SDL_DestroyTexture(screen);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

uint8_t display[144][160][3];

char pixels[160 * 144 * 3];
int pitch = 160 * 3;

void update_display(int colour_val) {
    for (int y = 0; y < 144; y++) {
        for (int x = 0; x < 160; x++) {
            display[y][x][0] = colour_val;
            display[y][x][1] = colour_val;
            display[y][x][2] = colour_val;
        }
    }
}

void update_screen() {
    return;
}

void Render() {
    SDL_UpdateTexture(screen, NULL, display, pitch);
    SDL_RenderClear(rend);
    SDL_RenderCopy(rend, screen, NULL, NULL);
    SDL_RenderPresent(rend);
}

int main() {
    if (INIT() != 0) {
        fprintf(stderr, "[Error] arrayRendering.c => main(), cannot initialize application");
        return 1;
    }

    for (uint8_t colour = 0;; colour++) {
        update_display(colour);
        update_screen();
        Render();
    }


    KILL();
    return 0;
}
