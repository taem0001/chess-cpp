#include "../include/chessgame.h"
#include "../include/fen.h"
#include "../include/movegen.h"

ChessGame::ChessGame() : board() {
    FenHandler::load_fen(*this, STARTPOS);
    MoveGenerator::init();
}

void ChessGame::draw_game() {
    u64 *bitboards = board.get_bitboards();
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

Board &ChessGame::get_board() { return board; }

void ChessGame::change_turn() { white_turn = !white_turn; }

void ChessGame::set_turn(bool b) { white_turn = b; }

bool ChessGame::get_turn() { return white_turn; }
