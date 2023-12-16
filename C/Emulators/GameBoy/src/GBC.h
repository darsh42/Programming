#ifndef GBC_H_INCLUDED
#define GBC_H_INCLUDED

#include "common.h"

// Load ROM
extern int mem_cartridge_load(char **filename);

// cpu clock handles
int cpu_clocks();
void cpu_clock_reset();

// Device initization functions
extern int sdl_init();
extern void mem_init();
extern void cpu_init();
extern void ppu_init();
extern void init_timer();
extern void init_handler();

// Device Execution functions
extern void cpu_exec();
extern void ppu_exec();
extern void apu_exec();
extern void update_timers(int oldtime, int currenttime);
extern void handle_interrupts();

// Rendering Functions
extern int sdl_kill();
extern void RenderScreen();

// Debugger
extern void debugger_init();
extern void debugger_kill();
extern int debugger_update();

#endif // GBC_H_INCLUDED
