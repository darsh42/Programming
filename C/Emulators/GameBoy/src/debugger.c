#include "debugger.h"


struct debugger debugger;
static int base_addr = 0;

int STR_TO_HEX(char *str) {
    int res = 0;
    for (int i = 0; str[i] != '\0'; i++, res <<= 4) {
        if (str[i] >= '0' && str[i] <= '9') res += (str[i] - '0');
        if (str[i] >= 'a' && str[i] <= 'f') res += (str[i] - 'a' + 10);
        if (str[i] >= 'A' && str[i] <= 'F') res += (str[i] - 'A' + 10);
    }

    return res >> 4;
}

int debugger_get_addr() {
    char addr[5];

    getstr(addr);
    addr[4] = '\0';

    return STR_TO_HEX(addr);
}

void debugger_seek_mem(uint16_t base, int maxY, int maxX) {
    bool status;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            mvprintw(3+i, 28+j*6, "%04x", mem_read(base + 8*i + j, 1, &status));
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

void debugger_flag_print(uint8_t flag, int yStart) {
    mvprintw(yStart, 2, "FLAGS:");
    mvprintw(yStart, 6+2, "%d", (flag & 0X80)== 0X80);
    mvprintw(yStart, 6+3, "%d", (flag & 0X40)== 0X40);
    mvprintw(yStart, 6+4, "%d", (flag & 0X20)== 0X20);
    mvprintw(yStart, 6+5, "%d", (flag & 0X10)== 0X10);
    mvprintw(yStart, 6+6, "%d", (flag & 0X08)== 0X08);
    mvprintw(yStart, 6+7, "%d", (flag & 0X04)== 0X04);
    mvprintw(yStart, 6+8, "%d", (flag & 0X02)== 0X02);
    mvprintw(yStart, 6+9, "%d", (flag & 0X01)== 0X01);
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

    debugger_flag_print(debugger.cpu->AF.lower, 14);

    // PPU Register
    mvprintw(17, 2, "PPU Registers");
    mvprintw(18, 2, "LCDC: 0X%04x", *debugger.ppu->LCDC);
    mvprintw(19, 2, "LCDS: 0X%04x", *debugger.ppu->LCDS);

    mvprintw(20, 2, "LY: 0X%04x", *debugger.ppu->LY);
    mvprintw(21, 2, "LYC: 0X%04x", *debugger.ppu->LYC);
    mvprintw(22, 2, "(SCX, SCY): {0X%04x, 0X%04x}", *debugger.ppu->SCX, *debugger.ppu->SCY);
    mvprintw(23, 2, "(WX, WY): {0X%04x, 0X%04x}", *debugger.ppu->WX, *debugger.ppu->WY);

    // Memory Registers

    refresh();

    // Input Handle
    char c;
    mvprintw(yMax - 2, 2, " SPC - cont, m - mem seek, q - quit");
    while ((c = getch()) != ' ') {
        switch (c) {
            // seek memory location
            case('m'): {
                move(yMax-2, 2);
                clrtoeol();
                mvprintw(yMax - 2, 2, "Specify a memory addr (e.g ffff)");

                base_addr = debugger_get_addr();
                break;
            }
            // quit Debugger and GameBoy
            case('q'): return 1; break;
        }

        move(yMax-2, 2);
        clrtoeol();
        mvprintw(yMax - 2, 2, " SPC - cont, m - mem seek, q - quit");
        mvprintw(yMax - 2, 72, "0X%04x", base_addr);
    }

    debugger_seek_mem(base_addr, yMax, xMax);
    refresh();

    return 0;
}
