#ifndef DEBUGGER_H_INCLUDED
#define DEBUGGER_H_INCLUDED

#include "common.h"
#include "cpu.h"
#include "ppu.h"
#include "apu.h"
#include "mem.h"

#include <ncurses.h>



extern struct cpu *get_cpu();
extern struct ppu *get_ppu();
extern struct mem *get_mem();

struct debugger {
    struct cpu *cpu;
    struct ppu *ppu;
    struct mem *mem;
};


#endif // DEBUGGER_H_INCLUDED
