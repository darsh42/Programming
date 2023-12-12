#include "GBC.h"

void gameboy_init() {
    sdl_init();
    mem_init();
    cpu_init();
    ppu_init();
    // apu_init();

    debugger_init();
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "[Error] GBC.c: Usage ./gameboy <rom>\n");
        return 1;
    }

    if (load_rom(argv+=1) != 0) return 1;

    gameboy_init();

    bool emulate = true;
    int MAXCYCLES = 69905;

    while (emulate) {
        while (cpu_clocks() < MAXCYCLES) {
            cpu_exec();
            ppu_exec();
            if (debugger_update() != 0) {
                emulate = false;
                break;
            }
        }
        cpu_clock_reset();
    }


    debugger_kill();
    sdl_kill();
    return 0;
}
