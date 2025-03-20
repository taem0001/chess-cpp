#ifndef BITBOARD_H
#define BITBOARD_H
#include "chessgame.h"
#include "utils.h"

class BitBoardGenerator {
public:
    static u64 rays[8][64];
    static u64 precomputed_bishop[64];
    static u64 precomputed_rook[64];

    static void init();

    static u64 diag_attacks(u64, unsigned long);
    static u64 anti_diag_attacks(u64, unsigned long);
    static u64 rank_attacks(u64, unsigned long);
    static u64 file_attacks(u64, unsigned long);
    static u64 pieces_attacking_square(ChessGame &, int, bool);
    static u64 pieces_attacking_king(ChessGame &, bool);

    static u64 generate_attacks_bitboard(ChessGame &, bool);

    static u64 generate_pawn_bitboard(u64 *, bool);
    static u64 generate_king_bitboard(u64 *, bool);
    static u64 generate_knight_bitboard(u64 *, bool);
    static u64 generate_bishop_bitboard(u64 *, bool);
    static u64 generate_rook_bitboard(u64 *, bool);
    static u64 generate_queen_bitboard(u64 *, bool);
    static u64 generate_castle_bitboard(ChessGame &, bool);
    static u64 generate_pawn_captures_bitboard(ChessGame &, bool);

private:
    static u64 get_positive_rays(u64, Direction, unsigned long);
    static u64 get_negative_rays(u64, Direction, unsigned long);
};

#endif // !BITBOARD_H
