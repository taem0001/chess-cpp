#include "../include/board.h"

u64 *Board::get_bitboards() { return bitboards; }
int *Board::get_piece_on_squares() { return piece_on_square; }

void Board::draw_board() {
    std::string line = "  +---+---+---+---+---+---+---+---+";
    std::string files = "    a   b   c   d   e   f   g   h";

    std::cout << files << "\n";
    std::cout << line << "\n";
    for (int rank = 15; rank >= 0; rank--) {
        if (rank % 2 == 0) {
            std::cout << line << "\n";
        } else {
            std::cout << (rank / 2) + 1 << " ";
            for (int file = 0; file < 8; file++) {
                int square = (rank / 2) * 8 + file;
                std::cout << "| " << get_symbol(bitboards, square) << " ";
            }
            std::cout << "| " << (rank / 2) + 1 << "\n";
        }
    }
    std::cout << files << "\n";
}

void Board::move_piece(int from, int to) {
    assert(from >= 0 && from < 64);
    assert(to >= 0 && to < 64);

    // Remove captured piece
    if (piece_on_square[to] != -1) {
        bitboards[piece_on_square[to]] &= ~mask_piece[to];
        piece_on_square[to] = -1;
    }

    // Move piece from the from square
    int piece_from = piece_on_square[from];
    bitboards[piece_from] &= ~mask_piece[from];
    piece_on_square[from] = -1;

    // Add the piece to the to square
    bitboards[piece_from] |= mask_piece[to];
    piece_on_square[to] = piece_from;

    bitboards[WHITE] = bitboards[WHITE_PAWN] | bitboards[WHITE_ROOK] | bitboards[WHITE_BISHOP] |
                       bitboards[WHITE_KNIGHT] | bitboards[WHITE_QUEEN] | bitboards[WHITE_KING];
    bitboards[BLACK] = bitboards[BLACK_PAWN] | bitboards[BLACK_ROOK] | bitboards[BLACK_BISHOP] |
                       bitboards[BLACK_KNIGHT] | bitboards[BLACK_QUEEN] | bitboards[BLACK_KING];
    bitboards[ALL] = bitboards[WHITE] | bitboards[BLACK];
}

void Board::promote_piece(bool turn, char piece, int sq) {
    assert(sq >= 0 && sq < 64);
    assert(piece == 'n' || piece == 'N' || piece == 'b' || piece == 'B' || piece == 'r' || piece == 'R' || piece == 'q' || piece == 'Q');

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

    // Update piece to square mapping
    piece_on_square[sq] = type;

    // Update the bitboards
    bitboards[type] |= mask_piece[sq];
    bitboards[WHITE] = bitboards[WHITE_PAWN] | bitboards[WHITE_ROOK] | bitboards[WHITE_BISHOP] |
                       bitboards[WHITE_KNIGHT] | bitboards[WHITE_QUEEN] | bitboards[WHITE_KING];
    bitboards[BLACK] = bitboards[BLACK_PAWN] | bitboards[BLACK_ROOK] | bitboards[BLACK_BISHOP] |
                       bitboards[BLACK_KNIGHT] | bitboards[BLACK_QUEEN] | bitboards[BLACK_KING];
    bitboards[ALL] = bitboards[WHITE] | bitboards[BLACK];
}

void Board::remove_piece(int sq) {
    assert(sq >= 0 && sq < 64);

    int type = piece_on_square[sq];

    // If the square is not empty, remove the piece
    if (type != -1) {
        // Update the square to piece mapping
        piece_on_square[sq] = -1;

        // Update the bitboards
        bitboards[type] &= ~mask_piece[sq];
        bitboards[WHITE] = bitboards[WHITE_PAWN] | bitboards[WHITE_ROOK] | bitboards[WHITE_BISHOP] |
                       bitboards[WHITE_KNIGHT] | bitboards[WHITE_QUEEN] | bitboards[WHITE_KING];
        bitboards[BLACK] = bitboards[BLACK_PAWN] | bitboards[BLACK_ROOK] | bitboards[BLACK_BISHOP] |
                        bitboards[BLACK_KNIGHT] | bitboards[BLACK_QUEEN] | bitboards[BLACK_KING];
        bitboards[ALL] = bitboards[WHITE] | bitboards[BLACK];
    }
}
