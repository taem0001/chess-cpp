#include "../include/movegen.h"

u64 MoveGenerator::generate_white_pawn_pushes(ChessGame &game) {
    u64 *bitboards = game.get_board().get_bitboards();
    u64 white_pawns = bitboards[WHITE_PAWN];
    u64 empty = bitboards[EMPTY];

    u64 single_push = shift_north(white_pawns) & empty;
    u64 double_push = shift_north(single_push) & empty & mask_rank[3];
    u64 all_pushes = single_push | double_push;

    return all_pushes;
}

u64 MoveGenerator::generate_black_pawn_pushes(ChessGame &game) {
    u64 *bitboards = game.get_board().get_bitboards();
    u64 black_pawns = bitboards[BLACK_PAWN];
    u64 empty = bitboards[EMPTY];

    u64 single_push = shift_south(black_pawns) & empty;
    u64 double_push = shift_south(single_push) & empty & mask_rank[4];
    u64 all_pushes = single_push | double_push;

    return all_pushes;
}
