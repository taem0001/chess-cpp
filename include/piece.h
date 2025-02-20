#pragma once

class Piece {
  private:
    static const unsigned char type_mask;
    static const unsigned char color_mask;

  public:
    static const unsigned char none = 0;
    static const unsigned char pawn = 1;
    static const unsigned char knight = 2;
    static const unsigned char bishop = 3;
    static const unsigned char rook = 4;
    static const unsigned char queen = 5;
    static const unsigned char king = 6;
    static const unsigned char white = 8;
    static const unsigned char black = 16;

    static unsigned char make_piece(unsigned char, unsigned char);
    static bool is_piece_white(unsigned char);
    static char get_symbol(unsigned char);
};

enum class PieceType {
    WHITE_PAWN = Piece::white | Piece::pawn,
    WHITE_KNIGHT = Piece::white | Piece::knight,
    WHITE_BISHOP = Piece::white | Piece::bishop,
    WHITE_ROOK = Piece::white | Piece::rook,
    WHITE_QUEEN = Piece::white | Piece::queen,
    WHITE_KING = Piece::white | Piece::king,
    BLACK_PAWN = Piece::black | Piece::pawn,
    BLACK_KNIGHT = Piece::black | Piece::knight,
    BLACK_BISHOP = Piece::black | Piece::bishop,
    BLACK_ROOK = Piece::black | Piece::rook,
    BLACK_QUEEN = Piece::black | Piece::queen,
    BLACK_KING = Piece::black | Piece::king,
};
