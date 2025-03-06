#include "../include/movegen.h"

// TODO: Implement captures and en passant for pawns
// TODO: Impement castling for king

u64 MoveGenerator::generate_moves(ChessGame &game) {
}

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

u64 MoveGenerator::generate_white_king_moves(ChessGame &game) {
    u64 *bitboards = game.get_board().get_bitboards();
    u64 white_king = bitboards[WHITE_KING];
    u64 moves = shift_east(white_king) | shift_west(white_king);
    white_king |= moves;
    moves |= shift_north(white_king) | shift_south(white_king);
    return moves;
}

u64 MoveGenerator::generate_black_king_moves(ChessGame &game) {
    u64 *bitboards = game.get_board().get_bitboards();
    u64 black_king = bitboards[BLACK_KING];
    u64 moves = shift_east(black_king) | shift_west(black_king);
    black_king |= moves;
    moves |= shift_north(black_king) | shift_south(black_king);
    return moves;
}

u64 MoveGenerator::generate_white_knight_moves(ChessGame &game) {
    u64 *bitboards = game.get_board().get_bitboards();
    u64 white_knight = bitboards[WHITE_KNIGHT];
    u64 no_no_ea = (white_knight << 17) & not_a_file;
    u64 no_ea_ea = (white_knight << 10) & not_ab_file;
    u64 so_ea_ea = (white_knight >> 6) & not_ab_file;
    u64 so_so_ea = (white_knight >> 15) & not_a_file;
    u64 no_no_we = (white_knight << 15) & not_h_file;
    u64 no_we_we = (white_knight << 6) & not_gh_file;
    u64 so_we_we = (white_knight >> 10) & not_gh_file;
    u64 so_so_we = (white_knight >> 17) & not_h_file;

    return no_no_ea | no_ea_ea | so_ea_ea | so_so_ea | no_no_we | no_we_we |
           so_we_we | so_so_we;
}

u64 MoveGenerator::generate_black_knight_moves(ChessGame &game) {
    u64 *bitboards = game.get_board().get_bitboards();
    u64 black_knight = bitboards[BLACK_KNIGHT];
    u64 no_no_ea = (black_knight << 17) & not_a_file;
    u64 no_ea_ea = (black_knight << 10) & not_ab_file;
    u64 so_ea_ea = (black_knight >> 6) & not_ab_file;
    u64 so_so_ea = (black_knight >> 15) & not_a_file;
    u64 no_no_we = (black_knight << 15) & not_h_file;
    u64 no_we_we = (black_knight << 6) & not_gh_file;
    u64 so_we_we = (black_knight >> 10) & not_gh_file;
    u64 so_so_we = (black_knight >> 17) & not_h_file;

    return no_no_ea | no_ea_ea | so_ea_ea | so_so_ea | no_no_we | no_we_we |
           so_we_we | so_so_we;
}
