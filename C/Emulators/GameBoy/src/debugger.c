#include "debugger.h"

int STR_TO_HEX(char *str) {
    int res = 0;
    for (int i = 0, place = 1; str[i] != EOF; i++, place *= 16) {
        if (str[i] >= '0' && str[i] <= '9') res += place * (str[i] - '0');
        if (str[i] >= 'a' && str[i] <= 'f') res += place * (str[i] - 'a');
        if (str[i] >= 'A' && str[i] <= 'F') res += place * (str[i] - 'A');
    }

    return res;
}

struct debugger debugger;

void debugger_seek_mem(uint8_t base, int maxY, int maxX) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            mvprintw(3+i, 28+j*6, "%04x", debugger.mem->main[base + 8*i + j]);
        }
    }

    return;
}

void debugger_init() {
    debugger.cpu = get_cpu();
    debugger.ppu = get_ppu();
    debugger.mem = get_mem();

    initscr(); noecho(); curs_set(0);
}

void debugger_kill() {
    endwin();
}

void debugger_flag_print(uint8_t flag) {
    return;
}

int debugger_update() {

    int xMax, yMax;
    getmaxyx(stdscr, yMax, xMax);

    // DEBUGGER.CPU Registers
    mvprintw(3, 2,"CPU Registers");
    mvprintw(4, 2,"clocks: %d", debugger.cpu->clock);

    mvprintw(5, 2,"PC: 0X%08x", debugger.cpu->PC);
    mvprintw(6, 2,"CIR: 0X%08x", debugger.cpu->CIR);
    mvprintw(7, 2,"SP: 0X%04x", debugger.cpu->SP);
    mvprintw(8, 2,"AF: 0X%04x 0X%04x", debugger.cpu->AF.upper, debugger.cpu->AF.lower);
    mvprintw(9, 2,"BC: 0X%04x 0X%04x", debugger.cpu->BC.upper, debugger.cpu->BC.lower);
    mvprintw(10, 2, "DE: 0X%04x 0X%04x", debugger.cpu->DE.upper, debugger.cpu->DE.lower);
    mvprintw(11, 2, "HL: 0X%04x 0X%04x", debugger.cpu->HL.upper, debugger.cpu->HL.lower);
    mvprintw(12, 2, "IME: 0X%04x", debugger.cpu->IME);

    // PPU Register
    mvprintw(14, 2, "PPU Registers");
    mvprintw(16, 2, "LCDC: 0X%04x", debugger.ppu->LCDC);
    mvprintw(17, 2, "LCDS: 0X%04x", debugger.ppu->LCDS);

    mvprintw(18, 2, "LY: 0X%04x", debugger.ppu->LY);
    mvprintw(19, 2, "LYC: 0X%04x", debugger.ppu->LYC);
    mvprintw(20, 2, "(SCX, SCY): {0X%04x, 0X%04x}", debugger.ppu->SCX, debugger.ppu->SCY);
    mvprintw(21, 2, "(WX, WY): {0X%04x, 0X%04x}", debugger.ppu->WX, debugger.ppu->WY);

    // Memory Registers

    refresh();

    // Input Handle

    char c;
    while ((c = getch()) != ' ') {
        switch (c) {
            // seek memory location
            case('m'): {
                char addr[4];
                getstr(addr);

                debugger_seek_mem(STR_TO_HEX(addr), yMax, xMax);
                break;
            }
            // quit Debugger and GameBoy
            case('q'): return 1; break;
        }
    }

    return 0;
}
