#ifndef BOARD_H
#define BOARD_H
#include "utils.h"

class Board {
private:
    u64 bitboards[15];

public:
    void init();
    u64 *get_bitboards();
};

#endif // !BOARD_H
