#include "../include/movegen.h"

// TODO: Implement captures and en passant for pawns
// TODO: Impement castling for king

u64 MoveGenerator::rays[8][64];
u64 MoveGenerator::rank_attacks[64];
u64 MoveGenerator::file_attacks[64];
u64 MoveGenerator::diag_attacks[64];
u64 MoveGenerator::anti_diag_attacks[64];

void MoveGenerator::init() {
    init_rays();
    init_sliding_attacks();
}

void MoveGenerator::init_rays() {
    u64 north = 0x0101010101010100;
    u64 south = 0x0080808080808080;
    u64 no_ea = 0x8040201008040200;
    u64 no_we = 0x0102040810204000;
    u64 so_we = 0x0040201008040201;
    u64 so_ea = 0x0002040810204080;
    for (int i = 0; i < 64; i++, north <<= 1ULL) {
        rays[NORTH][i] = north;
        rays[EAST][i] = 2ULL * ((1ULL << (i | 7ULL)) - (1ULL << i));
    }
    for (int i = 63; i >= 0; i--, south >>= 1) {
        rays[SOUTH][i] = south;
        rays[WEST][i] = (1ULL << i) - (1ULL << (i & 56ULL));
    }
    for (int f = 0; f < 8; f++, no_ea = shift_east(no_ea)) {
        u64 ne = no_ea;
        for (int r = 0; r < 8; r++, ne <<= 8) {
            rays[NORTH_EAST][f + r * 8] = ne;
        }
    }
    for (int f = 0; f < 8; f++, no_we = shift_west(no_we)) {
        u64 nw = no_we;
        for (int r = 0; r < 8; r++, nw <<= 8) {
            rays[NORTH_WEST][f + r * 8] = nw;
        }
    }
    for (int f = 0; f < 8; f++, so_ea = shift_east(so_ea)) {
        u64 se = so_ea;
        for (int r = 0; r < 8; r++, se >>= 8) {
            rays[SOUTH_EAST][f + r * 8] = se;
        }
    }
    for (int f = 0; f < 8; f++, so_we = shift_west(so_we)) {
        u64 sw = so_we;
        for (int r = 0; r < 8; r++, sw >>= 8) {
            rays[SOUTH_WEST][f + r * 8] = sw;
        }
    }
}

void MoveGenerator::init_sliding_attacks() {
    for (int i = 0; i < 64; i++) {
        rank_attacks[i] = rays[EAST][i] | rays[WEST][i];
        file_attacks[i] = rays[NORTH][i] | rays[SOUTH][i];
        diag_attacks[i] = rays[NORTH_EAST][i] | rays[SOUTH_WEST][63 - i];
        anti_diag_attacks[i] = rays[NORTH_WEST][i] | rays[SOUTH_EAST][63 - i];
    }
}

u64 MoveGenerator::generate_moves(ChessGame &game) {}

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

static u64 generate_white_sliding_moves(ChessGame &game) {}
