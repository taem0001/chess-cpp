#include "../include/chesslogic.h"
#include "../include/fen.h"

ChessLogic::ChessLogic() : board() {
    load_pos(STARTPOS);
    total_moves = 1;
}

void ChessLogic::load_pos(const std::string &fen) { FenHandler::load_fen(*this, fen); }

void ChessLogic::draw_game() { board.draw_board(); }

bool ChessLogic::make_move(u64 move) {
    int *piece_on_square = board.get_piece_on_squares();
    u64 *bitboards = board.get_bitboards();

    int from = (int)get_from(move);
    int to = (int)get_to(move);
    int flag = (int)get_flag(move);
    assert(from >= 0 && from < 64);
    assert(to >= 0 && to < 64);
    assert(flag >= 0 && flag < 16);

    int captured_piece_type = -1;

    // Store data for unmake function
    MoveData undo_data;
    undo_data.bkc = bk_castle;
    undo_data.bqc = bq_castle;
    undo_data.wkc = wk_castle;
    undo_data.wqc = wq_castle;
    undo_data.turn = white_turn;
    undo_data.ep_sq = en_passant_square;
    undo_data.half_moves = half_moves;
    undo_data.full_moves = full_moves;

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

    // Handle move types
    int rook_sq, en_passant_capture;
    switch (flag) {
        case double_pawn_push:
            board.move_piece(from, to);
            en_passant_square = white_turn ? to - 8 : to + 8;
            break;
        case king_castle:
            board.move_piece(from, to);
            rook_sq = white_turn ? 7 : 63;
            board.move_piece(rook_sq, to - 1);
            break;
        case queen_castle:
            board.move_piece(from, to);
            rook_sq = white_turn ? 0 : 56;
            board.move_piece(rook_sq, to + 1);
            break;
        case capture:
            captured_piece_type = piece_on_square[to];
            board.move_piece(from, to);
            half_moves = 0;
            break;
        case ep_capture:
            board.move_piece(from, to);
            half_moves = 0;
            en_passant_capture = white_turn ? ep_temp - 8 : ep_temp + 8;
            captured_piece_type = piece_on_square[en_passant_capture];
            board.remove_piece(en_passant_capture);
            break;
        case knight_promotion:
            board.move_piece(from, to);
            board.promote_piece(white_turn, 'n', to);
            break;
        case bishop_promotion:
            board.move_piece(from, to);
            board.promote_piece(white_turn, 'b', to);
            break;
        case rook_promotion:
            board.move_piece(from, to);
            board.promote_piece(white_turn, 'r', to);
            break;
        case queen_promotion:
            board.move_piece(from, to);
            board.promote_piece(white_turn, 'q', to);
            break;
        case knight_promo_capture:
            captured_piece_type = piece_on_square[to];
            board.move_piece(from, to);
            half_moves = 0;
            board.promote_piece(white_turn, 'n', to);
            break;
        case bishop_promo_capture:
            captured_piece_type = piece_on_square[to];
            board.move_piece(from, to);
            half_moves = 0;
            board.promote_piece(white_turn, 'b', to);
            break;
        case rook_promo_capture:
            captured_piece_type = piece_on_square[to];
            board.move_piece(from, to);
            half_moves = 0;
            board.promote_piece(white_turn, 'r', to);
            break;
        case queen_promo_capture:
            captured_piece_type = piece_on_square[to];
            board.move_piece(from, to);
            half_moves = 0;
            board.promote_piece(white_turn, 'q', to);
            break;
        default:
            board.move_piece(from, to);
            break;
    }

    undo_data.captured_piece_type = captured_piece_type;
    undo_stack.push_back(undo_data);

    if (!white_turn) full_moves++;
    change_turn();
    total_moves++;
    return true;
}

bool ChessLogic::unmake_move(u64 move) {
    assert(undo_stack.size() != 0);

    int from = (int)get_from(move);
    int to = (int)get_to(move);
    int flag = (int)get_flag(move);
    assert(from >= 0 && from < 64);
    assert(to >= 0 && to < 64);
    assert(flag >= 0 && flag < 16);

    // Pop previous move data from stack
    MoveData undo_data = undo_stack.back();
    undo_stack.pop_back();

    // Reset pieces to previous position
    if (flag == quiet_move || flag == double_pawn_push) {
        board.move_piece(to, from);
    } else if (flag == king_castle) {
        int rook_from = undo_data.turn ? 5 : 61;
        int rook_to = rook_from + 2;
        board.move_piece(rook_from, rook_to);
        board.move_piece(to, from);
    } else if (flag == queen_castle) {
        int rook_from = undo_data.turn ? 3 : 59;
        int rook_to = rook_from - 3;
        board.move_piece(rook_from, rook_to);
        board.move_piece(to, from);
    } else if (flag == capture) {
        assert(undo_data.captured_piece_type != -1);
        board.move_piece(to, from);
        board.add_piece(to, undo_data.captured_piece_type);        
    } else if (flag == ep_capture) {
        assert(undo_data.captured_piece_type != -1);
        board.move_piece(to, from);
        int sq = undo_data.turn ? to - 8 : to + 8;
        board.add_piece(sq, undo_data.captured_piece_type);
    } else if (flag >= knight_promotion && flag < knight_promo_capture) {
        board.remove_piece(to);
        int pawn = undo_data.turn ? WHITE_PAWN : BLACK_PAWN;
        board.add_piece(from, pawn);
    } else {
        assert(undo_data.captured_piece_type != -1);
        board.remove_piece(to);
        int pawn = undo_data.turn ? WHITE_PAWN : BLACK_PAWN;
        board.add_piece(from, pawn);
        board.add_piece(to, undo_data.captured_piece_type);
    }

    // Restore game state
    white_turn = undo_data.turn;
    bk_castle = undo_data.bkc;
    bq_castle = undo_data.bqc;
    wk_castle = undo_data.wkc;
    wq_castle = undo_data.wqc;
    half_moves = undo_data.half_moves;
    full_moves = undo_data.full_moves;
    en_passant_square = undo_data.ep_sq;
    total_moves--;

    return true;
}

// TODO: Complete this when zobrist hashing is implemented
bool ChessLogic::fivefold_repitition() {
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