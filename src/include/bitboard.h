#ifndef BITBOARD_H
#define BITBOARD_H
#include "chesslogic.h"
#include "utils.h"
#include "magic.h"

class BitBoardGenerator {
public:
    static u64 precomputed_in_between[64][64];
    static u64 bishop_attacks[64][512];
    static u64 rook_attacks[64][4096];
    static u64 passed_pawn_mask[64];

    static void init();

    static u64 pieces_attacking_square(u64 *, int, bool);
    static u64 pieces_attacking_king(u64 *, bool);

    static u64 generate_attacks_bitboard(ChessLogic&, bool);

    static u64 generate_pawn_bitboard(u64 *, u64, bool);
    static u64 generate_bishop_bitboard(int, u64);
    static u64 generate_rook_bitboard(int, u64);
    static u64 generate_queen_bitboard(int, u64);
    static u64 generate_king_bitboard(ChessLogic&, bool);
    static u64 generate_castle_bitboard(ChessLogic&, bool);
    static u64 generate_pawn_captures_bitboard(ChessLogic&, bool);
    static u64 generate_pinned_pieces_bitboard(u64 *, bool);

    static int get_pinning_piece_square(u64 *, int, bool);

private:
    static u64 bishop_masks[64];
    static u64 rook_masks[64];
    static u64 bishop_magics[64];
    static u64 rook_magics[64];

    static u64 find_magic(int, int, bool);
    static u64 generate_bishop_attacks(int, u64);
    static u64 generate_rook_attacks(int, u64);
    static u64 generate_bishop_mask(int);
    static u64 generate_rook_mask(int);
    static u64 xray_rook_attacks(u64, u64, int);
    static u64 xray_bishop_attacks(u64, u64, int);
    static bool square_attacked_by_pawn_or_knight(int, u64 *, bool);
};

#endif // !BITBOARD_H
