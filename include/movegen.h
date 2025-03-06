#ifndef MOVEGEN_H
#define MOVEGEN_H
#include "utils.h"
#include "chessgame.h"

class MoveGenerator {
private:
    static u64 generate_white_pawn_pushes(ChessGame &);
    static u64 generate_black_pawn_pushes(ChessGame &);
};

#endif // !MOVEGEN_H
