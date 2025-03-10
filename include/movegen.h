#ifndef MOVEGEN_H
#define MOVEGEN_H
#include "utils.h"
#include "chessgame.h"

class MoveGenerator {
public:
    static u64 rays[8][64];
    static u64 rank_attacks[64];
    static u64 file_attacks[64];
    static u64 diag_attacks[64];
    static u64 anti_diag_attacks[64];

    static u64 generate_moves(ChessGame &);;
    static void init();

private:
    static void init_rays();
    static void init_sliding_attacks();

    static u64 generate_white_pawn_pushes(ChessGame &);
    static u64 generate_black_pawn_pushes(ChessGame &);
    static u64 generate_white_king_moves(ChessGame &);
    static u64 generate_black_king_moves(ChessGame &);
    static u64 generate_white_knight_moves(ChessGame &);
    static u64 generate_black_knight_moves(ChessGame &);
    static u64 generate_white_bishop_attacks(ChessGame &);
    static u64 generate_black_bishop_attacks(ChessGame &);
    static u64 generate_white_rook_attacks(ChessGame &);
    static u64 generate_black_rook_attacks(ChessGame &);
    static u64 generate_white_queen_attacks(ChessGame &);
    static u64 generate_black_queen_attacks(ChessGame &);
};

#endif // !MOVEGEN_H
