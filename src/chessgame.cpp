#include "../include/chessgame.h"
#include "../include/fen.h"

ChessGame::ChessGame() : board() {
    FenHandler::load_fen(*this, STARTPOS);
}

void ChessGame::draw_game() {
    board.draw_board();
}

bool ChessGame::make_move(u16 move) {
    u64 *bitboards = get_board().get_bitboards();
    u16 from = get_from(move);
    u16 to = get_to(move);
    u16 flag = get_flag(move);

    // Reset castling rights if the king moves
    if (bitboards[WHITE_KING] & mask_piece[from]) {
        wk_castle = wq_castle = false;
    }
    if (bitboards[BLACK_KING] & mask_piece[from]) {
        bk_castle = bq_castle = false;
    }

    // Reset castling rights if rooks move from their original squares
    if (from == 0 && bitboards[WHITE_ROOK] & mask_piece[from]) {
        wq_castle = false;
    }
    if (from == 7 && bitboards[WHITE_ROOK] & mask_piece[from]) {
        wk_castle = false;
    }
    if (from == 56 && bitboards[BLACK_ROOK] & mask_piece[from]) {
        bq_castle = false;
    }
    if (from == 63 && bitboards[BLACK_ROOK] & mask_piece[from]) {
        bk_castle = false;
    }

    // TODO: Implement logic for other flags later
    // Handle move types
    int rook_sq;
    switch (flag) {
        case quiet_move:
            break;
        case double_pawn_push:
            en_passant_square = white_turn ? to - 8 : to + 8;
            break;
        case king_castle:
            rook_sq = white_turn ? 7 : 63;
            board.move_piece(rook_sq, to - 1);
            break;
        case queen_castle:
            rook_sq = white_turn ? 0 : 56;
            board.move_piece(rook_sq, to + 1);
            break;
        default:
            break;
    }

    board.move_piece(from, to);
    return true;
}

bool ChessGame::unmake_move(u16 move) {
    u64 *bitboards = get_board().get_bitboards();
    u16 from = get_from(move);
    u16 to = get_to(move);
    u16 flag = get_flag(move);

    // Handle castling rights restoration based on move
    if (bitboards[WHITE_KING] & mask_piece[to]) {
        wk_castle = wq_castle = true;
    }
    if (bitboards[BLACK_KING] & mask_piece[to]) {
        bk_castle = bq_castle = true;
    }

    if (from == 0 && bitboards[WHITE_ROOK] & mask_piece[to]) {
        wq_castle = true;
    }
    if (from == 7 && bitboards[WHITE_ROOK] & mask_piece[to]) {
        wk_castle = true;
    }
    if (from == 56 && bitboards[BLACK_ROOK] & mask_piece[to]) {
        bq_castle = true;
    }
    if (from == 63 && bitboards[BLACK_ROOK] & mask_piece[to]) {
        bk_castle = true;
    }

    // TODO: Implement logic for rest of the flags
    // Handle move types
    int rook_sq;
    switch (flag) {
        case quiet_move:
            break;
        case double_pawn_push:
            en_passant_square = white_turn ? from + 8 : from - 8;
            break;
        case king_castle:
            rook_sq = white_turn ? 7 : 63;
            board.move_piece(to - 1, rook_sq);
            break;
        case queen_castle:
            rook_sq = white_turn ? 0 : 56;
            board.move_piece(to + 1, rook_sq);
            break;
        default:
            break;
    }

    // Move piece back to original square
    board.move_piece(to, from);

    return true;
}

Board &ChessGame::get_board() { return board; }
void ChessGame::change_turn() { white_turn = !white_turn; }
void ChessGame::set_turn(bool b) { white_turn = b; }
bool ChessGame::get_turn() { return white_turn; }
bool ChessGame::get_wk_castle() { return wk_castle; }
bool ChessGame::get_wq_castle() { return wq_castle; }
bool ChessGame::get_bk_castle() { return bk_castle; }
bool ChessGame::get_bq_castle() { return bq_castle; }
void ChessGame::set_wk_castle(bool b) { wk_castle = b; }
void ChessGame::set_wq_castle(bool b) { wq_castle = b; }
void ChessGame::set_bk_castle(bool b) { bk_castle = b; }
void ChessGame::set_bq_castle(bool b) { bq_castle = b; }
int ChessGame::get_en_passant_sq() { return en_passant_square; }
void ChessGame::set_en_passant_sq(int sq) { en_passant_square = sq; }
