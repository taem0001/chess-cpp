#ifndef MOVEGEN_H
#define MOVEGEN_H
#include "chessgame.h"
#include "utils.h"

class MoveGenerator {
public:
    static u64 rays[8][64];

    static u64 generate_moves(ChessGame &);
    static void init();

private:
    static u64 get_positive_rays(u64, Direction, unsigned long);
    static u64 get_negative_rays(u64, Direction, unsigned long);
    static u64 diag_attacks(u64, unsigned long);
    static u64 anti_diag_attacks(u64, unsigned long);
    static u64 rank_attacks(u64, unsigned long);
    static u64 file_attacks(u64, unsigned long);

    static u64 generate_white_pawn_bitboard(ChessGame &);
    static u64 generate_black_pawn_bitboard(ChessGame &);
    static u64 generate_white_king_bitboard(ChessGame &);
    static u64 generate_black_king_bitboard(ChessGame &);
    static u64 generate_white_knight_bitboard(ChessGame &);
    static u64 generate_black_knight_bitboard(ChessGame &);
    static u64 generate_white_bishop_bitboard(ChessGame &);
    static u64 generate_black_bishop_bitboard(ChessGame &);
    static u64 generate_white_rook_bitboard(ChessGame &);
    static u64 generate_black_rook_bitboard(ChessGame &);
    static u64 generate_white_queen_bitboard(ChessGame &);
    static u64 generate_black_queen_bitboard(ChessGame &);
};

#endif // !MOVEGEN_H
