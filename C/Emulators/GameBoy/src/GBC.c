#include "GBC.h"

struct GameBoy GameBoy;
void gameboy_init() {
    sdl_init();
    mem_init(&GameBoy.mem);
    cpu_init(&GameBoy.cpu);
    ppu_init(&GameBoy.ppu);
    // apu_init(&Gameboy.apu);

    debugger_init(&GameBoy.cpu, &GameBoy.mem, &GameBoy.ppu, &GameBoy.apu);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "[Error] GBC.c: Usage ./gameboy <rom>\n");
        return 1;
    }

    FILE *rom;
    if ( (rom = fopen(*(argv+=1), "rb")) == NULL) {
        fprintf(stderr, "[Error] GBC.c: Cannot read rom <%s>\n", *argv);
        return 1;
    }
    int read_status = 0;
    if ((read_status = fread(GameBoy.mem.main, 1, 0X8000, rom)) == 0) {
        fprintf(stderr, "[Error] GBC.c: Could not read <%s>, bytes read: %d\n", *argv, read_status);
        return 1;
    }
    fclose(rom);

    gameboy_init();

    bool emulate = true;
    int MAXCYCLES = 69905;



    while (emulate) {
        while (GameBoy.cpu.clock < MAXCYCLES) {
            cpu_exec(&GameBoy.cpu);
            ppu_exec(&GameBoy.ppu);
            debugger_update();
        }
        GameBoy.cpu.clock = 0;
        RenderScreen(&GameBoy);
    }

    debugger_kill();
    return 0;
}
