#include "../include/board.h"
#include "../include/piece.h"
#include "../include/utils.h"
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

// TODO: Finish this function
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
            }
        }
    }

    return moves;
}

unsigned char *Board::get_board() {
    return board;
}

// Private functions
void Board::generate_sliding_moves(int square, std::vector<Move> &moves) {
    unsigned char piece = board[square];
    int start_index = 0;
    int end_index = 8;

    if (Piece::get_type(piece) == Piece::bishop) {
        start_index = 4;
    }
    if (Piece::get_type(piece) == Piece::rook) {
        end_index = 4;
    }

    for (int i = start_index; i < end_index; i++) {
        for (int j = 0; j < squares_to_edges[square][i]; j++) {
            int end_square = square + dir[i] * (j + 1);
            unsigned char piece_on_end_square = board[end_square];

            // Blocked by friendly piece
            if (Piece::is_friendly(piece, piece_on_end_square)) {
                break;
            }

            moves.push_back({square, end_square});

            // Enemy piece on end square
            if (!Piece::is_friendly(piece, piece_on_end_square)) {
                break;
            }
        }
    }
}
