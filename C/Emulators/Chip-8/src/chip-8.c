#include "chip-8.h"

// exit condition
bool emulate = true;
struct cpu {
    // General purpose registers
    uint8_t V[16];

    // Program Counter and address register
    uint16_t PC;
    uint16_t  I;

    // Stack Pointer
    uint8_t SP;

    // Delay and Sound Timers
    uint8_t DT;
    uint8_t ST;

    // Stack and RAM arrays
    uint16_t stack[16];
    /*
    **  -------------- <- 0x1000
    ** |              |
    ** |              |
    ** |              |
    ** |              |
    ** |              |
    ** |   PRG DATA   |
    ** |              |
    ** |              |
    ** |              |
    ** |              |
    ** |--------------| <- 0X0200
    ** |              |
    ** |    Fonts     |
    ** |              |
    **  --------------
    */
    uint8_t RAM[0X1000];

    // Current instruction register (for convinence)
    uint16_t CIR;

    // Display
    uint8_t display[HEIGHT * WIDTH];

    bool keyboard[16];
} cpu;

int main(int argc, char **argv) {
    if (argc == 1) {fprintf(stderr, "[ERROR] chip-8.c: Usage ./chip_8 <program.ch8>\n"); return 1;}

    // initialize cpu
    for (int i = 0; i < 16; i++)         cpu.V[i] = 0;
    for (int i = 0; i < 16; i++)     cpu.stack[i] = 0;
    for (int i = 0; i < 128; i++)  cpu.display[i] = 0;
    for (int i = 0; i < 80; i++) cpu.RAM[i] = fonts[i];

    cpu.PC  = 0;
    cpu.I   = 0;
    cpu.SP  = 0;
    cpu.DT  = 0;
    cpu.ST  = 0;
    cpu.CIR = 0;

    cpu.PC = PRG_START;

    FILE *rom;
    if ( (rom = fopen(*(argv+=1), "rb")) == NULL) {
        fprintf(stderr, "[Error] chip-8.c: Cannot read rom <%s>\n", *argv);
        return 1;
    }
    int read_status = 0;
    if ((read_status = fread(&cpu.RAM[PRG_START], 1, sizeof(cpu.RAM) - PRG_START, rom)) == 0) {
        fprintf(stderr, "[Error] chip-8.c: Could not read <%s>, bytes read: %d\n", *argv, read_status);
        return 1;
    }
    fclose(rom);

    init_display();
    while(emulate) {
        // process input
        getkey();
        // retrieve current instruction
        cpu.CIR = (cpu.RAM[cpu.PC++] << 8) | (cpu.RAM[cpu.PC++]);

        // interpret each instruction

        uint8_t *X = &cpu.V[(cpu.CIR & 0X0F00) >> 8];
        uint8_t *Y = &cpu.V[(cpu.CIR & 0X00F0) >> 4];

        switch((cpu.CIR & 0XF000) >> 12) {
            case(0X0):
                /* 0NNN - Call      | (not necessary)         *
                 * 00E0 - Display   | Clear the screen        *
                 * 00EE - Flow      | Returns from subroutine */
                switch(cpu.CIR & 0X0FFF) {
                    case (0X0E0):
                        for (int i = 0; i < HEIGHT; i++)
                            for (int j = 0; j < WIDTH; j++)
                                cpu.display[i * WIDTH + j] = 0;
                        break;
                    case (0X0EE):
                        cpu.PC = cpu.stack[cpu.SP--];
                        break;
                    default:
                        emulate = false;
                        break;
                }
                break;
            case(0X1):
                 /* 1NNN - Flow    | goto address NNN         */
                cpu.PC = cpu.CIR & 0X0FFF;
                break;
            case(0X2):
                /* 2NNN - Flow    | Calls subroutine at NNN  */
                cpu.stack[++cpu.SP] = cpu.PC;
                cpu.PC = cpu.CIR & 0X0FFF;
                break;
            case(0X3):
                /* 3XNN - Flow    | Skip next ins if VX == NN*/
                cpu.PC += (*X == (cpu.CIR & 0X00FF)) ? 2: 0;
                break;
            case(0X4):
                /* 4XNN - Flow    | Skip next ins if VX != NN*/
                cpu.PC += (*X != (cpu.CIR & 0X00FF)) ? 2: 0;
                break;
            case(0X5):
                /* 5XY0 - Cond   | skip next ins if VX == VY*/
                cpu.PC += (*X == *Y) ? 2: 0;
                break;
            case(0X6):
                /* 6XNN - Const  | set VX to NN             */
                *X = cpu.CIR & 0X00FF;
                break;
            case(0X7):
                /* 7XNN - Const  | increment VX by NN       */
                *X += cpu.CIR & 0X00FF;
                break;
            case(0X8): {
                /* 8XY0 - Const | increment VX by NN                 *
                 * 8XY1 - BitOp | set VX to VX or VY                 *
                 * 8XY2 - BitOp | set VX to VX and VY                *
                 * 8XY3 - BitOp | set VX to VX xor VY                *
                 * 8XY4 - Math  | Do VX += VY, store carry in VF     *
                 * 8XY5 - Math  | Do VX -= VY, store carry in VF     *
                 * 8XY6 - BitOp | Do VX>>1, store lsb in VF          *
                 * 8XY7 - Math  | Do VX = VY - VX, store carry in VF *
                 * 8XYE - BitOp | DO VX<<1, store msb in VF          */

                uint8_t tmp = *X;
                switch(cpu.CIR & 0X000F) {
                    case (0X0): *X = *Y;  break;
                    case (0X1): *X |= *Y; break;
                    case (0X2): *X &= *Y; break;
                    case (0X3): *X ^= *Y; break;
                    case (0X4):
                        *X += *Y;
                        cpu.V[0XF] = (tmp + *Y) > 0XFF;
                        break;
                    case (0X5):
                        *X = *X - *Y;
                        cpu.V[0XF] = (tmp >= *Y);
                        break;
                    case (0X6):
                        (*X) >>= 1;
                        cpu.V[0XF] = (tmp) & 0X1;
                        break;
                    case (0X7):
                        *X = *Y - *X;
                        cpu.V[0XF] = (*Y >= tmp);
                        break;
                    case (0XE):
                        (*X) <<= 1;
                        cpu.V[0XF] = (tmp >> 7) & 0X1;
                        break;
                }
                break;
            }
            case(0X9):
                /* 9XY0 - Cond   | skip next ins, if VX != VY */
                cpu.PC += (*X != *Y) ? 2: 0;
                break;
            case(0XA):
                /* ANNN - MEM    | sets I to addr NNN         */
                cpu.I = cpu.CIR & 0X0FFF;
                break;
            case(0XB):
                /* BNNN - Flow   | jumps to NNN + V0          */
                cpu.PC = cpu.CIR & 0X0FFF + cpu.V[0];
                break;
            case(0XC):
                /* CXNN - Rand   | sets VX to rand() & NN     */
                *X = rand() & cpu.CIR & 0X00FF;
                break;
            case(0XD): {
                /* DXYN - Display| set screen pixels          */
                unsigned short px;
                uint8_t N = cpu.CIR & 0X000F;

                // set collision flag to 0
                cpu.V[0xF] = 0;

                // loop over each row
                for (int yline = 0; yline < N; yline++) {
                    // fetch the pixel value from the memory starting at location I
                    px = cpu.RAM[cpu.I + yline];

                    // loop over 8 bits of one row
                    for (int xline = 0; xline < 8; xline++) {
                        // check if current evaluated pixel is set to 1
                        // (0x80 >> xline) scans throught the byte, one bit at the time
                        if ((px & (0x80 >> xline)) != 0) {
                            // if drawing causes any pixel to be erased set the
                            // collision flag to 1
                            if (cpu.display[(*X + xline + ((*Y + yline) * WIDTH))] == 1) {
                                cpu.V[0xF] = 1;
                            }

                            // set pixel value by using XOR
                            cpu.display[*X + xline + ((*Y + yline) * 64)] ^= 1;
                        }
                    }
                }

                render_display();
                break;
            }
            case(0XE): {
                /* EX9E - Key    | skip ins, if key == VX     *
                 * EXA1 - Key    | skip ins, if key != VX     */
                switch(cpu.CIR & 0x00FF) {
                    case (0X9E):
                        cpu.PC += (cpu.keyboard[*X]) ? 2: 0;
                        break;
                    case (0XA1):
                        cpu.PC += (!cpu.keyboard[*X]) ? 2: 0;
                        break;
                }

                break;
            }
            case(0XF): {
                /* FX07 - Timer  | sets VX to value of DT     *
                 * FX0A - KeyOp  | wait key, store in VX      *
                 * FX15 - Timer  | sets delay timer to VX     *
                 * FX18 - Sound  | sets sound timer to VX     *
                 * FX1E - MEM    | adds VX to I               *
                 * FX29 - MEM    | sets I to addr of VX spr   *
                 *                 0X50 is the pos of char 'F'*
                 * FX33 - BCD    | stores binary-coded-decimal*
                 *                 of VX                      *
                 * FX55 - MEM    | dump V0-VX in mem from I   *
                 * FX65 - MEM    | fill V0-VX from mem I      */

                switch (cpu.CIR & 0X00FF) {
                    case (0X07):       *X = cpu.DT; break;
                    case (0X0A): {
                        bool keypressed = false;
                        for (int i = 0; i < 16; i++)
                            if (cpu.keyboard[i]) {
                                *X = i;
                                keypressed = true;
                                break;
                            }

                        if (!keypressed) cpu.PC -= 2;

                        break;
                    }
                    case (0X15):       cpu.DT = *X; break;
                    case (0X18):       cpu.ST = *X; break;
                    case (0X1E):       cpu.I += *X; break;
                    case (0X29): cpu.I = 0X50 / *X; break;
                    case (0X33):
                        cpu.RAM[cpu.I] = (*X % 1000) / 100;
                        cpu.RAM[cpu.I + 1] = (*X % 100) / 10;
                        cpu.RAM[cpu.I + 2] = (*X % 10);
                        break;
                    case (0X55):
                        for (int i = 0; i <= ((cpu.CIR & 0X0F00) >> 8); i++)
                            cpu.RAM[cpu.I + i] = cpu.V[i];
                        break;
                    case (0X65):
                        for (int i = 0; i <= ((cpu.CIR & 0X0F00) >> 8); i++)
                            cpu.V[i] = cpu.RAM[cpu.I + i];
                        break;
                }
                break;
            }
        }

        SDL_Delay(4);
    }

    kill();
    return 0;
}

