#include "sdl.h"
#include "GBC.h"
#include <SDL2/SDL_render.h>

static SDL_Window *win;
static SDL_Renderer *rend;
static SDL_Texture *screen;
static char pixels[160 * 144 * 3];
static int pitch = 160 * 3;

int sdl_init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "[Error]: sdl.c: SDL could not Initialize, SDL error %s\n", SDL_GetError());
        return 1;
    }

    if ((win = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 160, 144, SDL_WINDOW_SHOWN)) == NULL) {
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
    win = NULL;
    rend = NULL;
    SDL_Quit();
    return 0;
}

void update_texture(struct GameBoy *GameBoy) {
    if (SDL_LockTexture(screen, NULL, (void *) pixels, &pitch) != 0) {
        fprintf(stderr, "[Error] sdl.c: Unable to lock screen texture, %s\n", SDL_GetError());
        return;
    }

    memcpy(pixels, GameBoy->ppu.display, sizeof(pixels));

    SDL_UnlockTexture(screen);

    return;
}

void Render(struct GameBoy *GameBoy) {
    SDL_SetRenderDrawColor(rend, 0XFF, 0XFF, 0XFF, 0XFF);
    SDL_RenderClear(rend);

    update_texture(GameBoy);
    SDL_RenderPresent(rend);
}
void RenderScreen(struct GameBoy *GameBoy) {
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);

    // clear the current rendering target with the drawing color
    SDL_RenderClear(rend);


    // iterating thru the display (160*144)
    SDL_Rect rect;
    for (int y = 0; y < 144; y++) {
        for (int x = 0; x < 160; x++) {

                SDL_SetRenderDrawColor(rend,
                                       GameBoy->ppu.display[x][y][0],
                                       GameBoy->ppu.display[x][y][1],
                                       GameBoy->ppu.display[x][y][2],
                                       255);

                SDL_RenderDrawPoint(rend, x, y);
        }
    }


    // update the screen
    SDL_RenderPresent(rend);
    return;
}
