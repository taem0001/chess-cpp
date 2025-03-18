#include "../include/bitboard.h"

u64 BitBoardGenerator::rays[8][64];
u64 BitBoardGenerator::precomputed_bishop[64];
u64 BitBoardGenerator::precomputed_rook[64];

u64 BitBoardGenerator::all_moves_bitboard(ChessGame &game) {
    bool turn = game.get_turn();
    return generate_attacks_bitboard(game, turn) |
           (turn ? generate_white_castle_bitboard(game)
                 : generate_black_castle_bitboard(game)) |
           (turn ? generate_white_pawn_bitboard(game)
                 : generate_black_pawn_bitboard(game));
}

void BitBoardGenerator::init() {
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
    for (int f = 7; f >= 0; f--, no_we = shift_west(no_we)) {
        u64 nw = no_we;
        for (int r = 0; r < 8; r++, nw <<= 8) {
            rays[NORTH_WEST][(r * 8) + f] = nw;
        }
    }
    for (int f = 0; f < 8; f++, so_ea = shift_east(so_ea)) {
        u64 se = so_ea;
        for (int r = 7; r >= 0; r--, se >>= 8) {
            rays[SOUTH_EAST][(r * 8) + f] = se;
        }
    }
    for (int f = 7; f >= 0; f--, so_we = shift_west(so_we)) {
        u64 sw = so_we;
        for (int r = 7; r >= 0; r--, sw >>= 8) {
            rays[SOUTH_WEST][(r * 8) + f] = sw;
        }
    }

    for (int i = 0; i < 64; i++) {
        precomputed_bishop[i] = diag_attacks(0, (unsigned long)i) |
                                anti_diag_attacks(0, (unsigned long)i);
        precomputed_rook[i] = rank_attacks(0, (unsigned long)i) |
                              file_attacks(0, (unsigned long)i);
    }
}

u64 BitBoardGenerator::get_positive_rays(u64 occ, Direction dir,
                                         unsigned long sq) {
    u64 attacks = rays[dir][sq];
    u64 blocker = attacks & occ;
    sq = first_bit(blocker | (u64)0x8000000000000000);
    return attacks ^ rays[dir][sq];
}

u64 BitBoardGenerator::get_negative_rays(u64 occ, Direction dir,
                                         unsigned long sq) {
    u64 attacks = rays[dir][sq];
    u64 blocker = attacks & occ;
    sq = last_bit(blocker | 1ULL);
    return attacks ^ rays[dir][sq];
}

u64 BitBoardGenerator::diag_attacks(u64 occ, unsigned long sq) {
    return get_positive_rays(occ, NORTH_EAST, sq) |
           get_negative_rays(occ, SOUTH_WEST, sq);
}

u64 BitBoardGenerator::anti_diag_attacks(u64 occ, unsigned long sq) {
    return get_positive_rays(occ, NORTH_WEST, sq) |
           get_negative_rays(occ, SOUTH_EAST, sq);
}

u64 BitBoardGenerator::rank_attacks(u64 occ, unsigned long sq) {
    return get_positive_rays(occ, EAST, sq) | get_negative_rays(occ, WEST, sq);
}

u64 BitBoardGenerator::file_attacks(u64 occ, unsigned long sq) {
    return get_positive_rays(occ, NORTH, sq) |
           get_negative_rays(occ, SOUTH, sq);
}

u64 BitBoardGenerator::generate_attacks_bitboard(ChessGame &game, bool turn) {
    u64 all_attacks;

    if (turn) {
        all_attacks = generate_white_pawn_captures_bitboard(game) |
                      generate_white_bishop_bitboard(game) |
                      generate_white_king_bitboard(game) |
                      generate_white_knight_bitboard(game) |
                      generate_white_queen_bitboard(game) |
                      generate_white_rook_bitboard(game);
    } else {
        all_attacks = generate_black_pawn_captures_bitboard(game) |
                      generate_black_bishop_bitboard(game) |
                      generate_black_king_bitboard(game) |
                      generate_black_knight_bitboard(game) |
                      generate_black_queen_bitboard(game) |
                      generate_black_rook_bitboard(game);
    }
    return all_attacks;
}

u64 BitBoardGenerator::generate_white_pawn_bitboard(ChessGame &game) {
    u64 *bitboards = game.get_board().get_bitboards();
    u64 white_pawns = bitboards[WHITE_PAWN];
    u64 empty = ~bitboards[ALL];

    u64 single_push = shift_north(white_pawns) & empty;
    u64 double_push = shift_north(single_push) & empty & mask_rank[3];
    u64 all_pushes = single_push | double_push;

    return all_pushes;
}

u64 BitBoardGenerator::generate_black_pawn_bitboard(ChessGame &game) {
    u64 *bitboards = game.get_board().get_bitboards();
    u64 black_pawns = bitboards[BLACK_PAWN];
    u64 empty = ~bitboards[ALL];

    u64 single_push = shift_south(black_pawns) & empty;
    u64 double_push = shift_south(single_push) & empty & mask_rank[4];
    u64 all_pushes = single_push | double_push;

    return all_pushes;
}

