#include "../include/chessgame.h"
#include "../include/fen.h"
#include "../include/movegen.h"

ChessGame::ChessGame() : board() {
    FenHandler::load_fen(*this, STARTPOS);
    MoveGenerator::init();
}

void ChessGame::draw_game() {
    print_bitboard(MoveGenerator::generate_moves(*this));

    board.draw_board();
}

Board &ChessGame::get_board() { return board; }

void ChessGame::change_turn() { white_turn = !white_turn; }

void ChessGame::set_turn(bool b) { white_turn = b; }

bool ChessGame::get_turn() { return white_turn; }
