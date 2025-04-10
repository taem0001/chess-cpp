#include "../include/chessgame.h"

void game(void) {
    ChessGame game;
    game.run_game();
    if (game.get_draw()) {
        std::cout << "DRAW" << std::endl;
    } else {
        if (game.get_winner()) {
            std::cout << "WHITE WINS" << std::endl;
        } else {
            std::cout << "BLACK WINS" << std::endl;
        }
    }
}

int main(void) {
    game();
    return 0;
}
