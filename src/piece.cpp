#include "../include/piece.h"
#include "../include/utils.h"

Piece::Piece(Position p, PieceColor c) : pos(p), color(c) {};

Position Piece::get_pos() const {
    return pos;
}

PieceColor Piece::get_color() const {
    return color;
}

bool Piece::set_pos(unsigned int newRow, unsigned int newCol) {
    pos.row = newRow;
    pos.col = newCol;
    return true;
}

Pawn::Pawn(Position p, PieceColor c) : Piece(p, c) {};

char Pawn::draw_piece() const {
    return 'p';
}

Rook::Rook(Position p, PieceColor c) : Piece(p, c) {};

char Rook::draw_piece() const {
    return 'R';
}

Bishop::Bishop(Position p, PieceColor c) : Piece(p, c) {};

char Bishop::draw_piece() const {
    return 'B';
}

Knight::Knight(Position p, PieceColor c) : Piece(p, c) {};

char Knight::draw_piece() const {
    return 'N';
}

Queen::Queen(Position p, PieceColor c) : Piece(p, c) {};

char Queen::draw_piece() const {
    return 'Q';
}

King::King(Position p, PieceColor c) : Piece(p, c) {};

char King::draw_piece() const {
    return 'K';
}
