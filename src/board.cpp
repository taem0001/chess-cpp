#include "../include/board.h"
#include "../include/piece.h"
#include "../include/utils.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#define START_POS "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w"

void Board::setup_board() {
    load_pos(START_POS);
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

// TODO: Finish implementing the entirety of the FEN-notation
void Board::load_pos(const char *fen) {
    int index = 0;
    int spaces = 0;
    unsigned char piece, temp;

    for (int i = 0; i < 64; i++) {
        board[i] = Piece::none;
    }

    while (*fen) {
        switch (*fen) {
            case 'p':
                piece = Piece::make_piece(Piece::pawn, Piece::black);
                if (index / 8 != 1) {
                    temp = Piece::set_piece_first_move(piece);
                    piece = temp;
                }
                board[index] = piece;
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
                piece = Piece::make_piece(Piece::pawn, Piece::white);
                if (index / 8 != 6) {
                    temp = Piece::set_piece_first_move(piece);
                    piece = temp;
                }
                board[index] = piece;
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
            case '1' ... '8':
                index += (*fen - '0');
                break;
            case ' ':
                if (spaces == 0) {
                    ++fen;
                    if (*fen == 'w') {
                        white_to_move = true;
                    } else if (*fen == 'b') {
                        white_to_move = false;
                    } else {
                        std::cerr << "Invalid FEN character: " << *fen << std::endl;
                    }
                }
                break;
            default:
                std::cerr << "Invalid FEN character: " << *fen << std::endl;
                return;
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
        if (board[index] == Piece::none) {
            spacing++;
        } else {
            if (spacing > 0) {
                res += std::to_string(spacing);
                spacing = 0;
            }
            res += Piece::get_symbol(board[index]);
        }

        if (index % 8 == 7) {
            if (spacing > 0) {
                res += std::to_string(spacing);
                spacing = 0;
            }
            if (index != 63)
                res += "/";
        }

        index++;
    }

    res += " ";
    res += white_to_move ? "w" : "b";

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
            unsigned char type = Piece::get_type(piece);
            if (type == Piece::queen || type == Piece::bishop || type == Piece::rook) {
                generate_sliding_moves(i, moves);
            } else if (type == Piece::knight) {
                generate_knight_moves(i, moves);
            } else if (type == Piece::pawn) {
                generate_pawn_moves(i, moves);
            } else {
                generate_king_moves(i, moves);
            }
        }
    }

    return moves;
}

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

void Board::change_turn() {
    white_to_move = !white_to_move;
}

bool Board::get_turn() {
    return white_to_move;
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
        for (int j = 1; j <= squares_to_edges[start_square][i]; j++) {
            int end_square = start_square + Piece::dir[i] * j;
            unsigned char piece_on_end_square = board[end_square];

            // Blocked by friendly piece
            if (Piece::is_friendly(piece, piece_on_end_square)) {
                break;
            }
            moves.push_back({start_square, end_square});

            // Enemy piece on end square
            if (!Piece::is_friendly(piece, piece_on_end_square) && piece_on_end_square != Piece::none) {
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

// TODO: Implement en passant
void Board::generate_pawn_moves(int start_square, std::vector<Move> &moves) {
    unsigned char pawn = board[start_square];

    int dir = Piece::is_piece_white(pawn) ? 1 : -1;

    // Single step forward
    int one_step = start_square + dir * Piece::pawn_dir[0];
    if (in_bounds(one_step) && board[one_step] == Piece::none) {
        moves.push_back({start_square, one_step});

        // Double step forward (only if the single step was possible)
        if (!Piece::has_piece_moved(pawn)) {
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

// TODO: include castling
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
