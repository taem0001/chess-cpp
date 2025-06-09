#include "../include/bitboard.h"

u64 BitBoardGenerator::bishop_masks[64];
u64 BitBoardGenerator::rook_masks[64];
u64 BitBoardGenerator::bishop_magics[64];
u64 BitBoardGenerator::rook_magics[64];
u64 BitBoardGenerator::bishop_attacks[64][512];
u64 BitBoardGenerator::rook_attacks[64][4096];
u64 BitBoardGenerator::precomputed_in_between[64][64];
u64 BitBoardGenerator::passed_pawn_mask[64];

void BitBoardGenerator::init() {
    // Initialize bishop and rook masks plus in between masks
    for (int i = 0; i < 64; i++) {
        bishop_masks[i] = generate_bishop_mask(i);
        rook_masks[i] = generate_rook_mask(i);
        for (int j = 0; j < 64; j++) {
            precomputed_in_between[i][j] = in_between(i, j);
        }
    }

    // Initalize magic numbers
    for (int i = 0; i < 64; i++) {
        bishop_magics[i] = find_magic(i, BBits[i], true);
        rook_magics[i] = find_magic(i, RBits[i], false);
    }

    // Generate attack tables for bishops and rooks with all blocker combinations
    for (int sq = 0; sq < 64; sq++) {
        u64 b_mask = bishop_masks[sq];
        u64 r_mask = rook_masks[sq];
        int b_bits = count_1s(b_mask);
        int r_bits = count_1s(r_mask);

        for (int i = 0; i < mask_piece[b_bits]; i++) {
            u64 blockers = index_to_u64(i, b_bits, b_mask);
            u64 attacks = generate_bishop_attacks(sq, blockers);
            int index = (blockers * bishop_magics[sq]) >> (64 - b_bits);
            bishop_attacks[sq][index] = attacks;
        }

        for (int i = 0; i < mask_piece[r_bits]; i++) {
            u64 blockers = index_to_u64(i, r_bits, r_mask);
            u64 attacks = generate_rook_attacks(sq, blockers);
            int index = (blockers * rook_magics[sq]) >> (64 - r_bits);
            rook_attacks[sq][index] = attacks;
        }
    }

    // Initialize passed pawn masks
    for (int sq = 0; sq < 64; sq++) {
        int rank = sq / 8;
        int file = sq % 8;
        u64 mask = 0;

        for (int r = rank + 1; r < 8; r++) {
            for (int f = max(file - 1, 0); f <= min(file + 1, 7); f++) {
                mask |= mask_piece[r * 8 + f];
            }
        }

        passed_pawn_mask[sq] = mask;
    }
}

u64 BitBoardGenerator::find_magic(int sq, int m, bool bishop) {
    assert(sq >= 0 && sq < 64);

    u64 mask, b[4096], a[4096], used[4096], magic;
    int i, j, k, n, fail;

    mask = bishop ? bishop_masks[sq] : rook_masks[sq];
    n = count_1s(mask);

    for (i = 0; i < mask_piece[n]; i++) {
        b[i] = index_to_u64(i, n, mask);
        a[i] = bishop ? generate_bishop_attacks(sq, b[i]) : generate_rook_attacks(sq, b[i]);
    }

    for (k = 0; k < 100000000; k++) {
        magic = random_u64_fewbits();

        if (count_1s((mask * magic) & 0xff00000000000000ULL) < 6) {
            continue;
        }

        for (i = 0; i < 4096; i++) {
            used[i] = 0ULL;
        }

        for (i = 0, fail = 0; !fail && i < mask_piece[n]; i++) {
            j = transform(b[i], magic, m);
            if (used[j] == 0ULL) {
                used[j] = a[i];
            } else if (used[j] != a[i]) {
                fail = 1;
            }
        }

        if (!fail) {
            return magic;
        }
    }
    std::cout << "FAILED" << "\n";
    return 0ULL;
}

