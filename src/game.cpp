#include "../include/game.h"
#include <iostream>

void Game::chess_game() {
    unsigned int row, col;
    Position start, end;
    char stop;

    game_logic.get_board()->draw_board();

    while (true) {
        std::cout << "Enter move: " << std::endl;
        std::cin >> row >> col;
        start = {row, col};
        std::cin >> row >> col;
        end = {row, col};

        game_logic.make_move(start, end);
        game_logic.get_board()->draw_board();
    }

    std::cout << "Game finished" << std::endl;
}
