#include "../include/board.h"
#include <iostream>
#include <ostream>

int main() {
    Board board;
    board.draw_board();
    std::cout << board.write_fen() << std::endl;
    return 0;
}
