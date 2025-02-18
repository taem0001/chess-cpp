#ifndef CHESSLOGIC_H
#define CHESSLOGIC_H
#include "chessboard.h"

class ChessLogic {
  private:
    ChessBoard *board;
    bool white_turn;

  public:
    ChessLogic();
    ~ChessLogic();
    ChessBoard *get_board();
    bool make_move(Position, Position);
};

#endif // !CHESSLOGIC_H
