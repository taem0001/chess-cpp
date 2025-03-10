#ifndef BOARD_H
#define BOARD_H
#include "utils.h"

class Board {
private:
    u64 bitboards[16];

public:
    u64 *get_bitboards();
    void draw_board();
};

#endif // !BOARD_H
