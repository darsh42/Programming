#ifndef SDL_H_INCLUDED
#define SDL_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include "common.h"

struct joypad {
    /*
    ** Stores memory pointer to joypad register
    ** BIT 7:
    ** BIT 6:
    ** BIT 5: Button select
    ** BIT 4: D-Pad select
    ** BIT 3: Start/Down
    ** BIT 2: Select/Up
    ** BIT 1: B-button/Left
    ** BIT 0: A-button/Right
    ** NOTE: A button is turned on when the bit is low
    */
    uint8_t *JOYP;

    bool up;
    bool down;
    bool left;
    bool right;

    bool A;
    bool B;
    bool select;
    bool start;
};
/* INTERRUPT FUNCTIONS */
void interrupt_request(uint8_t request);

/* MEMORY FUNCTION */
extern uint8_t *mem_pointer(uint16_t addr);

/* PPU FUNCTIONS */
extern uint32_t *get_display();

#endif // SDL_H_INCLUDED
