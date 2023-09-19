#include "main.h"

int map[BLK_HGHT][BLK_WDTH];
shape_t shape_varients[SHAPE_VARIANTS];

pnt_t map_size = {BLK_WDTH, BLK_HGHT};

int init() {
    // VIDEO SETTINGS
    initscr(); raw(); noecho(); curs_set(0); start_color();

    if (has_colors() == FALSE) {
        endwin();
        fprintf(stderr, "[Error]: Terminal does not support colour!\n");
        return 1;
    }

    init_pair(0, COLOR_BLACK, COLOR_BLACK);
    init_pair(1, COLOR_GREEN, COLOR_GREEN);
    init_pair(2, COLOR_BLUE, COLOR_BLUE);
    init_pair(3, COLOR_RED, COLOR_RED);
    init_pair(4, COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(10, COLOR_WHITE, COLOR_WHITE);


    // GAME SETTINGS
    for (int y = 0; y < BLK_HGHT; y++) {
        for (int x = 0; x < BLK_WDTH; x++) {
            map[y][x] = 0;
        }
    }

    shape_varients[0] = (shape_t) {1, (pnt_t) {2, 1}, {0, 0}, 4, {{0, 0}, {0, 1}, {1, 0}, {1, 1}}};
    shape_varients[1] = (shape_t) {2, (pnt_t) {2, 1}, {0, 0}, 4, {{0, 0}, {0, 1}, {1, 0}, {2, 0}}};
    shape_varients[2] = (shape_t) {3, (pnt_t) {2, 1}, {0, 0}, 4, {{0, 0}, {1, 0}, {2, 0}, {3, 0}}};
    shape_varients[3] = (shape_t) {4, (pnt_t) {2, 1}, {0, 0}, 4, {{0, 0}, {0, 1}, {1, 1}, {1, 2}}};

    return  0;
}


int map_shape(shape_t shp) {
    pnt_t p = {0, 0};

    // removes shape from previous location
    for (int i = 0; i < shp.block_num; i++) {
        p = (pnt_t) MAP_PNT(shp.prev, shp.blocks[i]);

        map[p.y][p.x] = 0;
    }

    // places shape in new location
    for (int i = 0; i < shp.block_num; i++) {
        p = (pnt_t) MAP_PNT(shp.pos, shp.blocks[i]);

        map[p.y][p.x] = shp.name;
    }


    return 0;
}

/*
** RETURN:
** - 1 : out of bounds
** - 2 : hit other block || hit bottom
 */
int collision(shape_t shp, pnt_t move) {
    pnt_t p;

    for (int i = 0; i < shp.block_num; i++) {
        p = (pnt_t) MAP_PNT( move, shp.blocks[i] );

        // if reaches bottom
        if (!(p.y < BLK_HGHT)) return 2;

        // out of bounds
        if (!((p.x >= 0 && p.x < BLK_WDTH) && (p.y >= 0 && p.y < BLK_HGHT)))
            return 1;

        // another block
        if (map[p.y][p.x] != 0) {
            if (map[p.y][p.x] == shp.name) continue;
            return 2;
        }
    }

    return false;
}

/*
** RETURN:
** - 1: Cannot move current shape to "move"
** - 2: Create new shape for player
** - 3: Reached top of play screen
*/
int move_shape(shape_t *shp, pnt_t move) {

    switch (collision(*shp, (pnt_t) ADD_PNT(shp->pos, move))) {
        case 1: return 1; break;
        case 2: return 2; break;
        case 3: return 3; break;
        default: break;
    }

    shp->prev = shp->pos;
    shp->pos = (pnt_t) ADD_PNT(shp->pos, move);

    map_shape(*shp);
    return 0;
}

int rotate_shape(shape_t *shp) {
    pnt_t p;
    pnt_t rot_matrix[2] = {(pnt_t) {0, -1}, (pnt_t) {1, 0}};

    // removes shape from previous location
    for (int i = 0; i < shp->block_num; i++) {
        p = (pnt_t) MAP_PNT(shp->pos, shp->blocks[i]);

        map[p.y][p.x] = 0;
    }

    for (int i = 0; i < shp->block_num; i++) {
        p = shp->blocks[i];
        shp->blocks[i] = (pnt_t) MATRIX_MUL(rot_matrix, p);
    }

    return 0;
}

int set_shape(shape_t shp) {
    pnt_t p;
    // sets shape into place
    for (int i = 0; i < shp.block_num; i++) {
        p = (pnt_t) MAP_PNT(shp.pos, shp.blocks[i]);

        map[p.y][p.x] = 10;
    }

    return 0;
}

void game_over(WINDOW *tetris, int points) {
    erase();
    werase(tetris);

    mvwprintw(tetris, 0, 0, "GAME OVER");
    mvwprintw(tetris, 0, 0, "SCORE: %d", points);

    refresh();
    getch();
    return;
}

int main(int argc, char *argv[]) {
    init();

    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    int top_h = (yMax - BLK_HGHT)/2 - 1;
    int top_w = (xMax - BLK_WDTH)/2 - 1;

    WINDOW *tetris = newwin(BLK_HGHT + 2, BLK_WDTH + 2, top_h, top_w);
    box(tetris, 0, 0);

    refresh();
    wrefresh(tetris);

    bool quit;
    int points;

    // current player shape variables
    int shp_var = 0;                            // what shape variation
    shape_t shape;                              // current shape


    // set start conditions
    quit = false;
    points = 0;
    shape = shape_varients[shp_var++];
    shp_var %= 4;

    while (!quit) {
        pnt_t move;
        switch(getch()) {
            case 'q': quit = true; break;
            case 'r': rotate_shape(&shape); break;
            case 'd': move = (pnt_t) {1, 0}; break;
            case 's': move = (pnt_t) {0, 1}; break;
            case 'a': move = (pnt_t) {-1, 0}; break;
        }

        if (quit) break;

        switch(move_shape(&shape, move)) {
            case 1: break;
            case 2:
                // set shape to finished colour
                set_shape(shape);

                shape = shape_varients[shp_var++];
                shp_var %= 4;
                // checks if a line needs to be cleared
                bool clear;
                for (int mapY = BLK_HGHT - 1; mapY >= 0; mapY--) {
                    clear = true;
                    for (int mapX = 0; mapX < BLK_WDTH; mapX++) {
                        if (map[mapY][mapX] == 0) {
                            clear = false; break;
                        }
                    }

                    if (clear) {
                        points++;
                        for (int mapX = 0; mapX < BLK_WDTH; mapX++) {
                            map[mapY][mapX] = 0;
                        }
                    }
                }
                break;
        }

        for (int mapY = 0, mapX = 0; mapX < BLK_WDTH; mapX++)
            if (map[mapY][mapX] == 10) {
                game_over(tetris, points);
                quit = true;
                break;
            }

        for (int mapY = 0, colour = 0; mapY < BLK_HGHT; mapY++) {
            for (int mapX = 0; mapX < BLK_WDTH; mapX++) {
                colour = map[mapY][mapX];

                wattron(tetris, COLOR_PAIR(colour));
                mvwaddch(tetris, mapY+1, mapX+1, ' ');
                wattroff(tetris, COLOR_PAIR(colour));
            }
        }

        mvprintw(yMax/2, xMax/6, "Points: %d", points);

        wrefresh(tetris);
        refresh();
    }

    endwin();
    return 0;
}
