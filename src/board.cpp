#include "../include/board.h"

u64 *Board::get_bitboards() { return bitboards; }

void Board::draw_board() {
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
