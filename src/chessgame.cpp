#include "../include/chessgame.h"
#include "../include/fen.h"
#include "../include/movegen.h"

ChessGame::ChessGame() : board() {
    FenHandler::load_fen(*this, STARTPOS);
    MoveGenerator::init();
}

void ChessGame::draw_game() {
    std::vector<u16> moves = MoveGenerator::generate_pseudolegal_moves(*this);

    print_moves(moves);

    board.draw_board();
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
