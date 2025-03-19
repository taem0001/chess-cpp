#include "../include/board.h"

u64 *Board::get_bitboards() { return bitboards; }

void Board::draw_board() {
    int square;

    std::cout << "    a b c d e f g h" << std::endl;
    std::cout << "  +-----------------+" << std::endl;
    for (int rank = 7; rank >= 0; rank--) {
        std::cout << rank + 1 << " | ";
        for (int file = 0; file < 8; file++) {
            square = rank * 8 + file;
            std::cout << get_symbol(bitboards, square) << " ";
        }
        std::cout << "| " << rank + 1 << std::endl;
    }
    std::cout << "  +-----------------+" << std::endl;
    std::cout << "    a b c d e f g h" << std::endl;
}

void Board::move_piece(int from, int to) {
    for (int i = 3; i < 15; i++) {
        if (bitboards[i] & mask_piece[from]) {
            bitboards[i] &= ~mask_piece[from];
            bitboards[i] |= mask_piece[to];
        }
    }

    bitboards[WHITE] = bitboards[WHITE_PAWN] | bitboards[WHITE_ROOK] |
                       bitboards[WHITE_BISHOP] | bitboards[WHITE_KNIGHT] |
                       bitboards[WHITE_QUEEN] | bitboards[WHITE_KING];
    bitboards[BLACK] = bitboards[BLACK_PAWN] | bitboards[BLACK_ROOK] |
                       bitboards[BLACK_BISHOP] | bitboards[BLACK_KNIGHT] |
                       bitboards[BLACK_QUEEN] | bitboards[BLACK_KING];
    bitboards[ALL] = bitboards[WHITE] | bitboards[BLACK];
}

void Board::promote_piece(bool turn, char piece, int sq) {
    PieceType type;
    switch (piece) {
        case 'n':
        case 'N':
            type = turn ? WHITE_KNIGHT : BLACK_KNIGHT;
            break;
        case 'b':
        case 'B':
            type = turn ? WHITE_BISHOP : BLACK_BISHOP;
            break;
        case 'r':
        case 'R':
            type = turn ? WHITE_ROOK : BLACK_ROOK;
            break;
        case 'q':
        case 'Q':
            type = turn ? WHITE_QUEEN : BLACK_QUEEN;
            break;
        default:
            break;
    }
    if (turn) {
        bitboards[WHITE_PAWN] &= ~mask_piece[sq];
    } else {
        bitboards[BLACK_PAWN] &= ~mask_piece[sq];
    }
    bitboards[type] |= mask_piece[sq];
    bitboards[WHITE] = bitboards[WHITE_PAWN] | bitboards[WHITE_ROOK] |
                       bitboards[WHITE_BISHOP] | bitboards[WHITE_KNIGHT] |
                       bitboards[WHITE_QUEEN] | bitboards[WHITE_KING];
    bitboards[BLACK] = bitboards[BLACK_PAWN] | bitboards[BLACK_ROOK] |
                       bitboards[BLACK_BISHOP] | bitboards[BLACK_KNIGHT] |
                       bitboards[BLACK_QUEEN] | bitboards[BLACK_KING];
    bitboards[ALL] = bitboards[WHITE] | bitboards[BLACK];
}
