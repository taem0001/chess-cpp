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

Move convert_pos(const std::string &pos) {
    if (pos.length() != 4) {
        std::cerr << "Invalid position length" << std::endl;
        return {};
    }

    Move res;
    int squares[2];
    for (int i = 0; i < 2; i++) {
        char file = pos[i * 2];     // 'a'-'h'
        char rank = pos[i * 2 + 1]; // '1'-'8'

        if (file < 'a' || file > 'h' || rank < '1' || rank > '8') {
            std::cerr << "Invalid character in position" << std::endl;
            return {};
        }

        int col = file - 'a';
        int row = 8 - (rank - '0');

        squares[i] = row * 8 + col;
    }

    res.start_square = squares[0];
    res.end_square = squares[1];
    return res;
}
