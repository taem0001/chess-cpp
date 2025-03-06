#ifndef BOARD_H
#define BOARD_H
#include "utils.h"

class Board {
private:
    u64 bitboards[16];
    bool white_turn;

public:
    u64 *get_bitboards();
};

#endif // !BOARD_H
