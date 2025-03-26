#ifndef MOVEGEN_H
#define MOVEGEN_H
#include "bitboard.h"
#include "chessgame.h"
#include "utils.h"

class MoveGenerator {
public:
    static void init();
    static std::vector<u16> generate_legal_moves(ChessGame &);

private:
    static void generate_pawn_pushes(std::vector<u16> &, ChessGame &);
    static void generate_pawn_captures(std::vector<u16> &, ChessGame &);
    static void generate_knight_moves(std::vector<u16> &, ChessGame &);
    static void generate_king_moves(std::vector<u16> &, ChessGame &);
    static void generate_bishop_moves(std::vector<u16> &, ChessGame &);
    static void generate_rook_moves(std::vector<u16> &, ChessGame &);
    static void generate_queen_moves(std::vector <u16> &, ChessGame &);

    static void check_detection(ChessGame &);
    static std::vector<u16> handle_single_check(std::vector<u16> &, ChessGame &);
};

#endif // !MOVEGEN_H
