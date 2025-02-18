#include "../include/piece.h"

// TODO: Implement is_valid_move function for the other pieces
// Parent class Piece
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

// Child class Pawn
Pawn::Pawn(Position p, PieceColor c) : Piece(p, c) {};

char Pawn::draw_piece() const {
    return 'p';
}

bool Pawn::is_valid_move(Position new_pos, Piece *board[8][8]) const {
    if (pos.row == new_pos.row && pos.col == new_pos.col) {
        return false;
    }

    int forward_dir = color == PieceColor::WHITE ? -1 : 1;
    unsigned int row_diff = abs(pos.row - new_pos.row);
    unsigned int col_diff = abs(pos.col - new_pos.col);

    // Moving the pawn one square forward
    if (row_diff == 1 && col_diff == 0 && !board[new_pos.row][new_pos.col]) {
        return true;
    }

    bool first_move = (color == PieceColor::WHITE && pos.row == 6) || (color == PieceColor::BLACK && pos.col == 1);

    // Initial two squares forward
    if (row_diff == 2 && col_diff == 0 && !board[new_pos.row][new_pos.col] && !board[pos.row + forward_dir][pos.col] && first_move) {
        return true;
    }

    // Capturing a piece
    if (row_diff == 1 && col_diff == 1 && board[new_pos.row][new_pos.col] && board[new_pos.row][new_pos.col]->get_color() != color) {
        return true;
    }

    return false;
}

// Child class Rook
Rook::Rook(Position p, PieceColor c) : Piece(p, c) {};

char Rook::draw_piece() const {
    return 'R';
}

bool Rook::is_valid_move(Position new_pos, Piece *board[8][8]) const {
    return false;
}

// Child class Bishop
Bishop::Bishop(Position p, PieceColor c) : Piece(p, c) {};

char Bishop::draw_piece() const {
    return 'B';
}

bool Bishop::is_valid_move(Position new_pos, Piece *board[8][8]) const {
    return false;
}

// Child class Knight
Knight::Knight(Position p, PieceColor c) : Piece(p, c) {};

char Knight::draw_piece() const {
    return 'N';
}

bool Knight::is_valid_move(Position new_pos, Piece *board[8][8]) const {
    return false;
}

// Child class Queen
Queen::Queen(Position p, PieceColor c) : Piece(p, c) {};

char Queen::draw_piece() const {
    return 'Q';
}

bool Queen::is_valid_move(Position new_pos, Piece *board[8][8]) const {
    return false;
}

// Child class King
King::King(Position p, PieceColor c) : Piece(p, c) {};

char King::draw_piece() const {
    return 'K';
}

bool King::is_valid_move(Position new_pos, Piece *board[8][8]) const {
    return false;
}
