#include "../include/move.h"
#include "../include/piece.h"
#include "../include/utils.h"
#include <iostream>

// TODO: Fix check detection

std::vector<Move> MoveGenerator::generate_legal_moves(Board &board) {
    std::vector<Move> moves;
    unsigned char *chess_board = board.get_board();
    bool white_to_move = board.get_turn();

    for (int i = 0; i < 64; i++) {
        unsigned char piece = chess_board[i];

        if (piece == Piece::none) {
            continue;
        }

        unsigned char type = Piece::get_type(piece);
        if (type == Piece::queen || type == Piece::bishop || type == Piece::rook) {
            generate_sliding_moves(board, i, moves);
        } else if (type == Piece::knight) {
            generate_knight_moves(board, i, moves);
        } else if (type == Piece::pawn) {
            generate_pawn_moves(board, i, moves);
        } else {
            generate_king_moves(board, i, moves);
        }
    }

    return moves;
}

void MoveGenerator::handle_check(Board &board, std::vector<Move> &moves) {
    if (board.is_in_check(moves)) {
        std::string color = board.get_turn() ? "White " : "Black ";
        std::cout << color << "king in check!" << std::endl;
    }
}

void MoveGenerator::generate_sliding_moves(Board &board, int start_square, std::vector<Move> &moves) {
    unsigned char *chess_board = board.get_board();
    int(*squares_to_edges)[8] = board.get_precomputed_distances();
    unsigned char piece = chess_board[start_square];
    bool is_white = Piece::is_piece_white(piece);
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
            unsigned char piece_on_end_square = chess_board[end_square];

            // Blocked by friendly piece
            if (Piece::is_friendly(piece, piece_on_end_square)) {
                break;
            }
            moves.push_back({start_square, end_square, is_white});

            // Enemy piece on end square
            if (!Piece::is_friendly(piece, piece_on_end_square) && piece_on_end_square != Piece::none) {
                break;
            }
        }
    }
}

void MoveGenerator::generate_knight_moves(Board &board, int start_square, std::vector<Move> &moves) {
    unsigned char *chess_board = board.get_board();
    unsigned char knight = chess_board[start_square];
    bool is_white = Piece::is_piece_white(knight);

    for (int i = 0; i < 8; i++) {
        int end_square = start_square + Piece::knight_dir[i];

        if (!in_bounds(end_square) || !knight_in_bounds(start_square, end_square)) {
            continue;
        }

        unsigned char piece_on_end_square = chess_board[end_square];

        // Blocked by friendly piece
        if (Piece::is_friendly(knight, piece_on_end_square)) {
            continue;
        }

        moves.push_back({start_square, end_square, is_white});
    }
}

// TODO: Implement en passant
void MoveGenerator::generate_pawn_moves(Board &board, int start_square, std::vector<Move> &moves) {
    unsigned char *chess_board = board.get_board();
    unsigned char pawn = chess_board[start_square];
    bool is_white = Piece::is_piece_white(pawn);

    int dir = is_white ? 1 : -1;

    // Single step forward
    int one_step = start_square + dir * Piece::pawn_dir[0];
    if (in_bounds(one_step) && chess_board[one_step] == Piece::none) {
        moves.push_back({start_square, one_step, is_white});

        // Double step forward (only if the single step was possible)
        if (!Piece::has_piece_moved(pawn)) {
            int two_step = start_square + dir * Piece::pawn_dir[3];
            if (in_bounds(two_step) && chess_board[two_step] == Piece::none) {
                moves.push_back({start_square, two_step, is_white});
            }
        }
    }

    // Diagonal captures
    for (int i = 1; i <= 2; i++) {
        int capture_square = start_square + dir * Piece::pawn_dir[i];

        if (in_bounds(capture_square)) {
            unsigned char piece_on_target = chess_board[capture_square];

            if (!Piece::is_friendly(pawn, piece_on_target) && piece_on_target != Piece::none) {
                moves.push_back({start_square, capture_square, is_white});
            }
        }
    }
}

// TODO: include castling
void MoveGenerator::generate_king_moves(Board &board, int start_square, std::vector<Move> &moves) {
    unsigned char *chess_board = board.get_board();
    unsigned char king = chess_board[start_square];
    bool is_white = Piece::is_piece_white(king);

    for (int i = 0; i < 8; i++) {
        int end_square = start_square + Piece::dir[i];

        if (!in_bounds(end_square)) {
            continue;
        }

        unsigned char piece_on_end_square = chess_board[end_square];

        if (Piece::is_friendly(king, piece_on_end_square)) {
            continue;
        }

        moves.push_back({start_square, end_square, is_white});
    }
}
