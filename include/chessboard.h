#include "piece.h"

class ChessBoard {
  private:
    Piece *chessBoard[8][8];

  public:
    ChessBoard();
    void draw_board();
};
