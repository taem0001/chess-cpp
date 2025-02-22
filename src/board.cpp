#include "../include/board.h"
#include "../include/piece.h"
#include "../include/utils.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#define START_POS "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"

Board::Board() {
    load_pos(START_POS, board);
    precompute_squares_to_edges();
    white_to_move = true;
}

void Board::precompute_squares_to_edges() {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            int dist_up = 7 - row;
            int dist_down = row;
            int dist_left = col;
            int dist_right = 7 - col;

            int index = row * 8 + col;

            squares_to_edges[index][0] = dist_down;
            squares_to_edges[index][1] = dist_up;
            squares_to_edges[index][2] = dist_left;
            squares_to_edges[index][3] = dist_right;
            squares_to_edges[index][4] = std::min(dist_left, dist_down);
            squares_to_edges[index][5] = std::min(dist_right, dist_up);
            squares_to_edges[index][6] = std::min(dist_right, dist_down);
            squares_to_edges[index][7] = std::min(dist_left, dist_up);
        }
    }
}

// TODO: Finish implementing the entirety of the FEN-notation
void Board::load_pos(const char *fen, unsigned char *board) {
    int index = 0;

    while (*fen) {
        switch (*fen) {
        case 'p':
            board[index] = Piece::make_piece(Piece::pawn, Piece::black);
            index++;
            break;
        case 'n':
            board[index] = Piece::make_piece(Piece::knight, Piece::black);
            index++;
            break;
        case 'b':
            board[index] = Piece::make_piece(Piece::bishop, Piece::black);
            index++;
            break;
        case 'r':
            board[index] = Piece::make_piece(Piece::rook, Piece::black);
            index++;
            break;
        case 'q':
            board[index] = Piece::make_piece(Piece::queen, Piece::black);
            index++;
            break;
        case 'k':
            board[index] = Piece::make_piece(Piece::king, Piece::black);
            index++;
            break;
        case 'P':
            board[index] = Piece::make_piece(Piece::pawn, Piece::white);
            index++;
            break;
        case 'N':
            board[index] = Piece::make_piece(Piece::knight, Piece::white);
            index++;
            break;
        case 'B':
            board[index] = Piece::make_piece(Piece::bishop, Piece::white);
            index++;
            break;
        case 'R':
            board[index] = Piece::make_piece(Piece::rook, Piece::white);
            index++;
            break;
        case 'Q':
            board[index] = Piece::make_piece(Piece::queen, Piece::white);
            index++;
            break;
        case 'K':
            board[index] = Piece::make_piece(Piece::king, Piece::white);
            index++;
            break;
        case '/':
            break;
        case '1':
            board[index] = Piece::none;
            index++;
            break;
        case '2':
            for (int i = index; i < index + 2; i++) {
                board[i] = Piece::none;
            }
            index += 2;
            break;
        case '3':
            for (int i = index; i < index + 3; i++) {
                board[i] = Piece::none;
            }
            index += 3;
            break;
        case '4':
            for (int i = index; i < index + 4; i++) {
                board[i] = Piece::none;
            }
            index += 4;
            break;
        case '5':
            for (int i = index; i < index + 5; i++) {
                board[i] = Piece::none;
            }
            index += 5;
            break;
        case '6':
            for (int i = index; i < index + 6; i++) {
                board[i] = Piece::none;
            }
            index += 6;
            break;
        case '7':
            for (int i = index; i < index + 7; i++) {
                board[i] = Piece::none;
            }
            index += 7;
            break;
        case '8':
            for (int i = index; i < index + 8; i++) {
                board[i] = Piece::none;
            }
            index += 8;
            break;
        }

        ++fen;
    }
}

// TODO: Finish implementing the entirety of the FEN-notation
std::string Board::write_fen() {
    int index = 0;
    int spacing = 0;
    std::string res = "";

    while (index < 64) {
        switch (board[index]) {
        case 0:
            spacing++;
            break;
        default:
            res += evaluate_fen_char(spacing, Piece::get_symbol(board[index]));
            break;
        }

        if (index % 8 == 7 && index != 63) {
            if (spacing > 0) {
                res += std::to_string(spacing);
                spacing = 0;
            }
            res += "/";
        }

        index++;
    }

    return res;
}

