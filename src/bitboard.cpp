#include "../include/bitboard.h"
#include <vector>

u64 BitBoardGenerator::rays[8][64];
u64 BitBoardGenerator::precomputed_bishop[64];
u64 BitBoardGenerator::precomputed_rook[64];

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

u64 BitBoardGenerator::pieces_attacking_square(ChessGame &game, int sq,
                                               bool turn) {
    u64 *bitboards = game.get_board().get_bitboards();
    u64 res = 0;
    u64 square_pos = mask_piece[sq];

    // Check for pawns attacking the square
    if (turn) {
        res |= shift_south_west(square_pos) & bitboards[WHITE_PAWN];
        res |= shift_south_east(square_pos) & bitboards[WHITE_PAWN];
    } else {
        res |= shift_north_west(square_pos) & bitboards[BLACK_PAWN];
        res |= shift_north_east(square_pos) & bitboards[BLACK_PAWN];
    }

    // Check for knights attacking the square
    PieceType knight_type = turn ? WHITE_KNIGHT : BLACK_KNIGHT;
    res |= knight_attack_squares[sq] & bitboards[knight_type];

    // Check for kings attacking the square
    PieceType king_type = turn ? WHITE_KING : BLACK_KING;
    u64 king_moves = shift_west(square_pos) | shift_east(square_pos);
    u64 temp = square_pos;
    temp |= king_moves;
    king_moves |= shift_north(temp) | shift_south(temp);
    king_moves &= ~square_pos;
    res |= bitboards[king_type] & king_moves;

    // Check for sliding pieces attacking the square
    // Get the rays from the attacked square
    u64 rays[8] = {
        get_positive_rays(bitboards[ALL], NORTH, (unsigned long)sq),
        get_negative_rays(bitboards[ALL], SOUTH, (unsigned long)sq),
        get_positive_rays(bitboards[ALL], EAST, (unsigned long)sq),
        get_negative_rays(bitboards[ALL], WEST, (unsigned long)sq),
        get_positive_rays(bitboards[ALL], NORTH_EAST, (unsigned long)sq),
        get_positive_rays(bitboards[ALL], NORTH_WEST, (unsigned long)sq),
        get_negative_rays(bitboards[ALL], SOUTH_EAST, (unsigned long)sq),
        get_negative_rays(bitboards[ALL], SOUTH_WEST, (unsigned long)sq)};

    // Get the first or last piece in each direction
    int blockers[8] = {
        last_bit(rays[0]),  first_bit(rays[1]), // North / South
        last_bit(rays[2]),  first_bit(rays[3]), // East / West
        last_bit(rays[4]),  last_bit(rays[5]),  // NE / NW
        first_bit(rays[6]), first_bit(rays[7])  // SE / SW
    };

    // Determine attacking pieces
    PieceType rook = turn ? WHITE_ROOK : BLACK_ROOK;
    PieceType bishop = turn ? WHITE_BISHOP : BLACK_BISHOP;
    PieceType queen = turn ? WHITE_QUEEN : BLACK_QUEEN;

    for (int i = 0; i < 4; i++) {
        res |= (mask_piece[blockers[i]] & bitboards[rook]) |
               (mask_piece[blockers[i]] & bitboards[queen]);
    }
    for (int i = 4; i < 8; i++) {
        res |= (mask_piece[blockers[i]] & bitboards[bishop]) |
               (mask_piece[blockers[i]] & bitboards[queen]);
    }

    return res;
}

u64 BitBoardGenerator::pieces_attacking_king(ChessGame &game, bool turn) {
    u64 *bitboards = game.get_board().get_bitboards();
    int king_pos = turn ? first_bit(bitboards[WHITE_KING])
                        : first_bit(bitboards[BLACK_KING]);
    u64 res = pieces_attacking_square(game, king_pos, !turn);
    res &= turn ? ~bitboards[BLACK_KING] : ~bitboards[WHITE_KING];
    return res;
}

u64 BitBoardGenerator::generate_attacks_bitboard(ChessGame &game, bool turn) {
    u64 *bitboards = game.get_board().get_bitboards();
    u64 all_attacks = generate_pawn_captures_bitboard(game, turn) |
                      generate_knight_bitboard(bitboards, turn) |
                      generate_bishop_bitboard(bitboards, turn) |
                      generate_rook_bitboard(bitboards, turn) |
                      generate_queen_bitboard(bitboards, turn) |
                      generate_king_bitboard(game, turn);
    return all_attacks;
}

