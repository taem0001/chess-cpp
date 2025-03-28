#ifndef BITBOARD_H
#define BITBOARD_H
#include "chessgame.h"
#include "utils.h"

class BitBoardGenerator {
public:
    static u64 rays[8][64];
    static u64 precomputed_bishop[64];
    static u64 precomputed_rook[64];
    static u64 precomputed_in_between[64][64];

    static void init();

    static u64 diag_attacks(u64, unsigned long);
    static u64 anti_diag_attacks(u64, unsigned long);
    static u64 rank_attacks(u64, unsigned long);
    static u64 file_attacks(u64, unsigned long);
    static u64 pieces_attacking_square(u64 *, int, bool);
    static u64 pieces_attacking_king(u64 *, bool);

    static u64 generate_attacks_bitboard(ChessGame &, bool);

    static u64 generate_pawn_bitboard(u64 *, u64, bool);
    static u64 generate_bishop_bitboard(u64 *, int, bool);
    static u64 generate_rook_bitboard(u64 *, int, bool);
    static u64 generate_queen_bitboard(u64 *, int, bool);
    static u64 generate_king_bitboard(ChessGame &, bool);
    static u64 generate_castle_bitboard(ChessGame &, bool);
    static u64 generate_pawn_captures_bitboard(ChessGame &, bool);
    static u64 generate_pinned_pieces_bitboard(u64 *, bool);

    static int get_pinning_piece_square(u64 *, int, bool);

private:
    static u64 get_positive_rays(u64, Direction, unsigned long);
    static u64 get_negative_rays(u64, Direction, unsigned long);
    static u64 xray_rook_attacks(u64, u64, int);
    static u64 xray_bishop_attacks(u64, u64, int);
    static u64 generate_knight_bitboard(u64 *, bool);
};

#endif // !BITBOARD_H