u64 BitBoardGenerator::generate_white_pawn_captures_bitboard(ChessGame &game) {
    u64 *bitboards = game.get_board().get_bitboards();
    u64 white_pawns = bitboards[WHITE_PAWN];
    int en_passant_sq = game.get_en_passant_sq();
    u64 en_passant_pos = en_passant_sq != -1 ? mask_piece[en_passant_sq] : 0;

    u64 nw_pawns = shift_north_west(white_pawns);
    u64 ne_pawns = shift_north_east(white_pawns);
    u64 nw_enp = nw_pawns & en_passant_pos;
    u64 ne_enp = ne_pawns & en_passant_pos;
    nw_pawns &= bitboards[BLACK];
    ne_pawns &= bitboards[BLACK];

    return nw_pawns | ne_pawns | nw_enp | ne_enp;
}

u64 BitBoardGenerator::generate_black_pawn_captures_bitboard(ChessGame &game) {
    u64 *bitboards = game.get_board().get_bitboards();
    u64 black_pawns = bitboards[BLACK_PAWN];
    int en_passant_sq = game.get_en_passant_sq();
    u64 en_passant_pos = en_passant_sq != -1 ? mask_piece[en_passant_sq] : 0;

    u64 sw_pawns = shift_south_west(black_pawns);
    u64 se_pawns = shift_south_east(black_pawns);
    u64 sw_enp = sw_pawns & en_passant_pos;
    u64 se_enp = se_pawns & en_passant_pos;
    sw_pawns &= bitboards[WHITE];
    se_pawns &= bitboards[WHITE];

    return sw_pawns | se_pawns | sw_enp | se_enp;
}

u64 BitBoardGenerator::generate_white_king_bitboard(ChessGame &game) {
    u64 *bitboards = game.get_board().get_bitboards();
    u64 white_king = bitboards[WHITE_KING];
    u64 moves = shift_east(white_king) | shift_west(white_king);
    white_king |= moves;
    moves |= shift_north(white_king) | shift_south(white_king);
    moves &= ~bitboards[ALL] | bitboards[BLACK];
    return moves;
}

u64 BitBoardGenerator::generate_black_king_bitboard(ChessGame &game) {
    u64 *bitboards = game.get_board().get_bitboards();
    u64 black_king = bitboards[BLACK_KING];
    u64 moves = shift_east(black_king) | shift_west(black_king);
    black_king |= moves;
    moves |= shift_north(black_king) | shift_south(black_king);
    moves &= ~bitboards[ALL] | bitboards[WHITE];
    return moves;
}

u64 BitBoardGenerator::generate_white_knight_bitboard(ChessGame &game) {
    u64 *bitboards = game.get_board().get_bitboards();
    u64 white_knight = bitboards[WHITE_KNIGHT];
    u64 no_no_ea = (white_knight << 17) & clear_file[0];
    u64 no_ea_ea = (white_knight << 10) & not_ab_file;
    u64 so_ea_ea = (white_knight >> 6) & not_ab_file;
    u64 so_so_ea = (white_knight >> 15) & clear_file[0];
    u64 no_no_we = (white_knight << 15) & clear_file[7];
    u64 no_we_we = (white_knight << 6) & not_gh_file;
    u64 so_we_we = (white_knight >> 10) & not_gh_file;
    u64 so_so_we = (white_knight >> 17) & clear_file[7];

    u64 res = no_no_ea | no_ea_ea | so_ea_ea | so_so_ea | no_no_we | no_we_we |
              so_we_we | so_so_we;
    res &= ~bitboards[ALL] | bitboards[BLACK];
    return res;
}

u64 BitBoardGenerator::generate_black_knight_bitboard(ChessGame &game) {
    u64 *bitboards = game.get_board().get_bitboards();
    u64 black_knight = bitboards[BLACK_KNIGHT];
    u64 no_no_ea = (black_knight << 17) & clear_file[0];
    u64 no_ea_ea = (black_knight << 10) & not_ab_file;
    u64 so_ea_ea = (black_knight >> 6) & not_ab_file;
    u64 so_so_ea = (black_knight >> 15) & clear_file[0];
    u64 no_no_we = (black_knight << 15) & clear_file[7];
    u64 no_we_we = (black_knight << 6) & not_gh_file;
    u64 so_we_we = (black_knight >> 10) & not_gh_file;
    u64 so_so_we = (black_knight >> 17) & clear_file[7];

    u64 res = no_no_ea | no_ea_ea | so_ea_ea | so_so_ea | no_no_we | no_we_we |
              so_we_we | so_so_we;
    res &= ~bitboards[ALL] | bitboards[WHITE];
    return res;
}

