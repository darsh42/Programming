#include "debugger.h"

struct debugger debugger;
static int base_addr = 0;
static int continue_for = 0;
static int breakpoint = -1;
static FILE *GBlog;

void debugger_init() {
    debugger.cpu = get_cpu();
    debugger.ppu = get_ppu();
    debugger.mem = get_mem();
    debugger.timers = get_timers();
    debugger.handler = get_handler();
    debugger.joypad = get_joypad();
    GBlog = fopen("GAMEBOY.log", "w");

    initscr(); noecho(); curs_set(0);
}

void debugger_kill() {
    fclose(GBlog);
    endwin();
}

int STR_TO_HEX(char *str) {
    int res = 0;
    for (int i = 0; str[i] != '\0'; i++, res <<= 4) {
        if (str[i] >= '0' && str[i] <= '9') res += (str[i] - '0');
        if (str[i] >= 'a' && str[i] <= 'f') res += (str[i] - 'a' + 10);
        if (str[i] >= 'A' && str[i] <= 'F') res += (str[i] - 'A' + 10);
    }

    return res >> 4;
}

void debugger_log_state() {
    uint16_t SP, PC;
    uint8_t A, F, B, C, D, E, H, L;

    A = debugger.cpu->AF.upper;
    B = debugger.cpu->BC.upper;
    D = debugger.cpu->DE.upper;
    H = debugger.cpu->HL.upper;

    F = debugger.cpu->AF.lower;
    C = debugger.cpu->BC.lower;
    E = debugger.cpu->DE.lower;
    L = debugger.cpu->HL.lower;

    PC = debugger.cpu->PC;
    SP = debugger.cpu->SP;
    uint8_t PC_mem_1 = mem_read(PC+0);
    uint8_t PC_mem_2 = mem_read(PC+1);
    uint8_t PC_mem_3 = mem_read(PC+2);
    uint8_t PC_mem_4 = mem_read(PC+3);
    fprintf(GBlog, "A:%02X F:%02X B:%02X C:%02X D:%02X E:%02X H:%02X L:%02X SP:%04X PC:%04X PCMEM:%02X,%02X,%02X,%02X\n", A, F, B, C, D, E, H, L, SP, PC, PC_mem_1, PC_mem_2, PC_mem_3, PC_mem_4);
}

int debugger_get_addr() {
    char addr[5];

    getstr(addr);
    addr[4] = '\0';

    return STR_TO_HEX(addr);
}

void debugger_seek_mem(uint16_t base) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            mvprintw(3+i, 28+j*6, "%02x", mem_read(base + 8*i + j));
        }
    }

    return;
}

void debugger_flag_print(uint8_t flag, int yStart, char *field) {
    int len;
    for (len = 0; field[len] != '\0'; len++);

    mvprintw(yStart, 2, "%s", field);
    mvprintw(yStart, len+2, "%d", (flag & 0X80)== 0X80);
    mvprintw(yStart, len+3, "%d", (flag & 0X40)== 0X40);
    mvprintw(yStart, len+4, "%d", (flag & 0X20)== 0X20);
    mvprintw(yStart, len+5, "%d", (flag & 0X10)== 0X10);
    mvprintw(yStart, len+6, "%d", (flag & 0X08)== 0X08);
    mvprintw(yStart, len+7, "%d", (flag & 0X04)== 0X04);
    mvprintw(yStart, len+8, "%d", (flag & 0X02)== 0X02);
    mvprintw(yStart, len+9, "%d", (flag & 0X01)== 0X01);
}

int debugger_get_input(int yMax) {
    // Input Handle
    char c;
    mvprintw(yMax - 2, 2, " SPC - cont, m - mem seek (0Xffff), b - breakpoint (0Xffff), c - continue for (e.g 10), q - quit");
    while ((c = getch()) != ' ') {
        switch (c) {
            // seek memory location
            case('m'): {
                move(yMax-2, 2);
                clrtoeol();
                mvprintw(yMax - 2, 2, "Specify a memory addr (e.g ffff)");

                // implement error checking
                base_addr = debugger_get_addr();
                debugger_seek_mem(base_addr);
                break;
            }
            case('b'):
                // jump to instruction
                move(yMax-2, 2);
                clrtoeol();
                mvprintw(yMax - 2, 2, "Specify a breakpoint (e.g ffff)");

                // implement error checking
                breakpoint = debugger_get_addr();
                break;
            // quit Debugger and GameBoy
            case('c'):
                // coninue for n instructions
                move(yMax-2, 2);
                clrtoeol();
                mvprintw(yMax - 2, 2, "Specify instruction count (e.g 10)");

                // implement error checking
                uint16_t n_ins = debugger_get_addr();
                continue_for = n_ins;
                break;
            case('q'): return 1; break;
        }

        move(yMax-2, 2);
        clrtoeol();
        mvprintw(yMax - 2, 2, " SPC - cont, m - mem seek, q - quit");
        mvprintw(yMax - 2, 72, "0X%04x", base_addr);

        refresh();
    }



    return 0;
}

