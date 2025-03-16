#include "../include/movegen.h"

void MoveGenerator::init() { BitBoardGenerator::init(); }

std::vector<u16> MoveGenerator::generate_pseudolegal_moves(ChessGame &game) {
    std::vector<u16> moves;
    generate_pawn_moves(moves, game);
    return moves;
}

void MoveGenerator::generate_pawn_moves(std::vector<u16> &moves,
                                        ChessGame &game) {
    bool turn = game.get_turn();
    u64 *bitboards = game.get_board().get_bitboards();
    u64 pawns = turn ? bitboards[WHITE_PAWN] : bitboards[BLACK_PAWN];
    u64 pushes = turn ? BitBoardGenerator::generate_white_pawn_bitboard(game)
                      : BitBoardGenerator::generate_black_pawn_bitboard(game);
    u64 captures =
        turn ? BitBoardGenerator::generate_white_pawn_captures_bitboard(game)
             : BitBoardGenerator::generate_black_pawn_captures_bitboard(game);

    int from, to;
    u16 flag;
    while (pawns) {
        from = first_bit(pawns);
        int file = from % 8;
        u64 file_pushes = pushes & mask_file[file];
        while (file_pushes) {
            to = first_bit(file_pushes);
            flag = abs(to - from) == 16 ? double_pawn_push : quiet_move;
            moves.push_back(define_move(from, to, flag));
            file_pushes &= file_pushes - 1;
        }

        u64 left_captures = file == 0 ? 0 : captures & mask_file[file - 1];
        while (left_captures) {
            to = first_bit(left_captures);
            flag = to == game.get_en_passant_sq() ? ep_capture : capture;
            moves.push_back(define_move(from, to, flag));
            left_captures &= left_captures - 1;
        }

        u64 right_captures = file == 7 ? 0 : captures & mask_file[file + 1];
        while (right_captures) {
            to = first_bit(right_captures);
            flag = to == game.get_en_passant_sq() ? ep_capture : capture;
            moves.push_back(define_move(from, to, flag));
            right_captures &= right_captures - 1;
        }
        pawns &= pawns - 1;
    }
}
