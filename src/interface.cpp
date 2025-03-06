#include "../include/interface.h"

Interface::Interface() : game() {}

void Interface::run_game() {
    bool turn;
    std::string player, pos;

    game.draw_game();

    while (true) {
        turn = game.get_turn();
        player = turn ? "White player's move: " : "Black player's move: ";
        std::cout << player;

        do {
            std::cin >> pos;
        } while (false);

        game.draw_game();
    }
}