int debugger_update() {

    int yMax = getmaxy(stdscr);

    // DEBUGGER.CPU Registers
    mvprintw(3, 2,"CPU Registers");
    mvprintw(4, 2,"clocks: %d", debugger.cpu->clock);

    mvprintw(5, 2,"PC: 0X%04x", debugger.cpu->PC);
    mvprintw(6, 2,"CIR: 0X%02x", debugger.cpu->CIR);
    mvprintw(7, 2,"SP: 0X%04x", debugger.cpu->SP);
    mvprintw(8, 2,"AF: 0X%02x 0X%02x", debugger.cpu->AF.upper, debugger.cpu->AF.lower);
    mvprintw(9, 2,"BC: 0X%02x 0X%02x", debugger.cpu->BC.upper, debugger.cpu->BC.lower);
    mvprintw(10, 2, "DE: 0X%02x 0X%02x", debugger.cpu->DE.upper, debugger.cpu->DE.lower);
    mvprintw(11, 2, "HL: 0X%02x 0X%02x", debugger.cpu->HL.upper, debugger.cpu->HL.lower);
    mvprintw(12, 2, "IME: 0X%04x", debugger.cpu->IME);

    debugger_flag_print(debugger.cpu->AF.lower, 14, "FLAGS:");

    // PPU Register
    mvprintw(17, 2, "PPU Registers");
    debugger_flag_print(*debugger.ppu->LCDC, 18, "LCDC: ");
    debugger_flag_print(*debugger.ppu->LCDS, 19, "LCDS: ");

    mvprintw(20, 2, "LY: 0X%04x", *debugger.ppu->LY);
    mvprintw(21, 2, "LYC: 0X%04x", *debugger.ppu->LYC);
    mvprintw(22, 2, "(SCX, SCY): {0X%04x, 0X%04x}", *debugger.ppu->SCX, *debugger.ppu->SCY);
    mvprintw(23, 2, "(WX, WY): {0X%04x, 0X%04x}", *debugger.ppu->WX, *debugger.ppu->WY);

    // TODO:
    // Memory Registers
    mvprintw(2, 2, "TITLE: %s", debugger.mem->cartridge_header.Title);
    debugger_seek_mem(base_addr);

    // Timer Registers
    mvprintw(24, 2, "DIV: 0X%02x", *debugger.timers->DIV);
    mvprintw(25, 2, "TIMA: 0X%02x", *debugger.timers->TIMA);
    mvprintw(26, 2, "TMA: 0X%02x", *debugger.timers->TMA);
    mvprintw(27, 2, "TAC: 0X%02x", *debugger.timers->TAC);

    // Interrupt Requests
    debugger_flag_print(*debugger.handler->IF, 28, "INTER REQ:");
    debugger_flag_print(*debugger.handler->IE, 29, "INTER ENA:");

    // Joypad Registers
    debugger_flag_print(*debugger.joypad->JOYP, 30, "JOYPAD:");

    int debug_status = 0;

    if (breakpoint != -1) {
        if (debugger.cpu->PC == breakpoint) {
            breakpoint = -1;
            debug_status = debugger_get_input(yMax);
        }
    } else if (continue_for > 0) {
        continue_for--;
    } else {
        debug_status = debugger_get_input(yMax);
    }

    if (debug_status != 0) return 1;

    refresh();

    debugger_log_state();
    // if (mem_read(0XFF02) == 0X81) {
    //     char c = mem_read(0XFF01);
    //     if (isalnum(c) && prevFF01 != c) {
    //         fprintf(log, "%c", mem_read(0XFF01));
    //         mem_write(0XFF02, 0);
    //         prevFF01 = mem_read(0XFF01);
    //     }
    // }

    return 0;
}