void Board::draw_board() {
    for (int i = 0; i < 64; i++) {
        std::cout << Piece::get_symbol(board[i]) << " ";

        if (i % 8 == 7) {
            std::cout << std::endl;
        }
    }
}

unsigned char *Board::get_board() {
    return board;
}

std::vector<Move> Board::generate_legal_moves() {
    std::vector<Move> moves;

    for (int i = 0; i < 64; i++) {
        unsigned char piece = board[i];

        if (piece == Piece::none) {
            continue;
        }

        if ((Piece::is_piece_white(piece) && white_to_move) || (!Piece::is_piece_white(piece) && !white_to_move)) {
            if (Piece::is_sliding_piece(piece)) {
                generate_sliding_moves(i, moves);
            } else if (Piece::get_type(piece) == Piece::knight) {
                generate_knight_moves(i, moves);
            } else if (Piece::get_type(piece) == Piece::pawn) {
                generate_pawn_moves(i, moves);
            } else {
                generate_king_moves(i, moves);
            }
        }
    }

    return moves;
}

// Private functions
void Board::generate_sliding_moves(int start_square, std::vector<Move> &moves) {
    unsigned char piece = board[start_square];
    int start_index = 0;
    int end_index = 8;

    if (Piece::get_type(piece) == Piece::bishop) {
        start_index = 4;
    }
    if (Piece::get_type(piece) == Piece::rook) {
        end_index = 4;
    }

    for (int i = start_index; i < end_index; i++) {
        for (int j = 0; j < squares_to_edges[start_square][i]; j++) {
            int end_square = start_square + Piece::dir[i] * (j + 1);
            unsigned char piece_on_end_square = board[end_square];

            if (!in_bounds(end_square)) {
                continue;
            }

            // Blocked by friendly piece
            if (Piece::is_friendly(piece, piece_on_end_square)) {
                break;
            }

            moves.push_back({start_square, end_square});

            // Enemy piece on end square
            if (!Piece::is_friendly(piece, piece_on_end_square)) {
                break;
            }
        }
    }
}

void Board::generate_knight_moves(int start_square, std::vector<Move> &moves) {
    unsigned char knight = board[start_square];

    for (int i = 0; i < 8; i++) {
        int end_square = start_square + Piece::knight_dir[i];

        if (!in_bounds(end_square) || !knight_in_bounds(start_square, end_square)) {
            continue;
        }

        unsigned char piece_on_end_square = board[end_square];

        // Blocked by friendly piece
        if (Piece::is_friendly(knight, piece_on_end_square)) {
            continue;
        }

        moves.push_back({start_square, end_square});
    }
}

void Board::generate_pawn_moves(int start_square, std::vector<Move> &moves) {
    unsigned char pawn = board[start_square];

    int dir = Piece::is_piece_white(pawn) ? 1 : -1;

    // Single step forward
    int one_step = start_square + dir * Piece::pawn_dir[0];
    if (in_bounds(one_step) && board[one_step] == Piece::none) {
        moves.push_back({start_square, one_step});

        // Double step forward (only if the single step was possible)
        if (!Piece::has_pawn_moved(pawn)) {
            int two_step = start_square + dir * Piece::pawn_dir[3];
            if (in_bounds(two_step) && board[two_step] == Piece::none) {
                moves.push_back({start_square, two_step});
            }
        }
    }

    // Diagonal captures
    for (int i = 1; i <= 2; i++) {
        int capture_square = start_square + dir * Piece::pawn_dir[i];

        if (in_bounds(capture_square)) {
            unsigned char piece_on_target = board[capture_square];

            if (!Piece::is_friendly(pawn, piece_on_target) && piece_on_target != Piece::none) {
                moves.push_back({start_square, capture_square});
            }
        }
    }
}

void Board::generate_king_moves(int start_square, std::vector<Move> &moves) {
    unsigned char king = board[start_square];

    for (int i = 0; i < 8; i++) {
        int end_square = start_square + Piece::dir[i];

        if (!in_bounds(end_square)) {
            continue;
        }

        unsigned char piece_on_end_square = board[end_square];

        if (Piece::is_friendly(king, piece_on_end_square)) {
            continue;
        }

        moves.push_back({start_square, end_square});
    }
}
