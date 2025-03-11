#include "../include/movegen.h"

// TODO: Implement en passant for pawns
// TODO: Impement castling for king
// TODO: Implement blocking for sliding pieces

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
            rays[NORTH_EAST][(r * 8) + f] = ne;
        }
    }
    for (int f = 0; f < 8; f++, no_we = shift_west(no_we)) {
        u64 nw = no_we;
        for (int r = 0; r < 8; r++, nw <<= 8) {
            rays[NORTH_WEST][(r * 8) + f] = nw;
        }
    }
    for (int f = 0; f < 8; f++, so_ea = shift_east(so_ea)) {
        u64 se = so_ea;
        for (int r = 0; r < 8; r++, se >>= 8) {
            rays[SOUTH_EAST][(r * 8) + f] = se;
        }
    }
    for (int f = 0; f < 8; f++, so_we = shift_west(so_we)) {
        u64 sw = so_we;
        for (int r = 0; r < 8; r++, sw >>= 8) {
            rays[SOUTH_WEST][(r * 8) + f] = sw;
        }
    }
}

void MoveGenerator::init_sliding_attacks() {
    for (int i = 0; i < 64; i++) {
        int new_r = 7 - (i / 8);
        int new_c = i % 8;
        rank_attacks[i] = rays[EAST][i] | rays[WEST][i];
        file_attacks[i] = rays[NORTH][i] | rays[SOUTH][i];
        diag_attacks[i] = rays[NORTH_EAST][i] | rays[SOUTH_WEST][63 - i];
        anti_diag_attacks[new_r * 8 + new_c] =
            rays[NORTH_WEST][63 - i] | rays[SOUTH_EAST][i];
    }
}

u64 MoveGenerator::generate_moves(ChessGame &game) {
    return generate_white_bishop_attacks(game);
}

u64 MoveGenerator::generate_white_pawn_pushes(ChessGame &game) {
    u64 *bitboards = game.get_board().get_bitboards();
    u64 white_pawns = bitboards[WHITE_PAWN];
    u64 empty = ~bitboards[ALL];

    u64 single_push = shift_north(white_pawns) & empty;
    u64 double_push = shift_north(single_push) & empty & mask_rank[3];
    u64 all_pushes = single_push | double_push;

    // Captures
    u64 nw_pawns = shift_north_west(white_pawns);
    u64 ne_pawns = shift_north_east(white_pawns);
    nw_pawns &= bitboards[BLACK];
    ne_pawns &= bitboards[BLACK];

    return all_pushes | nw_pawns | ne_pawns;
}

u64 MoveGenerator::generate_black_pawn_pushes(ChessGame &game) {
    u64 *bitboards = game.get_board().get_bitboards();
    u64 black_pawns = bitboards[BLACK_PAWN];
    u64 empty = ~bitboards[ALL];

    u64 single_push = shift_south(black_pawns) & empty;
    u64 double_push = shift_south(single_push) & empty & mask_rank[4];
    u64 all_pushes = single_push | double_push;

    // Captures
    u64 sw_pawns = shift_south_west(black_pawns);
    u64 se_pawns = shift_south_east(black_pawns);
    sw_pawns &= bitboards[WHITE];
    se_pawns &= bitboards[WHITE];

    return all_pushes | sw_pawns | se_pawns;
}

u64 MoveGenerator::generate_white_king_moves(ChessGame &game) {
    u64 *bitboards = game.get_board().get_bitboards();
    u64 white_king = bitboards[WHITE_KING];
    u64 moves = shift_east(white_king) | shift_west(white_king);
    white_king |= moves;
    moves |= shift_north(white_king) | shift_south(white_king);
    moves &= ~bitboards[ALL] | bitboards[BLACK];
    return moves;
}

u64 MoveGenerator::generate_black_king_moves(ChessGame &game) {
    u64 *bitboards = game.get_board().get_bitboards();
    u64 black_king = bitboards[BLACK_KING];
    u64 moves = shift_east(black_king) | shift_west(black_king);
    black_king |= moves;
    moves |= shift_north(black_king) | shift_south(black_king);
    moves &= ~bitboards[ALL] | bitboards[WHITE];
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

    u64 res = no_no_ea | no_ea_ea | so_ea_ea | so_so_ea | no_no_we | no_we_we |
              so_we_we | so_so_we;
    res &= ~bitboards[ALL] | bitboards[BLACK];
    return res;
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

    u64 res = no_no_ea | no_ea_ea | so_ea_ea | so_so_ea | no_no_we | no_we_we |
              so_we_we | so_so_we;
    res &= ~bitboards[ALL] | bitboards[WHITE];
    return res;
}

u64 MoveGenerator::generate_white_bishop_attacks(ChessGame &game) {
    u64 *bitboards = game.get_board().get_bitboards();
    u64 bishops = bitboards[WHITE_BISHOP];
    u64 res = 0;
    while (bishops) {
        int pos = first_bit(bishops);
        res |= diag_attacks[pos] | anti_diag_attacks[pos];
        bishops &= (bishops - 1);
    }
    return res;
}

u64 MoveGenerator::generate_black_bishop_attacks(ChessGame &game) {
    u64 *bitboards = game.get_board().get_bitboards();
    u64 bishops = bitboards[BLACK_BISHOP];
    u64 res = 0;
    while (bishops) {
        int pos = first_bit(bishops);
        res |= diag_attacks[pos] | anti_diag_attacks[pos];
        bishops &= (bishops - 1);
    }
    return res;
}

u64 MoveGenerator::generate_white_rook_attacks(ChessGame &game) {
    u64 *bitboards = game.get_board().get_bitboards();
    u64 rooks = bitboards[WHITE_ROOK];
    u64 res = 0;
    while (rooks) {
        int pos = first_bit(rooks);
        res |= file_attacks[pos] | rank_attacks[pos];
        rooks &= (rooks - 1);
    }

    return res;
}

u64 MoveGenerator::generate_black_rook_attacks(ChessGame &game) {
    u64 *bitboards = game.get_board().get_bitboards();
    u64 rooks = bitboards[BLACK_ROOK];
    u64 res = 0;
    while (rooks) {
        int pos = first_bit(rooks);
        res |= file_attacks[pos] | rank_attacks[pos];
        rooks &= (rooks - 1);
    }

    return res;
}

u64 MoveGenerator::generate_white_queen_attacks(ChessGame &game) {
    u64 *bitboards = game.get_board().get_bitboards();
    u64 queen = bitboards[WHITE_QUEEN];
    u64 res = 0;
    while (queen) {
        int pos = first_bit(queen);
        res |= rank_attacks[pos] | file_attacks[pos] | diag_attacks[pos] |
               anti_diag_attacks[pos];
        queen &= (queen - 1);
    }

    return res;
}

u64 MoveGenerator::generate_black_queen_attacks(ChessGame &game) {
    u64 *bitboards = game.get_board().get_bitboards();
    u64 queen = bitboards[BLACK_QUEEN];
    u64 res = 0;
    while (queen) {
        int pos = first_bit(queen);
        res |= rank_attacks[pos] | file_attacks[pos] | diag_attacks[pos] |
               anti_diag_attacks[pos];
        queen &= (queen - 1);
    }

    return res;
}
