#ifndef MOVEGEN_H
#define MOVEGEN_H
#include "utils.h"
#include "bitboard.h"

class MoveGenerator {
public:
    static void init();
    static std::vector<u16> generate_pseudolegal_moves(ChessGame &);

private:
    static void generate_pawn_moves(std::vector<u16> &, ChessGame &);
    static void generate_knight_moves(std::vector<u16> &, ChessGame &);
};

#endif // !MOVEGEN_H