u64 BitBoardGenerator::generate_white_bishop_bitboard(ChessGame &game) {
    u64 *bitboards = game.get_board().get_bitboards();
    u64 bishops = bitboards[WHITE_BISHOP];
    u64 res = 0;
    while (bishops) {
        int pos = first_bit(bishops);
        res |= diag_attacks(bitboards[ALL], (unsigned long)pos) |
               anti_diag_attacks(bitboards[ALL], (unsigned long)pos);
        bishops &= (bishops - 1);
    }
    res &= ~bitboards[ALL] | bitboards[BLACK];
    return res;
}

u64 BitBoardGenerator::generate_black_bishop_bitboard(ChessGame &game) {
    u64 *bitboards = game.get_board().get_bitboards();
    u64 bishops = bitboards[BLACK_BISHOP];
    u64 res = 0;
    while (bishops) {
        int pos = first_bit(bishops);
        res |= diag_attacks(bitboards[ALL], (unsigned long)pos) |
               anti_diag_attacks(bitboards[ALL], (unsigned long)pos);
        bishops &= (bishops - 1);
    }
    res &= ~bitboards[ALL] | bitboards[WHITE];
    return res;
}

u64 BitBoardGenerator::generate_white_rook_bitboard(ChessGame &game) {
    u64 *bitboards = game.get_board().get_bitboards();
    u64 rooks = bitboards[WHITE_ROOK];
    u64 res = 0;
    while (rooks) {
        int pos = first_bit(rooks);
        res |= rank_attacks(bitboards[ALL], (unsigned long)pos) |
               file_attacks(bitboards[ALL], (unsigned long)pos);
        rooks &= (rooks - 1);
    }
    res &= ~bitboards[ALL] | bitboards[BLACK];
    return res;
}

u64 BitBoardGenerator::generate_black_rook_bitboard(ChessGame &game) {
    u64 *bitboards = game.get_board().get_bitboards();
    u64 rooks = bitboards[BLACK_ROOK];
    u64 res = 0;
    while (rooks) {
        int pos = first_bit(rooks);
        res |= rank_attacks(bitboards[ALL], (unsigned long)pos) |
               file_attacks(bitboards[ALL], (unsigned long)pos);
        rooks &= (rooks - 1);
    }
    res &= ~bitboards[ALL] | bitboards[WHITE];
    return res;
}

u64 BitBoardGenerator::generate_white_queen_bitboard(ChessGame &game) {
    u64 *bitboards = game.get_board().get_bitboards();
    u64 queen = bitboards[WHITE_QUEEN];
    u64 res = 0;
    if (queen) {
        int pos = first_bit(queen);
        res = diag_attacks(bitboards[ALL], (unsigned long)pos) |
              anti_diag_attacks(bitboards[ALL], (unsigned long)pos) |
              rank_attacks(bitboards[ALL], (unsigned long)pos) |
              file_attacks(bitboards[ALL], (unsigned long)pos);
    }
    res &= ~bitboards[ALL] | bitboards[BLACK];
    return res;
}

u64 BitBoardGenerator::generate_black_queen_bitboard(ChessGame &game) {
    u64 *bitboards = game.get_board().get_bitboards();
    u64 queen = bitboards[BLACK_QUEEN];
    u64 res = 0;
    if (queen) {
        int pos = first_bit(queen);
        res = diag_attacks(bitboards[ALL], (unsigned long)pos) |
              anti_diag_attacks(bitboards[ALL], (unsigned long)pos) |
              rank_attacks(bitboards[ALL], (unsigned long)pos) |
              file_attacks(bitboards[ALL], (unsigned long)pos);
    }
    res &= ~bitboards[ALL] | bitboards[WHITE];
    return res;
}

u64 BitBoardGenerator::generate_white_castle_bitboard(ChessGame &game) {
    u64 attacks = generate_attacks_bitboard(game, false);
    u64 occ = game.get_board().get_bitboards()[ALL];
    u64 res = 0;
    if (game.get_wk_castle() && (attacks & wk_castle_mask) == 0 &&
        (occ & wk_castle_mask) == 0) {
        res |= 0x40ULL;
    }
    if (game.get_wq_castle() && (attacks & wq_castle_mask) == 0 &&
        (occ & wq_castle_mask) == 0) {
        res |= 0x4ULL;
    }
    return res;
}

u64 BitBoardGenerator::generate_black_castle_bitboard(ChessGame &game) {
    u64 attacks = generate_attacks_bitboard(game, true);
    u64 occ = game.get_board().get_bitboards()[ALL];
    u64 res = 0;
    if (game.get_bk_castle() && (attacks & bk_castle_mask) == 0 &&
        (occ & bk_castle_mask) == 0) {
        res |= 0x40ULL << 56;
    }
    if (game.get_bq_castle() && (attacks & bq_castle_mask) == 0 &&
        (occ & bq_castle_mask) == 0) {
        res |= 0x4ULL << 56;
    }
    return res;
}