u64 BitBoardGenerator::generate_bishop_attacks(int sq, u64 block) {
    assert(sq >= 0 && sq < 64);

    u64 result = 0ULL;
    int rank = sq / 8;
    int file = sq % 8;
    int r, f;

    for (r = rank + 1, f = file + 1; r <= 7 && f <= 7; r++, f++) {
        int i = f + r * 8;
        result |= mask_piece[i];
        if (block & mask_piece[i]) {
            break;
        }
    }

    for (r = rank + 1, f = file - 1; r <= 7 && f >= 0; r++, f--) {
        int i = f + r * 8;
        result |= mask_piece[i];
        if (block & mask_piece[i]) {
            break;
        }
    }

    for (r = rank - 1, f = file + 1; r >= 0 && f <= 7; r--, f++) {
        int i = f + r * 8;
        result |= mask_piece[i];
        if (block & mask_piece[i]) {
            break;
        }
    }

    for (r = rank - 1, f = file - 1; r >= 0 && f >= 0; r--, f--) {
        int i = f + r * 8;
        result |= mask_piece[i];
        if (block & mask_piece[i]) {
            break;
        }
    }

    return result;
}

u64 BitBoardGenerator::generate_rook_attacks(int sq, u64 block) {
    assert(sq >= 0 && sq < 64);

    u64 result = 0ULL;
    int rank = sq / 8;
    int file = sq % 8;
    int r, f;

    for (r = rank + 1; r < 8; r++) {
        int i = file + r * 8;
        result |= mask_piece[i];
        if (block & mask_piece[i]) {
            break;
        }
    }

    for (r = rank - 1; r >= 0; r--) {
        int i = file + r * 8;
        result |= mask_piece[i];
        if (block & mask_piece[i]) {
            break;
        }
    }

    for (f = file + 1; f < 8; f++) {
        int i = f + rank * 8;
        result |= mask_piece[i];
        if (block & mask_piece[i]) {
            break;
        }
    }

    for (f = file - 1; f >= 0; f--) {
        int i = f + rank * 8;
        result |= mask_piece[i];
        if (block & mask_piece[i]) {
            break;
        }
    }

    return result;
}

u64 BitBoardGenerator::generate_bishop_mask(int sq) {
    assert(sq >= 0 && sq < 64);

    u64 mask = 0ULL;
    int rank = sq / 8;
    int file = sq % 8;

    for (int dr = -1; dr <= 1; dr += 2) {
        for (int df = -1; df <= 1; df += 2) {
            int tr = rank + dr;
            int tf = file + df;

            while (tr > 0 && tr < 7 && tf > 0 && tf < 7) {
                mask |= mask_piece[tr * 8 + tf];
                tr += dr;
                tf += df;
            }
        }
    }
    return mask;
}

u64 BitBoardGenerator::generate_rook_mask(int sq) {
    assert(sq >= 0 && sq < 64);

    u64 mask = 0ULL;
    int rank = sq / 8;
    int file = sq % 8;

    for (int r = rank + 1; r < 7; r++) {
        mask |= mask_piece[r * 8 + file];
    }

    for (int r = rank - 1; r > 0; r--) {
        mask |= mask_piece[r * 8 + file];
    }

    for (int f = file + 1; f < 7; f++) {
        mask |= mask_piece[rank * 8 + f];
    }

    for (int f = file - 1; f > 0; f--) {
        mask |= mask_piece[rank * 8 + f];
    }

    return mask;
}