SDL_Window *win;
SDL_Renderer *rend;

int init_display() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "[Error]: chip-8.c: SDL could not Initialize, SDL error %s\n", SDL_GetError());
        return 1;
    }

    if ((win = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH * 8, HEIGHT * 8, SDL_WINDOW_SHOWN)) == NULL) {
        fprintf(stderr, "[Error] chip-8.c: SDL could not create window, SDL error %s\n", SDL_GetError());
        return 1;
    }

    if ((rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED)) == NULL) {
        fprintf(stderr, "[Error] chip-8.c: SDL could not make renderer, SDL_error: %s\n", SDL_GetError());
        return 1;
    }

    return 0;
}

int render_display() {
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);

    // clear the current rendering target with the drawing color
    SDL_RenderClear(rend);

    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);

    // iterating thru the display (64*32)
    SDL_Rect rect;
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (cpu.display[x + (y * WIDTH)]) {
                rect.x = x * 8;
                rect.y = y * 8;
                rect.w = 8;
                rect.h = 8;

                SDL_RenderFillRect(rend, &rect);
            }
        }
    }

    // update the screen
    SDL_RenderPresent(rend);
    return 0;
}

void getkey() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case(SDL_QUIT):
                emulate = false;
                break;
            case (SDL_KEYDOWN):
                switch (event.key.keysym.sym) {
                    case SDLK_1: cpu.keyboard[0X1] = true; break;
                    case SDLK_2: cpu.keyboard[0X2] = true; break;
                    case SDLK_3: cpu.keyboard[0X3] = true; break;
                    case SDLK_4: cpu.keyboard[0XC] = true; break;

                    case SDLK_q: cpu.keyboard[0X4] = true; break;
                    case SDLK_w: cpu.keyboard[0X5] = true; break;
                    case SDLK_e: cpu.keyboard[0X6] = true; break;
                    case SDLK_r: cpu.keyboard[0XD] = true; break;

                    case SDLK_a: cpu.keyboard[0X7] = true; break;
                    case SDLK_s: cpu.keyboard[0X8] = true; break;
                    case SDLK_d: cpu.keyboard[0X9] = true; break;
                    case SDLK_f: cpu.keyboard[0XE] = true; break;

                    case SDLK_z: cpu.keyboard[0XA] = true; break;
                    case SDLK_x: cpu.keyboard[0X0] = true; break;
                    case SDLK_c: cpu.keyboard[0XB] = true; break;
                    case SDLK_v: cpu.keyboard[0XF] = true; break;
                }

                break;
            case (SDL_KEYUP):
                switch (event.key.keysym.sym) {
                    case SDLK_1: cpu.keyboard[0X1] = false; break;
                    case SDLK_2: cpu.keyboard[0X2] = false; break;
                    case SDLK_3: cpu.keyboard[0X3] = false; break;
                    case SDLK_4: cpu.keyboard[0XC] = false; break;

                    case SDLK_q: cpu.keyboard[0X4] = false; break;
                    case SDLK_w: cpu.keyboard[0X5] = false; break;
                    case SDLK_e: cpu.keyboard[0X6] = false; break;
                    case SDLK_r: cpu.keyboard[0XD] = false; break;

                    case SDLK_a: cpu.keyboard[0X7] = false; break;
                    case SDLK_s: cpu.keyboard[0X8] = false; break;
                    case SDLK_d: cpu.keyboard[0X9] = false; break;
                    case SDLK_f: cpu.keyboard[0XE] = false; break;

                    case SDLK_z: cpu.keyboard[0XA] = false; break;
                    case SDLK_x: cpu.keyboard[0X0] = false; break;
                    case SDLK_c: cpu.keyboard[0XB] = false; break;
                    case SDLK_v: cpu.keyboard[0XF] = false; break;
                }

                break;
        }
    }

    return;
}

int kill() {
    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(rend);
    win = NULL;
    rend = NULL;
    SDL_Quit();
    return 0;
}
