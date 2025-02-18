#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "piece.h"

class ChessBoard {
  private:
    Piece *board[8][8];

  public:
    ChessBoard();
    ~ChessBoard();
    Piece *get_piece(Position);
    Piece *(*get_board())[8][8];
    void move_piece(Position, Position);
    void draw_board();
};

#endif // !CHESSBOARD_H
