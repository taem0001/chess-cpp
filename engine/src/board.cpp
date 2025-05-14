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

void Board::promote_piece(bool turn, int type, int sq) {
    assert(sq >= 0 && sq < 64);
    assert(type == WHITE_KNIGHT || type == BLACK_KNIGHT || type == WHITE_BISHOP || type == BLACK_BISHOP || type == WHITE_ROOK || type == BLACK_ROOK || type == WHITE_QUEEN || type == BLACK_QUEEN);

    // Remove pawn on the promote square
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

void Board::add_piece(int sq, int type) {
    assert(sq >= 0 && sq < 64);
    assert(type >= 3 && type < 15);
    assert(piece_on_square[sq] == -1);
    assert(bitboards[type] & mask_piece[sq] == 0);

    piece_on_square[sq] = type;
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
    assert(type >= 3 && type < 15);
    assert(bitboards[type] & mask_piece[sq] == 0);

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

void Board::init_zobrist() {
    for (int i = 0; i < 12; i++) {
        for (int sq = 0; sq < 64; sq++) {
            zobrist_pieces[i][sq] = random_u64();
        }
    }

    zobrist_blacktomove = random_u64();

    for (int i = 0; i < 16; i++) {
        zobrist_castle[i] = random_u64();
    }

    for (int file = 0; file < 8; file++) {
        zobrist_epfile[file] = random_u64();
    }
}

u64 Board::get_zobrist_piece(int type, int sq) {
    assert(type >= 3 && type < 15);
    assert(sq >= 0 && sq < 64);

    return zobrist_pieces[type - 3][sq];
}

u64 Board::get_zobrist_blacktomove() { return zobrist_blacktomove; }

u64 Board::get_zobrist_castle(int castle_rights) {
    assert(castle_rights >= 0 && castle_rights < 16);
    return zobrist_castle[castle_rights];
} 

u64 Board::get_zobrist_ep(int ep_sq) {
    return zobrist_epfile[ep_sq % 8];
}