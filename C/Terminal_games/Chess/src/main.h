#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>

#define BRD_H 8
#define BRD_W 8

#define MAP(p) p.y * BRD_W + p.x
#define BOUNDS(p) ((p.x >=0 && p.x < BRD_W) && (p.y >= 0 && p.y < BRD_H))
#define ROT_90(a) {-a.y, a.x} // omitted 0 in calc
#define ADD_PNT(a, b) (pnt_t) {a.x + b.x, a.y + b.y}
#define COM_PNT(a, b) (a.x == b.x && a.y == b.y)
#define MUL_PNT_CONST(a, c) (pnt_t) {a.x * c, a.y * c}

// pieces
#define TMP     7
#define KING    6
#define QUEEN   5
#define BISHOP  4
#define KNIGHT  3
#define ROOK    2
#define PAWN    1
#define EMPTY   0

#define WHITE   1
#define BLACK   2

typedef struct point {
    int x, y;
} pnt_t;

typedef struct piece {
    char sym;
    int name;
    int team;
    pnt_t pos;
    // function pointer to move type
    int (*possible)(struct piece *);
} piece_t;

int possible_moves_rook(piece_t *p);
int possible_moves_king(piece_t *p);
int possible_moves_pawn(piece_t *p);
int possible_moves_queen(piece_t *p);
int possible_moves_bishop(piece_t *p);
int possible_moves_knight(piece_t *p);

int move_piece(piece_t *piece, pnt_t move);
int place_piece(piece_t *piece);

#endif // MAIN_H_INCLUDED
