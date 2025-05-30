#include "../include/chesslogic.h"
#include "../include/fen.h"

ChessLogic::ChessLogic() : total_pieces(0), board() {
    board.init_zobrist();
    load_pos(STARTPOS);
}

void ChessLogic::load_pos(const std::string &fen) {
    FenHandler::load_fen(*this, fen);
    zobrist_hash = compute_zobrist_hash();
}

void ChessLogic::draw_game() { board.draw_board(); }

bool ChessLogic::make_move(u16 move) {
    int *piece_on_square = board.get_piece_on_squares();
    u64 *bitboards = board.get_bitboards();

    int from = (int)get_from(move);
    int to = (int)get_to(move);
    int flag = (int)get_flag(move);
    assert(from >= 0 && from < 64);
    assert(to >= 0 && to < 64);
    assert(flag >= 0 && flag < 16);

    int moved_piece = piece_on_square[from];
    int captured_piece_type = -1;

    // Store data for unmake function
    MoveData undo_data;
    undo_data.zobrist_hash = zobrist_hash;
    undo_data.bkc = bk_castle;
    undo_data.bqc = bq_castle;
    undo_data.wkc = wk_castle;
    undo_data.wqc = wq_castle;
    undo_data.turn = white_turn;
    undo_data.ep_sq = en_passant_square;
    undo_data.half_moves = half_moves;
    undo_data.full_moves = full_moves;

    // Save castling rights in hash before changes
    int old_castling = get_castling_mask(bk_castle, bq_castle, wk_castle, wq_castle);
    if (old_castling)
        zobrist_hash ^= board.get_zobrist_castle(old_castling);

    // Save en passant in hash before changes and reset
    int old_ep = en_passant_square;
    if (old_ep != -1)
        zobrist_hash ^= board.get_zobrist_ep(old_ep);
    en_passant_square = -1;

    // Remove moving piece from origin in hash
    zobrist_hash ^= board.get_zobrist_piece(moved_piece, from);

    // Handle move types
    int rook_sq, en_passant_capture, promoted;
    switch (flag) {
        case double_pawn_push:
            board.move_piece(from, to);
            zobrist_hash ^= board.get_zobrist_piece(moved_piece, to);
            en_passant_square = white_turn ? to - 8 : to + 8;
            zobrist_hash ^= board.get_zobrist_ep(en_passant_square);
            break;
        case king_castle:
            board.move_piece(from, to);
            rook_sq = white_turn ? 7 : 63;
            board.move_piece(rook_sq, to - 1);
            zobrist_hash ^= board.get_zobrist_piece(moved_piece, to);
            zobrist_hash ^= board.get_zobrist_piece(piece_on_square[to - 1], to - 1);
            zobrist_hash ^= board.get_zobrist_piece(piece_on_square[to - 1], rook_sq);
            break;
        case queen_castle:
            board.move_piece(from, to);
            rook_sq = white_turn ? 0 : 56;
            board.move_piece(rook_sq, to + 1);
            zobrist_hash ^= board.get_zobrist_piece(moved_piece, to);
            zobrist_hash ^= board.get_zobrist_piece(piece_on_square[to + 1], to + 1);
            zobrist_hash ^= board.get_zobrist_piece(piece_on_square[to + 1], rook_sq);
            break;
        case capture:
            captured_piece_type = piece_on_square[to];
            zobrist_hash ^= board.get_zobrist_piece(captured_piece_type, to);
            board.move_piece(from, to);
            zobrist_hash ^= board.get_zobrist_piece(moved_piece, to);
            half_moves = 0;
            total_pieces--;
            break;
        case ep_capture:
            board.move_piece(from, to);
            half_moves = 0;
            en_passant_capture = white_turn ? to - 8 : to + 8;
            captured_piece_type = piece_on_square[en_passant_capture];
            board.remove_piece(en_passant_capture);
            zobrist_hash ^= board.get_zobrist_piece(captured_piece_type, en_passant_capture);
            zobrist_hash ^= board.get_zobrist_piece(moved_piece, to);
            total_pieces--;
            break;
        case knight_promotion:
        case bishop_promotion:
        case rook_promotion:
        case queen_promotion:
            board.move_piece(from, to);
            promoted = get_promoted_piece_index(flag, white_turn);
            board.promote_piece(white_turn, promoted, to);
            zobrist_hash ^= board.get_zobrist_piece(promoted, to);
            break;
        case knight_promo_capture:
        case bishop_promo_capture:
        case rook_promo_capture:
        case queen_promo_capture:
            captured_piece_type = piece_on_square[to];
            zobrist_hash ^= board.get_zobrist_piece(captured_piece_type, to);
            board.move_piece(from, to);
            half_moves = 0;
            promoted = get_promoted_piece_index(flag, white_turn);
            board.promote_piece(white_turn, promoted, to);
            zobrist_hash ^= board.get_zobrist_piece(promoted, to);
            total_pieces--;
            break;
        default:
            board.move_piece(from, to);
            zobrist_hash ^= board.get_zobrist_piece(moved_piece, to);
            break;
    }

    // Reset halfmove clock counter
    half_moves++;
    if (moved_piece == WHITE_PAWN || moved_piece == BLACK_PAWN) {
        half_moves = 0;
    }

    // Reset castling rights if the king moves
    if (moved_piece == WHITE_KING)
        wk_castle = wq_castle = false;
    if (moved_piece == BLACK_KING)
        bk_castle = bq_castle = false;

    // Reset castling rights if rooks move from their original squares
    if (from == 0 && moved_piece == WHITE_ROOK)
        wq_castle = false;
    if (from == 7 && moved_piece == WHITE_ROOK)
        wk_castle = false;
    if (from == 56 && moved_piece == BLACK_ROOK)
        bq_castle = false;
    if (from == 63 && moved_piece == BLACK_ROOK)
        bk_castle = false;

    // Update castling rights in hash
    int new_castle = get_castling_mask(bk_castle, bq_castle, wk_castle, wq_castle);
    if (new_castle)
        zobrist_hash ^= board.get_zobrist_castle(new_castle);

    // Toggle side to move in hash and update full moves
    if (!white_turn) {
        zobrist_hash ^= board.get_zobrist_blacktomove();
        full_moves++;
    }

    undo_data.captured_piece_type = captured_piece_type;
    undo_stack.push_back(undo_data);

    change_turn();
    return true;
}

