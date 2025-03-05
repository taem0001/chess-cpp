#ifndef CHESSGAME_H
#define CHESSGAME_H
#include "board.h"

class ChessGame {
private:
    Board board;

public:
    ChessGame();
    void draw_game();
};

#endif // !CHESSGAME_H
