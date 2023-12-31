#include "GBC.h"

void gameboy_init() {
    sdl_init();
    mem_init();
    cpu_init();
    ppu_init();
    init_timer();
    init_handler();
    // apu_init();

    debugger_init();
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "[Error] GBC.c: Usage ./gameboy <rom>\n");
        return 1;
    }

    if (mem_cartridge_load(argv+=1) != 0) return 1;

    gameboy_init();

    bool emulate = true;
    int cycles = 0;

    while (emulate) {
        while (cpu_clocks() < MAXCYCLES) {

            if (debugger_update() != 0) {
                emulate = false;
                break;
            }

            cycles = cpu_exec();

            ppu_exec(cycles);

            update_timers(cycles);

            handle_interrupts();

            if (sdl_input() != 0) {
                emulate = false;
                break;
            }
        }
        sdl_render();
        cpu_clock_reset();
    }


    debugger_kill();
    sdl_kill();
    return 0;
}
