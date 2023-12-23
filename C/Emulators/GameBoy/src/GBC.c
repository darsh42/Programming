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

    if (mem_load_boot(argv+=1) != 0) return 1;

    gameboy_init();

    bool emulate = true;
    int MAXCYCLES = 69905;
    int oldtime = 0;

    while (emulate) {
        while ((oldtime = cpu_clocks()) < MAXCYCLES) {
            cpu_exec();
            ppu_exec();
            update_timers(oldtime, cpu_clocks());
            handle_interrupts();
            if (debugger_update() != 0) {
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
