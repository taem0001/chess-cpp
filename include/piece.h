#ifndef PIECE_H
#define PIECE_H
#include "utils.h"

class Piece {
  protected:
    Position pos;
    const PieceColor color;

  public:
    Piece(Position, PieceColor);
    virtual ~Piece() {}
    Position get_pos() const;
    PieceColor get_color() const;
    bool set_pos(unsigned int, unsigned int);
    virtual char draw_piece() const = 0;
    virtual bool is_valid_move(Position, Piece *[8][8]) const = 0;
};

class Pawn : public Piece {
  public:
    Pawn(Position, PieceColor);
    virtual char draw_piece() const override;
    virtual bool is_valid_move(Position, Piece *[8][8]) const override;
};

class Rook : public Piece {
  public:
    Rook(Position, PieceColor);
    virtual char draw_piece() const override;
    virtual bool is_valid_move(Position, Piece *[8][8]) const override;
};

class Bishop : public Piece {
  public:
    Bishop(Position, PieceColor);
    virtual char draw_piece() const override;
    virtual bool is_valid_move(Position, Piece *[8][8]) const override;
};

class Knight : public Piece {
  public:
    Knight(Position, PieceColor);
    virtual char draw_piece() const override;
    virtual bool is_valid_move(Position, Piece *[8][8]) const override;
};

class Queen : public Piece {
  public:
    Queen(Position, PieceColor);
    virtual char draw_piece() const override;
    virtual bool is_valid_move(Position, Piece *[8][8]) const override;
};

class King : public Piece {
  public:
    King(Position, PieceColor);
    virtual char draw_piece() const override;
    virtual bool is_valid_move(Position, Piece *[8][8]) const override;
};

#endif // !PIECE_H
