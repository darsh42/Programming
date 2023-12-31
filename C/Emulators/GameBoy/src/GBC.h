#ifndef GBC_H_INCLUDED
#define GBC_H_INCLUDED

#include "common.h"

// Load ROM
extern int mem_load_boot(char **filename);
extern int mem_cartridge_load(char **filename);

// cpu clock handles
extern int cpu_clocks();
extern int cpu_clocks_ins(int oldtime);
extern void cpu_clock_reset();

// Emulation initization functions
extern void mem_init();
extern void cpu_init();
extern void ppu_init();
extern void init_timer();
extern void init_handler();

// Device Execution functions
extern int cpu_exec();
extern void ppu_exec(int cpu_ticks);
extern void apu_exec();
extern void update_timers(int cpu_ticks);
extern void handle_interrupts();

// I/O Functions
extern int sdl_init();
extern int sdl_kill();
extern int sdl_render();
extern int sdl_input();

// Debugger
extern void debugger_init();
extern void debugger_kill();
extern int debugger_update();

#endif // GBC_H_INCLUDED