u64 BitBoardGenerator::generate_pawn_bitboard(u64 *bitboards, bool turn) {
    u64 pawns = turn ? bitboards[WHITE_PAWN] : bitboards[BLACK_PAWN];
    u64 empty = ~bitboards[ALL];

    u64 single_push, double_push;
    if (turn) {
        single_push = shift_north(pawns) & empty;
        double_push = shift_north(single_push) & empty & mask_rank[3];
    } else {
        single_push = shift_south(pawns) & empty;
        double_push = shift_south(single_push) & empty & mask_rank[4];
    }
    u64 all_pushes = single_push | double_push;

    return all_pushes;
}

u64 BitBoardGenerator::generate_pawn_captures_bitboard(ChessGame &game,
                                                       bool turn) {
    u64 *bitboards = game.get_board().get_bitboards();
    u64 pawns = turn ? bitboards[WHITE_PAWN] : bitboards[BLACK_PAWN];
    int en_passant_sq = game.get_en_passant_sq();
    u64 en_passant_pos = en_passant_sq != -1 ? mask_piece[en_passant_sq] : 0;
    u64 res = 0;

    u64 nw_pawns, ne_pawns, nw_enp, ne_enp;
    u64 sw_pawns, se_pawns, sw_enp, se_enp;
    if (turn) {
        nw_pawns = shift_north_west(pawns);
        ne_pawns = shift_north_east(pawns);
        nw_enp = nw_pawns & en_passant_pos;
        ne_enp = ne_pawns & en_passant_pos;
        nw_pawns &= bitboards[BLACK];
        ne_pawns &= bitboards[BLACK];
        res = nw_pawns | ne_pawns | nw_enp | ne_enp;
    } else {
        sw_pawns = shift_south_west(pawns);
        se_pawns = shift_south_east(pawns);
        sw_enp = sw_pawns & en_passant_pos;
        se_enp = se_pawns & en_passant_pos;
        sw_pawns &= bitboards[WHITE];
        se_pawns &= bitboards[WHITE];
        res = sw_pawns | se_pawns | sw_enp | se_enp;
    }

    return res;
}

u64 BitBoardGenerator::generate_king_bitboard(ChessGame &game, bool turn) {
    u64 *bitboards = game.get_board().get_bitboards();
    u64 king = turn ? bitboards[WHITE_KING] : bitboards[BLACK_KING];
    u64 moves = shift_east(king) | shift_west(king);
    king |= moves;
    moves |= shift_north(king) | shift_south(king);
    if (turn) {
        moves &= ~bitboards[WHITE];
    } else {
        moves &= ~bitboards[BLACK];
    }
    return moves;
}

u64 BitBoardGenerator::generate_knight_bitboard(u64 *bitboards, bool turn) {
    u64 knights = turn ? bitboards[WHITE_KNIGHT] : bitboards[BLACK_KNIGHT];
    u64 no_no_ea = (knights << 17) & clear_file[0];
    u64 no_ea_ea = (knights << 10) & not_ab_file;
    u64 so_ea_ea = (knights >> 6) & not_ab_file;
    u64 so_so_ea = (knights >> 15) & clear_file[0];
    u64 no_no_we = (knights << 15) & clear_file[7];
    u64 no_we_we = (knights << 6) & not_gh_file;
    u64 so_we_we = (knights >> 10) & not_gh_file;
    u64 so_so_we = (knights >> 17) & clear_file[7];

    u64 res = no_no_ea | no_ea_ea | so_ea_ea | so_so_ea | no_no_we | no_we_we |
              so_we_we | so_so_we;
    if (turn) {
        res &= ~bitboards[WHITE];
    } else {
        res &= ~bitboards[BLACK];
    }
    return res;
}

u64 BitBoardGenerator::generate_bishop_bitboard(u64 *bitboards, bool turn) {
    u64 bishops = turn ? bitboards[WHITE_BISHOP] : bitboards[BLACK_BISHOP];
    u64 res = 0;
    while (bishops) {
        int pos = first_bit(bishops);
        res |= diag_attacks(bitboards[ALL], (unsigned long)pos) |
               anti_diag_attacks(bitboards[ALL], (unsigned long)pos);
        bishops &= (bishops - 1);
    }
    if (turn) {
        res &= ~bitboards[WHITE];
    } else {
        res &= ~bitboards[BLACK];
    }
    return res;
}

