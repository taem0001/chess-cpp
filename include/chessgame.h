#ifndef CHESSGAME_H
#define CHESSGAME_H
#include "board.h"

class ChessGame {
private:
    Board board;
    bool white_turn;

public:
    ChessGame();
    void draw_game();
    Board &get_board();
    void change_turn();
    void set_turn(bool);
    bool get_turn();
};

#endif // !CHESSGAME_H
