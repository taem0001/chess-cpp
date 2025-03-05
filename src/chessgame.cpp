#include "../include/chessgame.h"

ChessGame::ChessGame() : board() {
    board.init();
}

void ChessGame::draw_game() {
    u64 *bitboards = board.get_bitboards();
    int square;

    std::cout << "  +-----------------+" << std::endl;
    for (int rank = 0; rank < 8; rank++) {
        std::cout << 8 - rank << " | ";
        for (int file = 0; file < 8; file++) {
            square = rank * 8 + file;
            std::cout << get_symbol(bitboards, square) << " ";
        }
        std::cout << "|" << std::endl;
    }
    std::cout << "  +-----------------+" << std::endl;
    std::cout << "    a b c d e f g h" << std::endl;
}
