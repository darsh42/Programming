#include "main.h"

int map[MAP_H * MAP_W];

int make_segment(seg_t *head, int *total_segments) {
    seg_t *cur;
    for (cur = head; cur->next != NULL; cur = cur->next)
        ;

    cur->next = malloc(sizeof(seg_t));

    cur->next->pos.x = (cur->pos.x + 1) % MAP_W;
    cur->next->pos.y = cur->pos.y % MAP_W;

    cur->next->next = NULL;

    (*total_segments)++;
    return 0;
}

int draw_segment(seg_t *head) {

    // clear screen
    for (int i = 0; i < MAP_W * MAP_H; i++)
        map[i] = 0;

    // place snake
    for (seg_t *s = head; s != NULL; s = s->next)
        map[MAP_COORDS(s->pos)] = SEG_SYM;

    return 0;
}

int move_segment(seg_t *head, pnt_t move) {
    head->prev = head->pos;
    pnt_t tmp = ADD_PNT(head->pos, move);
    head->pos.x = (tmp.x < 0) ? MAP_W + tmp.x: tmp.x % MAP_W;
    head->pos.y = (tmp.y < 0) ? MAP_H + tmp.y: tmp.y % MAP_H;

    for (seg_t *s = head, *n = s->next; n != NULL; s = s->next, n = n->next) {
        n->prev = n->pos;
        n->pos = s->prev;
    }

    return 0;
}

/*
** RETURN:
** 0 - hit nothing
** 1 - hit other snake segment
** 2 - hit apple
*/
int hits_segment(seg_t *head, pnt_t move) {
    pnt_t p = ADD_PNT(head->pos, move);
    p.x = (p.x < 0) ? MAP_W + p.x: p.x % MAP_W;
    p.y = (p.y < 0) ? MAP_H + p.y: p.y % MAP_H;

    switch (map[MAP_COORDS(p)]) {
        case(SEG_SYM): return 1;
        case(APP_SYM): return 2;
    }

    return 0;
}

int main(void) {
    initscr(); raw(); noecho(); curs_set(0); start_color();

    if (has_colors() == FALSE) {
        endwin();
        fprintf(stderr, "[Error]: Terminal does not support colour!\n");
        return 1;
    }

    init_pair(0, COLOR_BLACK, COLOR_BLACK);
    init_pair(1, COLOR_GREEN, COLOR_GREEN);
    init_pair(2, COLOR_RED, COLOR_RED);

    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    int top_h = (yMax - MAP_H)/2 - 1;
    int top_w = (xMax - MAP_W)/2 - 1;

    WINDOW *snake = newwin(MAP_H + 2, MAP_W + 2, top_h, top_w); box(snake, 0, 0);

    refresh();
    wrefresh(snake);

    bool quit;
    int points;

    seg_t head;
    int segments = 0;

    pnt_t move;
    pnt_t apple;
    bool taken = false;

    quit = false;
    points = 0;

    srand(time(NULL));
    apple = (pnt_t) {rand() % MAP_W, rand() % MAP_H};
    head = (seg_t) {NULL, (pnt_t) {0, 0}, (pnt_t) {0, 0}};
    while (!quit) {
        if (taken) {
            taken = false;
            srand(time(NULL));
            apple = (pnt_t) {rand() % MAP_W, rand() % MAP_H};
        }

        switch (getch()) {
            default:
            case 'q': quit = true; break;
            case 'd': move = (pnt_t) {1, 0}; break;
            case 's': move = (pnt_t) {0, 1}; break;
            case 'a': move = (pnt_t) {-1, 0}; break;
            case 'w': move = (pnt_t) {0, -1}; break;
        }

        if (quit) continue;

        switch (hits_segment(&head, move)) {
            case 1:
                quit = true; break;
            case 2:
                taken = true;
                points++;
                make_segment(&head, &segments);
            default:
                move_segment(&head, move);
        }

        if (quit) continue;

        draw_segment(&head);
        map[MAP_COORDS(apple)] = APP_SYM;

        for (int mapY = 0, colour = 0; mapY < MAP_H; mapY++) {
            for (int mapX = 0; mapX < MAP_W; mapX++) {
                pnt_t map_c = {mapX, mapY};
                colour = map[MAP_COORDS(map_c)];

                wattron(snake, COLOR_PAIR(colour));
                mvwaddch(snake, mapY+1, mapX+1, ' ');
                wattroff(snake, COLOR_PAIR(colour));
            }
        }

        mvprintw(yMax/2, xMax/8, "Points: %d", points);

        wrefresh(snake);
        refresh();
    }

    if (segments != 0) {
        seg_t *c = head.next, *n = c->next;
        for (int i = 0; i < segments - 1; i++, c = n, n = n->next)
            free(c);

        free(c);
        free(n);
    }

    delwin(snake);
    endwin();

    printf("Your score: %d\n", points);

    return 0;
}
