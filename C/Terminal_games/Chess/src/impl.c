#include "main.h"
#include <stdlib.h>

extern piece_t white[16];
extern piece_t black[16];
extern int num_taken_white;
extern int num_taken_black;
extern char taken_white[16];
extern char taken_black[16];
extern piece_t tmp;
extern piece_t empty;
extern piece_t *board[BRD_H * BRD_W];

int place_piece(piece_t *piece) {
    if (piece->name == EMPTY)
        return 1;

    board[MAP(piece->pos)] = piece;

    return 0;
}

/*
** RETURN:
** 0 - No pieces taken
** 1 - Piece taken
*/
int take_piece(piece_t *piece, piece_t *taken[16], int *num_taken, piece_t *enemy_team[16], pnt_t move) {

    for (int i = 0; i < 16; i++) {
        if (COM_PNT((*enemy_team)[i].pos, move) == 0) {
            (*taken)[(*num_taken)++] = (*enemy_team)[i];
            (*enemy_team)[i] = empty;
            return 1;
        }
    }



    return 0;
}

int move_piece(piece_t *piece, pnt_t move) {
    piece_t *pos = board[MAP(move)];

    if ( pos == NULL)
        return 1;

    if (piece->team == WHITE) {
        for (int i = 0; i < 16; i++) {
            if (COM_PNT(black[i].pos, move)) {
                taken_white[num_taken_white++] = black[i].sym;
                black[i] = empty;
                break;
            }
        }
    }

    if (piece->team == BLACK) {
        for (int i = 0; i < 16; i++) {
            if (COM_PNT(white[i].pos, move)) {
                taken_black[num_taken_black++] = white[i].sym;
                white[i] = empty;
                break;
            }
        }
    }

    board[MAP(piece->pos)] = NULL;
    piece->pos = move;

    return 0;
}

/*
** Knight movement
*/
int possible_moves_knight(piece_t *piece) {
    pnt_t p;
    pnt_t right_L = {-1, 2};
    pnt_t left_L = {1, 2};

    int moves = 0;
    for (int i = 0; i < 4; i++) {
        // set tmp on board for right L
        right_L = (pnt_t) ROT_90(right_L);
        p = ADD_PNT(piece->pos, right_L);

        if (BOUNDS(p)) {
            if ( board[MAP(p)] == NULL || !(board[MAP(p)]->team == piece->team)) {
                board[MAP(p)] = &tmp;
                moves++;
            }
        }

        // set tmp on board for left L
        left_L = (pnt_t) ROT_90(left_L);
        p = ADD_PNT(piece->pos, left_L);

        if (BOUNDS(p)) {
            if ( board[MAP(p)] == NULL || !(board[MAP(p)]->team == piece->team)) {
                board[MAP(p)] = &tmp;
                moves++;
            }
        }
    }

    return moves;
}

/*
** Bishop movement
*/
int possible_moves_bishop(piece_t *piece) {
    pnt_t p;
    pnt_t vec = {1, 1};

    int moves = 0;
    for (int i = 0; i < 4; i++) {
        vec = (pnt_t) ROT_90(vec);
        for (p = ADD_PNT(piece->pos, vec); BOUNDS(p) ; p = ADD_PNT(p, vec)) {
            piece_t *place = board[MAP(p)];
            // multiply the vec vector ->
            // add to the current piece pos ->
            // set to TMP on board
            if (place == NULL) {
                board[MAP(p)] = &tmp;
                moves++;
                continue;
            }

            if (place->team != piece->team) {
                board[MAP(p)] = &tmp;
                moves++;
            }
            break;
        }
    }

    return moves;
}

/*
** Rook movement
*/
int possible_moves_rook(piece_t *piece) {
    pnt_t p;
    pnt_t vec = {0, 1};

    int moves = 0;
    for (int i = 0; i < 4; i++) {
        vec = (pnt_t) ROT_90(vec);
        for (p = ADD_PNT(piece->pos, vec); BOUNDS(p) ; p = ADD_PNT(p, vec)) {
            piece_t *place = board[MAP(p)];
            // multiply the vec vector ->
            // add to the current piece pos ->
            // set to TMP on board
            if (place == NULL) {
                board[MAP(p)] = &tmp;
                moves++;
                continue;
            }

            if (place->team != piece->team) {
                board[MAP(p)] = &tmp;
                moves++;
            }
            break;
        }
    }

    return moves;
}

/*
** Queen movement
*/
int possible_moves_queen(piece_t *piece) {
    int moves = 0;
    moves += possible_moves_bishop(piece);
    moves += possible_moves_rook(piece);
    return moves;
}

/*
** King movement
*/
int possible_moves_king(piece_t *piece) {
    int moves = 0;

    pnt_t p;
    pnt_t straight = {1, 0};
    pnt_t diagonal = {1, 1};

    for (int i = 0; i < 4; i++) {
        p = ADD_PNT(piece->pos, straight);

        if (BOUNDS(p)) {
            if (board[MAP(p)] == NULL || board[MAP(p)]->team != piece->team) {
                board[MAP(p)] = &tmp;
                moves++;
            }
        }

        straight = (pnt_t) ROT_90(straight);

        p = ADD_PNT(piece->pos, diagonal);

        if (BOUNDS(p)) {
            if (board[MAP(p)] == NULL || board[MAP(p)]->team != piece->team) {
                board[MAP(p)] = &tmp;
                moves++;
            }
        }

        diagonal = (pnt_t) ROT_90(diagonal);
    }


    return moves;
}

/*
** Pawn movement
*/
int possible_moves_pawn(piece_t *piece) {
    int moves = 0;
    pnt_t move_fwd = (piece->team == WHITE) ? (pnt_t) {1, 0}: (pnt_t) {-1, 0};
    pnt_t take_top = (piece->team == WHITE) ? (pnt_t) {1, -1}: (pnt_t) {-1, -1};
    pnt_t take_bot = (piece->team == WHITE) ? (pnt_t) {1, 1}: (pnt_t) {-1, 1};
    move_fwd = ADD_PNT(piece->pos, move_fwd);
    take_top = ADD_PNT(piece->pos, take_top);
    take_bot = ADD_PNT(piece->pos, take_bot);

    if (BOUNDS(move_fwd)) {
        if (board[MAP(move_fwd)] == NULL) {
            board[MAP(move_fwd)] = &tmp;
            moves++;
        }
    }

    if (BOUNDS(take_top)) {
        if (board[MAP(take_top)] != NULL && board[MAP(take_top)]->team != piece->team ) {
            board[MAP(take_top)] = &tmp;
            moves++;
        }
    }

    if (BOUNDS(take_bot)) {
        if (board[MAP(take_bot)] != NULL && board[MAP(take_bot)]->team != piece->team ) {
            board[MAP(take_bot)] = &tmp;
            moves++;
        }
    }
    return moves;
}

int is_check() {


    return 0;
}
