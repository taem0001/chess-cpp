#include "../include/chesslogic.h"
#include "../include/fen.h"

ChessLogic::ChessLogic() : board() {
    load_pos(STARTPOS);
    total_moves = 1;
}

void ChessLogic::load_pos(const std::string &fen) { FenHandler::load_fen(*this, fen); }

void ChessLogic::draw_game() { board.draw_board(); }

bool ChessLogic::make_move(u64 move) {
    fens[move] = FenHandler::write_fen(*this);
    int *piece_on_square = board.get_piece_on_squares();
    u64 *bitboards = board.get_bitboards();

    int from = (int)get_from(move);
    int to = (int)get_to(move);
    int flag = (int)get_flag(move);

    assert(from >= 0 && from < 64);
    assert(to >= 0 && to < 64);
    assert(flag >= 0 && flag < 16);

    // Reset castling rights if the king moves
    if (piece_on_square[from] == WHITE_KING) {
        wk_castle = wq_castle = false;
    }
    if (piece_on_square[from] == BLACK_KING) {
        bk_castle = bq_castle = false;
    }

    // Reset halfmove clock counter
    half_moves++;
    if (piece_on_square[from] == WHITE_PAWN || piece_on_square[from] == BLACK_PAWN) {
        half_moves = 0;
    }

    // Reset castling rights if rooks move from their original squares
    if (from == 0 && piece_on_square[from] == WHITE_ROOK) {
        wq_castle = false;
    }
    if (from == 7 && piece_on_square[from] == WHITE_ROOK) {
        wk_castle = false;
    }
    if (from == 56 && piece_on_square[from] == BLACK_ROOK) {
        bq_castle = false;
    }
    if (from == 63 && piece_on_square[from] == BLACK_ROOK) {
        bk_castle = false;
    }

    // Reset en passant square
    int ep_temp = en_passant_square;
    en_passant_square = -1;

    board.move_piece(from, to);

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
            break;
        case ep_capture:
            half_moves = 0;
            en_passant_capture = white_turn ? ep_temp - 8 : ep_temp + 8;
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
            half_moves = 0;
            board.promote_piece(white_turn, 'n', to);
            break;
        case bishop_promo_capture:
            half_moves = 0;
            board.promote_piece(white_turn, 'b', to);
            break;
        case rook_promo_capture:
            half_moves = 0;
            board.promote_piece(white_turn, 'r', to);
            break;
        case queen_promo_capture:
            half_moves = 0;
            board.promote_piece(white_turn, 'q', to);
            break;
        default:
            break;
    }
    if (!white_turn) full_moves++;
    change_turn();
    total_moves++;
    return true;
}

bool ChessLogic::unmake_move(u64 move) {
    std::string pos = fens[move];
    load_pos(pos);
    return true;
}

bool ChessLogic::fivefold_repitition() {
    std::map<std::string, int> freq;

    for (const auto &pair : fens) {
        freq[pair.second]++;
    }

    for (const auto &pair : freq) {
        if (pair.second == 5) {
            return true;
        }
    }

    return false;
}

Board &ChessLogic::get_board() { return board; }
void ChessLogic::change_turn() { white_turn = !white_turn; }
void ChessLogic::set_turn(bool b) { white_turn = b; }
bool ChessLogic::get_turn() { return white_turn; }
bool ChessLogic::get_wk_castle() { return wk_castle; }
bool ChessLogic::get_wq_castle() { return wq_castle; }
bool ChessLogic::get_bk_castle() { return bk_castle; }
bool ChessLogic::get_bq_castle() { return bq_castle; }
void ChessLogic::set_wk_castle(bool b) { wk_castle = b; }
void ChessLogic::set_wq_castle(bool b) { wq_castle = b; }
void ChessLogic::set_bk_castle(bool b) { bk_castle = b; }
void ChessLogic::set_bq_castle(bool b) { bq_castle = b; }
int ChessLogic::get_en_passant_sq() { return en_passant_square; }
void ChessLogic::set_en_passant_sq(int sq) { en_passant_square = sq; }
int ChessLogic::get_halfmoves() { return half_moves; }
void ChessLogic::set_halfmoves(int n) { half_moves = n; }
int ChessLogic::get_fullmoves() { return full_moves; }
void ChessLogic::set_fullmoves(int n) { full_moves = n; }
bool ChessLogic::get_singlecheck() { return single_check; }
void ChessLogic::set_singlecheck(bool b) { single_check = b; }
bool ChessLogic::get_doublecheck() { return double_check; }
void ChessLogic::set_doublecheck(bool b) { double_check = b; }
bool ChessLogic::fifty_move_rule() { return half_moves >= 100; }
u64 ChessLogic::get_totalmoves() { return total_moves; }
std::map<u64, std::string> ChessLogic::get_fens() { return fens; }
