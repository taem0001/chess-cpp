#ifndef MOVEGEN_H
#define MOVEGEN_H
#include "bitboard.h"
#include "chesslogic.h"
#include "utils.h"

class MoveGenerator {
public:
    static void init();
    static std::vector<u16> generate_legal_moves(ChessLogic &);

private:
    static void generate_pawn_pushes(std::vector<u16> &, ChessLogic &, u64);
    static void generate_pawn_captures(std::vector<u16> &, ChessLogic &, u64);
    static void generate_knight_moves(std::vector<u16> &, ChessLogic &, u64);
    static void generate_king_moves(std::vector<u16> &, ChessLogic &);
    static void generate_bishop_moves(std::vector<u16> &, ChessLogic &, u64);
    static void generate_rook_moves(std::vector<u16> &, ChessLogic &, u64);
    static void generate_queen_moves(std::vector <u16> &, ChessLogic &, u64);

    static bool simulate_check(u64 *, int *, bool, int, int);
    static std::vector<u16> handle_single_check(std::vector<u16> &, ChessLogic &);
};

#endif // !MOVEGEN_H
