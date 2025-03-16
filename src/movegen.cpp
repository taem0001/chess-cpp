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

    int from, to;
    while (pawns) {
        from = first_bit(pawns);
        int file = from % 8;
        u64 file_attacks = pushes & mask_file[file];
        while (file_attacks) {
            to = first_bit(file_attacks);
            moves.push_back(define_move(from, to, quiet_move));
            file_attacks &= file_attacks - 1;
        }
        pawns &= pawns - 1;
    }
}
