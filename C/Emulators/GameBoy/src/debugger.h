#ifndef DEBUGGER_H_INCLUDED
#define DEBUGGER_H_INCLUDED

#include "common.h"
#include "cpu.h"
#include "ppu.h"
#include "apu.h"
#include "mem.h"
#include "timer.h"
#include "interrupts.h"

#include <ncurses.h>

#define strlen(s, len) for (*len = 0; s[*len] != '\0'; *len++)


extern struct cpu *get_cpu();
extern struct ppu *get_ppu();
extern struct mem *get_mem();
extern struct timers *get_timers();
extern struct interrupt_handler *get_handler();

struct debugger {
    struct cpu *cpu;
    struct ppu *ppu;
    struct mem *mem;
    struct timers *timers;
    struct interrupt_handler *handler;
};

#endif // DEBUGGER_H_INCLUDED
