#ifndef CHESSGAME_H
#define CHESSGAME_H
#include "board.h"

class ChessGame {
private:
    Board board;
    bool white_turn;
    bool wk_castle, wq_castle, bk_castle, bq_castle;
    bool single_check, double_check;
    int en_passant_square;
    int half_moves;
    int full_moves;

public:
    ChessGame();
    void draw_game();
    bool make_move(u16);
    bool unmake_move(u16);
    Board &get_board();
    void change_turn();
    void set_turn(bool);
    bool get_turn();
    bool get_wk_castle();
    bool get_wq_castle();
    bool get_bk_castle();
    bool get_bq_castle();
    void set_wk_castle(bool);
    void set_wq_castle(bool);
    void set_bk_castle(bool);
    void set_bq_castle(bool);
    int get_en_passant_sq();
    void set_en_passant_sq(int);
    int get_halfmoves();
    void set_halfmoves(int);
    int get_fullmoves();
    void set_fullmoves(int);
    bool get_singlecheck();
    void set_singlecheck(bool);
    bool get_doublecheck();
    void set_doublecheck(bool);
};

#endif // !CHESSGAME_H
