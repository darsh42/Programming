#include "debugger.h"

struct cpu *cpu;
struct ppu *ppu;
struct apu *apu;
struct mem *mem;

void debugger_seek_mem() {
    return;
}

void debugger_init(struct cpu *GBcpu, struct mem *GBmem, struct ppu *GBppu, struct apu *GBapu) {
    cpu = GBcpu;
    mem = GBmem;
    ppu = GBppu;
    apu = GBapu;

    initscr(); noecho(); curs_set(0);
}

void debugger_kill() {
    endwin();
}

void debugger_flag_print(uint8_t flag) {
    return;
}

void debugger_update() {

    int xMax, yMax;
    getmaxyx(stdscr, yMax, xMax);

    // CPU Registers
    mvprintw(2, 3, "CPU Registers");
    mvprintw(2, 4, "clocks: %d", cpu->clock);

    mvprintw(2, 5, "PC: 0X%04x", cpu->PC);
    mvprintw(2, 6, "CIR: 0X%04x", cpu->CIR);
    mvprintw(2, 7, "SP: 0X%04x", cpu->SP);
    mvprintw(2, 8, "AF: 0X%02x 0X02%x", cpu->AF.upper, cpu->AF.lower);
    mvprintw(2, 9, "BC: 0X%02x 0X02%x", cpu->BC.upper, cpu->BC.lower);
    mvprintw(2, 10, "DE: 0X%02x 0X02%x", cpu->DE.upper, cpu->DE.lower);
    mvprintw(2, 11, "HL: 0X%02x 0X02%x", cpu->HL.upper, cpu->HL.lower);
    mvprintw(2, 12, "IME: 0X%04x", cpu->IME);

    // PPU Registers
    mvprintw(2, 14, "PPU Registers");
    mvprintw(2, 15, "clocks: %d", cpu->clock);

    mvprintw(2, 16, "LCDC: 0X%04x", ppu->LCDC);
    mvprintw(2, 17, "LCDS: 0X%04x", ppu->LCDS);

    mvprintw(2, 18, "LY: 0X%04x", ppu->LY);
    mvprintw(2, 19, "LYC: 0X%04x", ppu->LYC);
    mvprintw(2, 20, "(SCX, SCY): {0X%04x, 0X%04x}", ppu->SCX, ppu->SCY);
    mvprintw(2, 21, "(WX, WY): {0X%04x, 0X%04x}", ppu->WX, ppu->WY);

    // Memory Registers

    refresh();

    // Input Handle

    char c;
    while ((c = getch()) != ' ') {
        switch (c) {
            // seek memory location
            case('m'): debugger_seek_mem(); break;
            // quit Debugger and GameBoy
            case('q'): break;
        }
    }

    return;
}
