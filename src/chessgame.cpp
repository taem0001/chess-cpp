#include "../include/chessgame.h"
#include "../include/fen.h"

ChessGame::ChessGame() : board() { FenHandler::load_fen(*this, STARTPOS); }

void ChessGame::draw_game() { board.draw_board(); }

bool ChessGame::make_move(u16 move) {
    fens[move] = FenHandler::write_fen(*this);
    u64 *bitboards = get_board().get_bitboards();
    u16 from = get_from(move);
    u16 to = get_to(move);
    u16 flag = get_flag(move);
    half_moves++;

    if (!white_turn) {
        full_moves++;
    }

    // Reset castling rights if the king moves
    if (bitboards[WHITE_KING] & mask_piece[from]) {
        wk_castle = wq_castle = false;
    }
    if (bitboards[BLACK_KING] & mask_piece[from]) {
        bk_castle = bq_castle = false;
    }

    // Reset halfmove clock counter
    if (bitboards[WHITE_PAWN] & mask_piece[from] || bitboards[BLACK_PAWN] & mask_piece[from]) {
        half_moves = 0;
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

    // Handle move types
    int rook_sq, en_passant_capture;
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
        case capture:
            half_moves = 0;
            board.remove_piece(to);
            break;
        case ep_capture:
            half_moves = 0;
            en_passant_capture = white_turn ? en_passant_square - 8 : en_passant_square + 8;
            board.remove_piece(en_passant_capture);
            break;
        case knight_promotion:
            board.promote_piece(white_turn, 'n', to);
            break;
        case bishop_promotion:
            board.promote_piece(white_turn, 'b', to);
            break;
        case rook_promotion:
            board.promote_piece(white_turn, 'r', to);
            break;
        case queen_promotion:
            board.promote_piece(white_turn, 'q', to);
            break;
        case knight_promo_capture:
            board.remove_piece(to);
            board.promote_piece(white_turn, 'n', to);
            break;
        case bishop_promo_capture:
            board.remove_piece(to);
            board.promote_piece(white_turn, 'b', to);
            break;
        case rook_promo_capture:
            board.remove_piece(to);
            board.promote_piece(white_turn, 'r', to);
            break;
        case queen_promo_capture:
            board.remove_piece(to);
            board.promote_piece(white_turn, 'q', to);
            break;
        default:
            break;
    }

    // Reset en passant square
    if (en_passant_square != -1) {
        en_passant_square = -1;
    }

    board.move_piece(from, to);
    return true;
}

bool ChessGame::unmake_move(u16 move) {
    std::string pos = fens[move];
    FenHandler::load_fen(*this, pos);
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
int ChessGame::get_halfmoves() { return half_moves; }
void ChessGame::set_halfmoves(int n) { half_moves = n; }
int ChessGame::get_fullmoves() { return full_moves; }
void ChessGame::set_fullmoves(int n) { full_moves = n; }
bool ChessGame::get_singlecheck() { return single_check; }
void ChessGame::set_singlecheck(bool b) { single_check = b; }
bool ChessGame::get_doublecheck() { return double_check; }
void ChessGame::set_doublecheck(bool b) { double_check = b; }
std::map<u16, std::string> ChessGame::get_fens() { return fens; }
