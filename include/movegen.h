#ifndef MOVEGEN_H
#define MOVEGEN_H
#include "utils.h"
#include "chessgame.h"

class MoveGenerator {
public:
    static u64 generate_moves(ChessGame &);;

private:
    static u64 generate_white_pawn_pushes(ChessGame &);
    static u64 generate_black_pawn_pushes(ChessGame &);
    static u64 generate_white_king_moves(ChessGame &);
    static u64 generate_black_king_moves(ChessGame &);
    static u64 generate_white_knight_moves(ChessGame &);
    static u64 generate_black_knight_moves(ChessGame &);
};

#endif // !MOVEGEN_H