u64 BitBoardGenerator::generate_rook_bitboard(u64 *bitboards, bool turn) {
    u64 rooks = turn ? bitboards[WHITE_ROOK] : bitboards[BLACK_ROOK];
    u64 res = 0;
    while (rooks) {
        int pos = first_bit(rooks);
        res |= rank_attacks(bitboards[ALL], (unsigned long)pos) |
               file_attacks(bitboards[ALL], (unsigned long)pos);
        rooks &= (rooks - 1);
    }
    if (turn) {
        res &= ~bitboards[WHITE];
    } else {
        res &= ~bitboards[BLACK];
    }
    return res;
}

u64 BitBoardGenerator::generate_queen_bitboard(u64 *bitboards, bool turn) {
    u64 queen = turn ? bitboards[WHITE_QUEEN] : bitboards[BLACK_QUEEN];
    u64 res = 0;
    if (queen) {
        int pos = first_bit(queen);
        res = diag_attacks(bitboards[ALL], (unsigned long)pos) |
              anti_diag_attacks(bitboards[ALL], (unsigned long)pos) |
              rank_attacks(bitboards[ALL], (unsigned long)pos) |
              file_attacks(bitboards[ALL], (unsigned long)pos);
    }
    if (turn) {
        res &= ~bitboards[WHITE];
    } else {
        res &= ~bitboards[BLACK];
    }
    return res;
}

u64 BitBoardGenerator::generate_castle_bitboard(ChessGame &game, bool turn) {
    u64 attacks = generate_attacks_bitboard(game, !turn);
    u64 occ = game.get_board().get_bitboards()[ALL];
    u64 res = 0;
    if (turn) {
        if (game.get_wk_castle() && (attacks & wk_castle_mask) == 0 &&
            (occ & wk_castle_mask) == 0) {
            res |= 0x40ULL;
        }
        if (game.get_wq_castle() && (attacks & wq_castle_mask) == 0 &&
            (occ & wq_castle_mask) == 0) {
            res |= 0x4ULL;
        }
    } else {
        if (game.get_bk_castle() && (attacks & bk_castle_mask) == 0 &&
            (occ & bk_castle_mask) == 0) {
            res |= 0x40ULL << 56;
        }
        if (game.get_bq_castle() && (attacks & bq_castle_mask) == 0 &&
            (occ & bq_castle_mask) == 0) {
            res |= 0x4ULL << 56;
        }
    }
    return res;
}

u64 BitBoardGenerator::generate_pinned_pieces_bitboard(u64 *bitboards,
                                                       bool turn) {
    u64 pinned = 0;
    u64 own_pieces = turn ? bitboards[WHITE] : bitboards[BLACK];
    u64 op_rq = turn ? bitboards[BLACK_ROOK] | bitboards[BLACK_QUEEN]
                     : bitboards[WHITE_ROOK] | bitboards[WHITE_QUEEN];
    u64 op_bq = turn ? bitboards[BLACK_BISHOP] | bitboards[BLACK_QUEEN]
                     : bitboards[WHITE_BISHOP] | bitboards[WHITE_QUEEN];
    int king_sq = turn ? first_bit(bitboards[WHITE_KING])
                       : first_bit(bitboards[BLACK_KING]);

    u64 pinner = xray_rook_attacks(bitboards[ALL], own_pieces, king_sq) & op_rq;
    while (pinner) {
        int sq = first_bit(pinner);
        pinned |= in_between(sq, king_sq) & own_pieces;
        pinner &= pinner - 1;
    }
    pinner = xray_bishop_attacks(bitboards[ALL], own_pieces, king_sq) & op_bq;
    while (pinner) {
        int sq = first_bit(pinner);
        pinned |= in_between(sq, king_sq) & own_pieces;
        pinner &= pinner - 1;
    }
    return pinned;
}

u64 BitBoardGenerator::xray_rook_attacks(u64 occ, u64 blockers, int sq) {
    u64 attacks = rank_attacks(occ, (unsigned long)sq) |
                  file_attacks(occ, (unsigned long)sq);
    blockers &= attacks;
    u64 var = rank_attacks(occ ^ blockers, (unsigned long)sq) |
              file_attacks(occ ^ blockers, (unsigned long)sq);
    return attacks ^ var;
}

u64 BitBoardGenerator::xray_bishop_attacks(u64 occ, u64 blockers, int sq) {
    u64 attacks = diag_attacks(occ, (unsigned long)sq) |
                  anti_diag_attacks(occ, (unsigned long)sq);
    blockers &= attacks;
    u64 var = diag_attacks(occ ^ blockers, (unsigned long)sq) |
              anti_diag_attacks(occ ^ blockers, (unsigned long)sq);
    return attacks ^ var;
}