u64 BitBoardGenerator::pieces_attacking_square(u64 *bitboards, int sq, bool turn) {
    assert(sq >= 0 && sq < 64);

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
    res |= bitboards[king_type] & king_attack_pattern[sq];

    // Sliding pieces using magic bitboards
    u64 occupancy = bitboards[ALL];

    // Bishop-like attacks (bishops and queens)
    u64 bishop_attackers = generate_bishop_bitboard(sq, occupancy);
    res |= bishop_attackers &
           (bitboards[turn ? WHITE_BISHOP : BLACK_BISHOP] | bitboards[turn ? WHITE_QUEEN : BLACK_QUEEN]);

    // Rook-like attacks (rooks and queens)
    u64 rook_attackers = generate_rook_bitboard(sq, occupancy);
    res |= rook_attackers & (bitboards[turn ? WHITE_ROOK : BLACK_ROOK] | bitboards[turn ? WHITE_QUEEN : BLACK_QUEEN]);

    return res;
}

u64 BitBoardGenerator::pieces_attacking_king(u64 *bitboards, bool turn) {
    int king_pos = turn ? first_bit(bitboards[WHITE_KING]) : first_bit(bitboards[BLACK_KING]);
    return pieces_attacking_square(bitboards, king_pos, !turn);
}

u64 BitBoardGenerator::generate_attacks_bitboard(ChessLogic &game, bool turn) {
    u64 *bitboards = game.get_board().get_bitboards();
    u64 bishops = turn ? bitboards[WHITE_BISHOP] : bitboards[BLACK_BISHOP];
    u64 rooks = turn ? bitboards[WHITE_ROOK] : bitboards[BLACK_ROOK];
    u64 queens = turn ? bitboards[WHITE_QUEEN] : bitboards[BLACK_QUEEN];
    u64 all_attacks = 0;
    u64 friendly = turn ? bitboards[WHITE] : bitboards[BLACK];

    while (bishops) {
        int sq = first_bit(bishops);
        u64 bishop = generate_bishop_bitboard(sq, bitboards[ALL]);
        bishop &= ~friendly;
        all_attacks |= bishop;
        bishops &= bishops - 1;
    }
    while (rooks) {
        int sq = first_bit(rooks);
        u64 rook = generate_rook_bitboard(sq, bitboards[ALL]);
        rook &= ~friendly;
        all_attacks |= rook;
        rooks &= rooks - 1;
    }
    while (queens) {
        int sq = first_bit(queens);
        u64 queen = generate_queen_bitboard(sq, bitboards[ALL]);
        queen &= ~friendly;
        all_attacks |= queen;
        queens &= queens - 1;
    }
    for (int i = 0; i < 8; i++) {
        if (square_attacked_by_pawn_or_knight(CBits[i], bitboards, !turn)) {
            all_attacks |= mask_piece[CBits[i]];
        }
    }
    return all_attacks;
}

u64 BitBoardGenerator::generate_pawn_bitboard(u64 *bitboards, u64 pawn, bool turn) {
    u64 empty = ~bitboards[ALL];

    u64 single_push, double_push;
    if (turn) {
        single_push = shift_north(pawn) & empty;
        double_push = shift_north(single_push) & empty & mask_rank[3];
    } else {
        single_push = shift_south(pawn) & empty;
        double_push = shift_south(single_push) & empty & mask_rank[4];
    }
    u64 all_pushes = single_push | double_push;

    return all_pushes;
}

u64 BitBoardGenerator::generate_pawn_captures_bitboard(ChessLogic &game, bool turn) {
    u64 *bitboards = game.get_board().get_bitboards();
    u64 pawns = turn ? bitboards[WHITE_PAWN] : bitboards[BLACK_PAWN];
    u64 res = 0;

    u64 nw_pawns, ne_pawns;
    u64 sw_pawns, se_pawns;
    if (turn) {
        nw_pawns = shift_north_west(pawns);
        ne_pawns = shift_north_east(pawns);
        nw_pawns &= bitboards[BLACK];
        ne_pawns &= bitboards[BLACK];
        res = nw_pawns | ne_pawns;
    } else {
        sw_pawns = shift_south_west(pawns);
        se_pawns = shift_south_east(pawns);
        sw_pawns &= bitboards[WHITE];
        se_pawns &= bitboards[WHITE];
        res = sw_pawns | se_pawns;
    }

    return res;
}

