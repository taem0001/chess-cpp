#include "../include/utils.h"
#include "../include/piece.h"
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

void precompute_squares_to_edges(int (*arr)[8]) {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            int dist_south = 7 - row;
            int dist_north = row;
            int dist_east = 7 - col;
            int dist_west = col;

            int index = row * 8 + col;

            arr[index][0] = dist_south;
            arr[index][1] = dist_north;
            arr[index][2] = dist_west;
            arr[index][3] = dist_east;
            arr[index][4] = std::min(dist_west, dist_south);
            arr[index][5] = std::min(dist_east, dist_north);
            arr[index][6] = std::min(dist_east, dist_south);
            arr[index][7] = std::min(dist_west, dist_north);
        }
    }
}

int find_king_pos(unsigned char *board, unsigned char color) {
    for (int i = 0; i < 64; i++) {
        if (Piece::get_type(board[i]) == Piece::king) {
            if (color == Piece::white && Piece::is_piece_white(board[i])) {
                return i;
            }
            if (color == Piece::black && !Piece::is_piece_white(board[i])) {
                return i;
            }
        }
    }
    return -1;
}
