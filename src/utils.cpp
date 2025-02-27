#include "../include/utils.h"
#include <iostream>

bool in_bounds(int square) {
    return (square >= 0 && square < 64);
}

bool knight_in_bounds(int start_square, int end_square) {
    int s_row = start_square / 8;
    int s_col = start_square % 8;
    int e_row = end_square / 8;
    int e_col = end_square % 8;

    int row_diff = std::abs(s_row - e_row);
    int col_diff = std::abs(s_col - e_col);

    return (row_diff == 2 && col_diff == 1) || (row_diff == 1 && col_diff == 2);
}

void load_player_turn(char fen, bool *turn) {
    if (fen == 'w') {
        *turn = true;
    } else if (fen == 'b') {
        *turn = false;
    } else {
        std::cerr << "Invalid FEN character: " << fen << std::endl;
    }
}
