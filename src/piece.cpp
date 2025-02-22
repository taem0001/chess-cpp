#include "../include/piece.h"

const unsigned char Piece::type_mask = 0b111;
const unsigned char Piece::color_mask = 0b11000;

unsigned char Piece::make_piece(unsigned char type, unsigned char color) {
    return color | type;
}

unsigned char Piece::get_type(unsigned char piece) {
    return piece & type_mask;
}

bool Piece::is_piece_white(unsigned char piece) {
    unsigned char color = piece & Piece::color_mask;
    return color == Piece::white;
}

bool Piece::is_sliding_piece(unsigned char piece) {
    unsigned char type = piece & type_mask;
    return type == queen || type == bishop || type == rook;
}

bool Piece::is_friendly(unsigned char p1, unsigned char p2) {
    unsigned char p1_color = p1 & color_mask;
    unsigned char p2_color = p2 & color_mask;
    return p1_color == p2_color;
}

char Piece::get_symbol(unsigned char piece) {
    char res;
    switch (piece) {
    case 0:
        res = '*';
        break;
    case static_cast<unsigned char>(PieceType::BLACK_PAWN):
        res = 'p';
        break;
    case static_cast<unsigned char>(PieceType::BLACK_KNIGHT):
        res = 'n';
        break;
    case static_cast<unsigned char>(PieceType::BLACK_BISHOP):
        res = 'b';
        break;
    case static_cast<unsigned char>(PieceType::BLACK_ROOK):
        res = 'r';
        break;
    case static_cast<unsigned char>(PieceType::BLACK_QUEEN):
        res = 'q';
        break;
    case static_cast<unsigned char>(PieceType::BLACK_KING):
        res = 'k';
        break;
    case static_cast<unsigned char>(PieceType::WHITE_PAWN):
        res = 'P';
        break;
    case static_cast<unsigned char>(PieceType::WHITE_KNIGHT):
        res = 'N';
        break;
    case static_cast<unsigned char>(PieceType::WHITE_BISHOP):
        res = 'B';
        break;
    case static_cast<unsigned char>(PieceType::WHITE_ROOK):
        res = 'R';
        break;
    case static_cast<unsigned char>(PieceType::WHITE_QUEEN):
        res = 'Q';
        break;
    case static_cast<unsigned char>(PieceType::WHITE_KING):
        res = 'K';
        break;
    }

    return res;
}
