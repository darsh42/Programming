#include "main.h"

int board[WIDTH][HEIGHT];

void board_gen(int **board, int w, int h) {
    srand(time(NULL));

    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++)
            board[x][y] = (rand() < (RAND_MAX / 2));
    }

    return;
}

void board_display(int **board, int w, int h) {
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++)
            printf("%d", board[x][y]);
    }
}

int main(void) {



    return 0;
}