u64 BitBoardGenerator::generate_king_bitboard(ChessLogic &game, bool turn) {
    u64 *bitboards = game.get_board().get_bitboards();
    u64 king = turn ? bitboards[WHITE_KING] : bitboards[BLACK_KING];
    u64 mask = turn ? ~bitboards[WHITE] : ~bitboards[BLACK];
    int pos = first_bit(king);
    // Pseudolegal king attacks
    u64 moves = king_attack_pattern[pos] & mask;
    return moves;
}

u64 BitBoardGenerator::generate_bishop_bitboard(int sq, u64 occ) {
    assert(sq >= 0 && sq < 64);

    u64 mask = bishop_masks[sq];
    u64 blockers = occ & mask;
    int index = (int)((blockers * bishop_magics[sq]) >> (64 - BBits[sq]));
    return bishop_attacks[sq][index];
}

u64 BitBoardGenerator::generate_rook_bitboard(int sq, u64 occ) {
    assert(sq >= 0 && sq < 64);

    u64 mask = rook_masks[sq];
    u64 blockers = occ & mask;
    int index = (int)((blockers * rook_magics[sq]) >> (64 - RBits[sq]));
    return rook_attacks[sq][index];
}

u64 BitBoardGenerator::generate_queen_bitboard(int sq, u64 occ) {
    assert(sq >= 0 && sq < 64);

    u64 bishop = generate_bishop_bitboard(sq, occ);
    u64 rook = generate_rook_bitboard(sq, occ);
    return bishop | rook;
}

u64 BitBoardGenerator::generate_castle_bitboard(ChessLogic &game, bool turn) {
    u64 *bitboards = game.get_board().get_bitboards();
    u64 attacks = generate_attacks_bitboard(game, !turn);
    u64 king_attacks = pieces_attacking_king(bitboards, turn);
    u64 occ = bitboards[ALL];
    u64 res = 0;

    if (king_attacks) {
        return res;
    }
    if (turn) {
        bool wkr_present = mask_piece[7] & bitboards[WHITE_ROOK];
        bool wkq_present = mask_piece[0] & bitboards[WHITE_ROOK];
        if (game.get_wk_castle() && (attacks & wk_castle_mask) == 0 && (occ & wk_castle_mask) == 0 && wkr_present) {
            res |= mask_piece[6];
        }
        if (game.get_wq_castle() && (attacks & wq_castle_mask_t) == 0 && (occ & wq_castle_mask) == 0 && wkq_present) {
            res |= mask_piece[2];
        }
    } else {
        bool bkr_present = mask_piece[63] & bitboards[BLACK_ROOK];
        bool bkq_present = mask_piece[56] & bitboards[BLACK_ROOK];
        if (game.get_bk_castle() && (attacks & bk_castle_mask) == 0 && (occ & bk_castle_mask) == 0 && bkr_present) {
            res |= mask_piece[62];
        }
        if (game.get_bq_castle() && (attacks & bq_castle_mask_t) == 0 && (occ & bq_castle_mask) == 0 && bkq_present) {
            res |= mask_piece[58];
        }
    }
    return res;
}

u64 BitBoardGenerator::generate_pinned_pieces_bitboard(u64 *bitboards, bool turn) {
    u64 pinned = 0;
    u64 own_pieces = turn ? bitboards[WHITE] : bitboards[BLACK];
    u64 op_rq = turn ? bitboards[BLACK_ROOK] | bitboards[BLACK_QUEEN] : bitboards[WHITE_ROOK] | bitboards[WHITE_QUEEN];
    u64 op_bq =
        turn ? bitboards[BLACK_BISHOP] | bitboards[BLACK_QUEEN] : bitboards[WHITE_BISHOP] | bitboards[WHITE_QUEEN];
    int king_sq = turn ? first_bit(bitboards[WHITE_KING]) : first_bit(bitboards[BLACK_KING]);

    u64 pinner = xray_rook_attacks(bitboards[ALL], own_pieces, king_sq) & op_rq;
    while (pinner) {
        int sq = first_bit(pinner);
        pinned |= precomputed_in_between[sq][king_sq] & own_pieces;
        pinner &= pinner - 1;
    }
    pinner = xray_bishop_attacks(bitboards[ALL], own_pieces, king_sq) & op_bq;
    while (pinner) {
        int sq = first_bit(pinner);
        pinned |= precomputed_in_between[sq][king_sq] & own_pieces;
        pinner &= pinner - 1;
    }
    return pinned;
}

