#ifndef GBC_H_INCLUDED
#define GBC_H_INCLUDED

#include "common.h"
#include "cpu.h"
#include "ppu.h"
#include "apu.h"
#include "mem.h"

struct GameBoy {
    struct cpu cpu;
    struct ppu ppu;
    struct apu apu;
    struct mem mem;

    /*
    ** Display
    */

};

// Device initization functions
extern int sdl_init();
extern void mem_init();
extern void cpu_init(struct cpu *cpu);
extern void ppu_init(struct ppu *ppu);

// Device Execution functions
extern void cpu_exec(struct cpu *cpu);
extern void ppu_exec(struct ppu *cpu);
extern void apu_exec(struct apu *cpu);

// Rendering Functions
extern int sdl_kill();
extern void RenderScreen(struct GameBoy *GameBoy);

// Debugger
extern void debugger_init(struct cpu *cpu, struct mem *mem, struct ppu *ppu, struct apu *apu);
extern void debugger_kill();
extern void debugger_update();

#endif // GBC_H_INCLUDED
