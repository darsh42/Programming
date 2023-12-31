#include "sdl.h"

struct joypad joypad;

struct joypad *get_joypad() {
    return &joypad;
}

SDL_Window *win;
SDL_Renderer *rend;
SDL_Texture *screen;

int sdl_init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "[Error]: sdl.c: SDL could not Initialize, SDL error %s\n", SDL_GetError());
        return 1;
    }

    if ((win = SDL_CreateWindow("GAMEBOY", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 320, 288, SDL_WINDOW_SHOWN)) == NULL) {
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

    joypad.JOYP = mem_pointer(0XFF00);
    joypad.A      = false;
    joypad.B      = false;
    joypad.select = false;
    joypad.start  = false;
    joypad.left   = false;
    joypad.right  = false;
    joypad.up     = false;
    joypad.down   = false;

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
    SDL_RenderCopy(rend, screen, NULL, &(SDL_Rect) {0, 0, 320, 288});
    // Render the texture to the screen
    SDL_RenderPresent(rend);
    return 0;
}

int sdl_input() {
    // unset all the joypad input bits
    *joypad.JOYP = SET_BIT(*joypad.JOYP, 0);
    *joypad.JOYP = SET_BIT(*joypad.JOYP, 1);
    *joypad.JOYP = SET_BIT(*joypad.JOYP, 2);
    *joypad.JOYP = SET_BIT(*joypad.JOYP, 3);
    SDL_Event keypress;

    while(SDL_PollEvent(&keypress)) {
        switch(keypress.type) {
            case(SDL_QUIT): return 1; break;
            case(SDL_KEYDOWN):
                switch(keypress.key.keysym.sym) {
                    case(SDLK_a):      joypad.A = true; break;
                    case(SDLK_s):      joypad.B = true; break;
                    case(SDLK_SPACE):  joypad.start = true; break;
                    case(SDLK_RETURN): joypad.select = true; break;
                    case(SDLK_RIGHT):  joypad.right = true; break;
                    case(SDLK_LEFT):   joypad.left = true; break;
                    case(SDLK_UP):     joypad.up = true; break;
                    case(SDLK_DOWN):   joypad.down = true; break;
                    case(SDLK_ESCAPE): return 1; break;
                }
                break;
            case(SDL_KEYUP):
                switch(keypress.key.keysym.sym) {
                    case(SDLK_a):      joypad.A = false; break;
                    case(SDLK_s):      joypad.B = false; break;
                    case(SDLK_SPACE):  joypad.start = false; break;
                    case(SDLK_RETURN): joypad.select = false; break;
                    case(SDLK_RIGHT):  joypad.right = false; break;
                    case(SDLK_LEFT):   joypad.left = false; break;
                    case(SDLK_UP):     joypad.up = false; break;
                    case(SDLK_DOWN):   joypad.down = false; break;
                }
                break;
        }
    }

    if (!TEST_BIT(*joypad.JOYP, 5)) {
        // Game asking for SELECT/START/A/B buttons
        *joypad.JOYP = (joypad.A) ? RESET_BIT(*joypad.JOYP, 0): *joypad.JOYP;
        *joypad.JOYP = (joypad.B) ? RESET_BIT(*joypad.JOYP, 1): *joypad.JOYP;
        *joypad.JOYP = (joypad.select) ? RESET_BIT(*joypad.JOYP, 2): *joypad.JOYP;
        *joypad.JOYP = (joypad.start) ? RESET_BIT(*joypad.JOYP, 3): *joypad.JOYP;
    } else if (!TEST_BIT(*joypad.JOYP, 4)) {
        // Game asking for LEFT/RIGHT/UP/DOWN buttons
        *joypad.JOYP = (joypad.right) ? RESET_BIT(*joypad.JOYP, 0): *joypad.JOYP;
        *joypad.JOYP = (joypad.left) ? RESET_BIT(*joypad.JOYP, 1): *joypad.JOYP;
        *joypad.JOYP = (joypad.up) ? RESET_BIT(*joypad.JOYP, 2): *joypad.JOYP;
        *joypad.JOYP = (joypad.down) ? RESET_BIT(*joypad.JOYP, 3): *joypad.JOYP;
    }

    if ((*joypad.JOYP & 0b1111) != 0b1111)
        interrupt_request(0b00010000);

    // if ((*joypad.JOYP & 0b111111) != 0b111111)
    //     printf("%x\n", *joypad.JOYP);

    return 0;
}
