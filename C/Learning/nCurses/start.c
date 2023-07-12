#include <ncurses.h>
#include <string.h>
#include <stdio.h>

#define ITEM_COUNT 6
#define ASM_SIZE 10

void menu(int option, char *menuItems[ITEM_COUNT]);
void source_print(int PC, int source_size, char source[0XFFFF][30]);
int main( int argc, char *argv[] ) {
    if (argc != 2) {printf("Usage: ./start asm.txt\n"); return 1;}

    // initialization functions
    initscr(); raw(); noecho(); curs_set(0); start_color();

    int quit = 0, ch, PC, asm_size = 0;
    int maxY, maxX;
    FILE *asm_code = fopen(argv[1], "r");

    char *menuItems[ITEM_COUNT] = {"(c)ontinue", "(r)eset", "(i)rq", "(n)mi", "(R)efresh", "(q)uit"};
    char source[0XFFFF][30];

    for (int asm_size = 0; asm_size < 0XFFFF && fgets(source[asm_size], 30, asm_code) != NULL; asm_size++)
        ;

    do {
        getmaxyx(stdscr, maxY, maxX);

        if (maxY < 40 || maxX < 120) {
            clear();
            printw("Screen too small, please re-size to at least 120x40\n");
        } else {
            source_print(PC, asm_size, source);
            menu(ch, menuItems);
            box(stdscr, 0, 0);
        }

        refresh();
        ch = getch();

        switch (ch) {
            case('q'):
                quit = 1;
                break;
            case('c'):
                PC++;
                break;
            case('R'):
                clear();
            default:
                break;
        }

    } while (!quit);


    endwin();

    return 0;
}

void menu(int option, char *menuItems[ITEM_COUNT]) {
    int x, y;
    getmaxyx(stdscr, y, x);

    for (int i = 0, offsetY = y - 2, offsetX = 3; i < ITEM_COUNT; offsetX += (strlen(menuItems[i++]) + 1)) {
        if (option == menuItems[i][1]) {
            attron(A_BOLD);
            mvprintw(offsetY, offsetX, "%s", menuItems[i]);
            attroff(A_BOLD);
        } else {
            mvprintw(offsetY, offsetX, "%s", menuItems[i]);
        }
    }
}

void source_print(int PC, int source_size, char source[0XFFFF][30]) {
    int x, y;
    getmaxyx(stdscr, y, x);
    for (int i = PC - ASM_SIZE, offsetY = y/2 - ASM_SIZE, offsetX = 3*(x/4); i <= PC + ASM_SIZE; i++, offsetY++) {
        if (i < 0) {
            mvprintw(offsetY, offsetX, "------------------------------");
            continue;
        }

        if (i == PC) {
            init_pair(1, COLOR_WHITE, COLOR_MAGENTA);
            attron(COLOR_PAIR(1));
            mvprintw(offsetY, offsetX - 4, "PC> %s", source[i]);
            attroff(COLOR_PAIR(1));
            continue;
        }

        mvprintw(offsetY, offsetX, "%s", source[i]);
    }
}
