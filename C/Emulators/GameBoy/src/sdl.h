#ifndef SDL_H_INCLUDED
#define SDL_H_INCLUDED

#include <SDL2/SDL.h>
#include "common.h"
#include "ppu.h"

extern uint32_t *get_display();

extern struct ppu *get_ppu();

#endif // SDL_H_INCLUDED