bool ChessLogic::unmake_move(u16 move) {
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

    // Restore previous zobrist hash
    zobrist_hash = undo_data.zobrist_hash;

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
        total_pieces++;
    } else if (flag == ep_capture) {
        assert(undo_data.captured_piece_type != -1);
        board.move_piece(to, from);
        int sq = undo_data.turn ? to - 8 : to + 8;
        board.add_piece(sq, undo_data.captured_piece_type);
        total_pieces++;
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
        total_pieces++;
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

    return true;
}

u64 ChessLogic::compute_zobrist_hash() {
    int *piece_on_square = board.get_piece_on_squares();
    u64 hash = 0;

    for (int sq = 0; sq < 64; sq++) {
        int type = piece_on_square[sq];
        if (type != -1)
            hash ^= board.get_zobrist_piece(type, sq);
    }

    if (!white_turn)
        hash ^= board.get_zobrist_blacktomove();

    int castling = get_castling_mask(bk_castle, bq_castle, wk_castle, wq_castle);
    if (castling != 0)
        hash ^= board.get_zobrist_castle(castling);

    if (en_passant_square != -1)
        hash ^= board.get_zobrist_ep(en_passant_square);

    return hash;
}

// TODO: Complete this when zobrist hashing is implemented
bool ChessLogic::fivefold_repitition() { return false; }

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
u64 ChessLogic::get_zobrist_hash() { return zobrist_hash; }
int *ChessLogic::get_total_pieces() { return &total_pieces; }