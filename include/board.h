#ifndef BOARD_H
#define BOARD_H
#include "utils.h"

class Board {
private:
    u64 bitboards[16];

public:
    u64 *get_bitboards();
    void draw_board();
    void move_piece(int, int);
    void promote_piece(bool, char, int);
};

#endif // !BOARD_H
