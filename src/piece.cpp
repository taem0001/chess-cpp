#include "../include/piece.h"

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
    if (color == PieceColor::WHITE) {
        return 'P';
    }
    return 'p';
}

bool Pawn::is_valid_move(Position new_pos, Piece *board[8][8]) const {
    if (pos.row == new_pos.row && pos.col == new_pos.col) {
        return false;
    }

    int forward_dir = color == PieceColor::WHITE ? -1 : 1;
    unsigned int row_diff = _abs(pos.row - new_pos.row);
    unsigned int col_diff = _abs(pos.col - new_pos.col);

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
    if (color == PieceColor::WHITE) {
        return 'R';
    }
    return 'r';
}

bool Rook::is_valid_move(Position new_pos, Piece *board[8][8]) const {
    if (pos.row == new_pos.row && pos.col == new_pos.col) {
        return false;
    }

    unsigned int row_diff = _abs(pos.row - new_pos.row);
    unsigned int col_diff = _abs(pos.col - new_pos.col);

    // Only diagonal movement is allowed
    if (row_diff > 0 && col_diff > 0) {
        return false;
    }

    if (row_diff == 0) { // Check horizontal movement
        unsigned int start_col = _min(new_pos.col, pos.col) + 1;
        unsigned int end_col = _max(new_pos.col, pos.col);

        for (int col = start_col; col < end_col; col++) {
            Piece *rook = board[new_pos.row][col];
            if (rook) {
                return false;
            }
        }
    } else if (col_diff == 0) { // Check vertical movement
        unsigned int start_row = _min(new_pos.row, pos.row) + 1;
        unsigned int end_row = _max(new_pos.row, pos.row);

        for (int row = start_row; row < end_row; row++) {
            Piece *rook = board[row][new_pos.col];
            if (rook) {
                return false;
            }
        }
    }

    Piece *piece = board[new_pos.row][new_pos.col];

    if (!piece) {
        return true;
    } else if (piece->get_color() != this->color) {
        return true;
    }

    return false;
}

// Child class Bishop
Bishop::Bishop(Position p, PieceColor c) : Piece(p, c) {};

char Bishop::draw_piece() const {
    if (color == PieceColor::WHITE) {
        return 'B';
    }
    return 'b';
}

bool Bishop::is_valid_move(Position new_pos, Piece *board[8][8]) const {
    if (pos.row == new_pos.row && pos.col == new_pos.col) {
        return false;
    }

    int row_diff = pos.row - new_pos.row;
    int col_diff = pos.col - new_pos.col;

    if (_abs(row_diff) != _abs(col_diff)) {
        return false;
    }

    int row_dir = row_diff > 0 ? -1 : 1;
    int col_dir = col_diff > 0 ? -1 : 1;
    int steps = _abs(row_diff);

    for (int i = 1; i < steps; i++) {
        if (board[i * row_dir + pos.row][i * col_dir + pos.col]) {
            return false;
        }
    }

    Piece *piece = board[new_pos.row][new_pos.col];

    if (!piece) {
        return true;
    } else if (piece->get_color() != this->color) {
        return true;
    }

    return false;
}

// Child class Knight
Knight::Knight(Position p, PieceColor c) : Piece(p, c) {};

char Knight::draw_piece() const {
    if (color == PieceColor::WHITE) {
        return 'N';
    }
    return 'n';
}

bool Knight::is_valid_move(Position new_pos, Piece *board[8][8]) const {
    if (pos.row == new_pos.row && pos.col == new_pos.col) {
        return false;
    }

    unsigned int row_diff = _abs(pos.row - new_pos.row);
    unsigned int col_diff = _abs(pos.col - new_pos.col);

    bool valid_move = (row_diff == 2 && col_diff == 1) || (row_diff == 1 && col_diff == 2);

    if (!valid_move) {
        return false;
    }

    Piece *piece = board[new_pos.row][new_pos.col];

    if (!piece) {
        return true;
    } else if (piece->get_color() != this->color) {
        return true;
    }

    return false;
}

// Child class Queen
Queen::Queen(Position p, PieceColor c) : Piece(p, c) {};

char Queen::draw_piece() const {
    if (color == PieceColor::WHITE) {
        return 'Q';
    }
    return 'q';
}

bool Queen::is_valid_move(Position new_pos, Piece *board[8][8]) const {
    if (pos.row == new_pos.row && pos.col == new_pos.col) {
        return false;
    }

    int row_diff = pos.row - new_pos.row;
    int col_diff = pos.col - new_pos.col;

    if (row_diff == 0) { // Check horizontal movement
        unsigned int start_col = _min(new_pos.col, pos.col) + 1;
        unsigned int end_col = _max(new_pos.col, pos.col);

        for (int col = start_col; col < end_col; col++) {
            Piece *queen = board[new_pos.row][col];
            if (queen) {
                return false;
            }
        }
    } else if (col_diff == 0) { // Check vertical movement
        unsigned int start_row = _min(new_pos.row, pos.row) + 1;
        unsigned int end_row = _max(new_pos.row, pos.row);

        for (int row = start_row; row < end_row; row++) {
            Piece *rook = board[row][new_pos.col];
            if (rook) {
                return false;
            }
        }
    } else if (_abs(row_diff) == _abs(col_diff)) {
        int row_dir = row_diff > 0 ? -1 : 1;
        int col_dir = col_diff > 0 ? -1 : 1;
        int steps = _abs(row_diff);

        for (int i = 1; i < steps; i++) {
            if (board[i * row_dir + pos.row][i * col_dir + pos.col]) {
                return false;
            }
        }
    }

    Piece *piece = board[new_pos.row][new_pos.col];

    if (!piece) {
        return true;
    } else if (piece->get_color() != this->color) {
        return true;
    }

    return false;
}

// Child class King
King::King(Position p, PieceColor c) : Piece(p, c) {};

char King::draw_piece() const {
    if (color == PieceColor::WHITE) {
        return 'K';
    }
    return 'k';
}

bool King::is_valid_move(Position new_pos, Piece *board[8][8]) const {
    if (pos.row == new_pos.row && pos.col == new_pos.col) {
        return false;
    }

    unsigned int row_diff = _abs(pos.row - new_pos.row);
    unsigned int col_diff = _abs(pos.col - new_pos.col);

    if (row_diff > 1 || col_diff > 1) {
        return false;
    }

    Piece *piece = board[new_pos.row][new_pos.col];

    if (!piece) {
        return true;
    } else if (piece->get_color() != this->color) {
        return true;
    }

    return false;
}