u64 BitBoardGenerator::xray_rook_attacks(u64 occ, u64 blockers, int sq) {
    assert(sq >= 0 && sq < 64);

    u64 attacks = generate_rook_bitboard(sq, occ);
    blockers &= attacks;
    u64 var = generate_rook_bitboard(sq, occ ^ blockers);
    return attacks ^ var;
}

u64 BitBoardGenerator::xray_bishop_attacks(u64 occ, u64 blockers, int sq) {
    assert(sq >= 0 && sq < 64);

    u64 attacks = generate_bishop_bitboard(sq, occ);
    blockers &= attacks;
    u64 var = generate_bishop_bitboard(sq, occ ^ blockers);
    return attacks ^ var;
}

int BitBoardGenerator::get_pinning_piece_square(u64 *bitboards, int sq, bool turn) {
    assert(sq >= 0 && sq < 64);

    u64 own_pieces = turn ? bitboards[WHITE] : bitboards[BLACK];
    u64 op_rq = turn ? bitboards[BLACK_ROOK] | bitboards[BLACK_QUEEN] : bitboards[WHITE_ROOK] | bitboards[WHITE_QUEEN];
    u64 op_bq =
        turn ? bitboards[BLACK_BISHOP] | bitboards[BLACK_QUEEN] : bitboards[WHITE_BISHOP] | bitboards[WHITE_QUEEN];
    int king_sq = turn ? first_bit(bitboards[WHITE_KING]) : first_bit(bitboards[BLACK_KING]);

    u64 pinner = xray_rook_attacks(bitboards[ALL], own_pieces, king_sq) & op_rq;
    while (pinner) {
        int pinner_sq = first_bit(pinner);
        if (precomputed_in_between[king_sq][pinner_sq] & mask_piece[sq]) {
            return pinner_sq;
        }
        pinner &= pinner - 1;
    }

    pinner = xray_bishop_attacks(bitboards[ALL], own_pieces, king_sq) & op_bq;
    while (pinner) {
        int pinner_sq = first_bit(pinner);
        if (precomputed_in_between[king_sq][pinner_sq] & mask_piece[sq]) {
            return pinner_sq;
        }
        pinner &= pinner - 1;
    }

    return 64;
}

bool BitBoardGenerator::square_attacked_by_pawn_or_knight(int sq, u64 *bitboards, bool enemy_turn) {
    assert(sq >= 0 && sq < 64);

    // enemy_turn = the color of the potential attacker
    u64 attackers = 0;

    if (enemy_turn) {
        // Black attackers
        attackers |= shift_north_west(mask_piece[sq]) & bitboards[BLACK_PAWN];
        attackers |= shift_north_east(mask_piece[sq]) & bitboards[BLACK_PAWN];
        attackers |= knight_attack_squares[sq] & bitboards[BLACK_KNIGHT];
    } else {
        // White attackers
        attackers |= shift_south_west(mask_piece[sq]) & bitboards[WHITE_PAWN];
        attackers |= shift_south_east(mask_piece[sq]) & bitboards[WHITE_PAWN];
        attackers |= knight_attack_squares[sq] & bitboards[WHITE_KNIGHT];
    }

    return attackers != 0;
}

int BitBoardGenerator::evaluate_passed_pawns(u64 white, u64 black) {

}