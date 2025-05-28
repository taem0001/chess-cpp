#ifndef BOARD_H
#define BOARD_H
#include "utils.h"
#include "magic.h"

class Board {
private:
    u64 zobrist_pieces[12][64];
    u64 zobrist_blacktomove;
    u64 zobrist_castle[16];
    u64 zobrist_epfile[8];

    u64 bitboards[16];
    int piece_on_square[64];

public:
    u64 *get_bitboards();
    int *get_piece_on_squares();
    void draw_board();
    void move_piece(int, int);
    void promote_piece(bool, int, int);
    void add_piece(int, int);
    void remove_piece(int);
    void init_zobrist();
    u64 get_zobrist_piece(int, int);
    u64 get_zobrist_blacktomove();
    u64 get_zobrist_castle(int);
    u64 get_zobrist_ep(int);
};

#endif // !BOARD_H
