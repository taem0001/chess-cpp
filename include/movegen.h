#ifndef MOVEGEN_H
#define MOVEGEN_H
#include "bitboard.h"
#include "chesslogic.h"
#include "utils.h"

class MoveGenerator {
public:
    static void init();
    static std::vector<u64> generate_legal_moves(ChessLogic &);

private:
    static void generate_pawn_pushes(std::vector<u64> &, ChessLogic &);
    static void generate_pawn_captures(std::vector<u64> &, ChessLogic &);
    static void generate_knight_moves(std::vector<u64> &, ChessLogic &);
    static void generate_king_moves(std::vector<u64> &, ChessLogic &);
    static void generate_bishop_moves(std::vector<u64> &, ChessLogic &);
    static void generate_rook_moves(std::vector<u64> &, ChessLogic &);
    static void generate_queen_moves(std::vector <u64> &, ChessLogic &);

    static void check_detection(u64 *, bool *, bool *, bool);
    static bool simulate_check(u64 *, PieceType, bool, int, int);
    static std::vector<u64> handle_single_check(std::vector<u64> &, ChessLogic &);
};

#endif // !MOVEGEN_H
