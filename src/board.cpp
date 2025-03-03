#include "../include/board.h"
#include "../include/piece.h"
#include "../include/utils.h"
#include <algorithm>
#include <iostream>

void Board::setup_board() {
    precompute_squares_to_edges(squares_to_edges);
}

void Board::precompute_squares_to_edges(int (*arr)[8]) {
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

int (*Board::get_precomputed_distances())[8] {
    return squares_to_edges;
}

void Board::draw_board() {
    char c;

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (i == 0 && j == 0) {
                c = ' ';
            } else if (j == 0) {
                c = '8' - (i - 1);
            } else if (i == 0) {
                c = j + 96;
            } else {
                c = Piece::get_symbol(board[(i - 1) * 8 + (j - 1)]);
            }
            std::cout << c << ' ';
        }
        std::cout << std::endl;
    }
}

unsigned char *Board::get_board() { return board; }

bool Board::is_move_legal(std::vector<Move> &moves, int s_square, int e_square) {
    for (Move move : moves) {
        if (move.start_square == s_square && move.end_square == e_square) {
            return true;
        }
    }
    return false;
}

void Board::move_piece(int s_square, int e_square) {
    if (!Piece::has_piece_moved(board[s_square])) {
        unsigned char temp = board[s_square];
        board[s_square] = Piece::set_piece_first_move(temp);
    }
    board[e_square] = board[s_square];
    board[s_square] = Piece::none;
}

void Board::change_turn() { white_to_move = !white_to_move; }

void Board::set_turn(bool b) {
    white_to_move = b;
}

bool Board::get_turn() { return white_to_move; }

void Board::set_white_castle_king(bool b) {
    white_castle_king = b;
}

void Board::set_white_castle_queen(bool b) {
    white_castle_queen = b;
}

void Board::set_black_castle_king(bool b) {
    black_castle_king = b;
}

void Board::set_black_castle_queen(bool b) {
    black_castle_queen = b;
}

void Board::set_board_element(unsigned char piece, int index) {
    board[index] = piece;
}

bool Board::get_black_castle_king() {
    return black_castle_king;
}

bool Board::get_white_castle_king() {
    return white_castle_king;
}

bool Board::get_black_castle_queen() {
    return black_castle_queen;
}

bool Board::get_white_castle_queen() {
    return white_castle_queen;
}
