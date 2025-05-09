#ifndef BOARD_H
#define BOARD_H
#include "utils.h"

class Board {
private:
    u64 bitboards[16];
    int piece_on_square[64];

public:
    u64 *get_bitboards();
    int *get_piece_on_squares();
    void draw_board();
    void move_piece(int, int);
    void promote_piece(bool, char, int);
    void remove_piece(int);
};

#endif // !BOARD_H
