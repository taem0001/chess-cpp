#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "piece.h"

class ChessBoard {
  private:
    Piece *chessBoard[8][8];

  public:
    ChessBoard();
    ~ChessBoard();
    void draw_board();
};

#endif // !CHESSBOARD_H
