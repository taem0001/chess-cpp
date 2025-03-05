#ifndef MOVE_H
#define MOVE_H

#include "board.h"

class MoveGenerator {
public:
    static std::vector<Move> generate_legal_moves(Board &);
    static void handle_check(Board &, std::vector<Move> &);

private:
    static void generate_sliding_moves(Board &, int, std::vector<Move> &);
    static void generate_knight_moves(Board &, int, std::vector<Move> &);
    static void generate_pawn_moves(Board &, int, std::vector<Move> &);
    static void generate_king_moves(Board &, int, std::vector<Move> &);
};

#endif // !MOVE_H
