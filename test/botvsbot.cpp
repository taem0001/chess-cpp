#include "../include/chessgame.h"

int main(void) {
    ChessGame game;
    Bot bot1, bot2;

    game.run_game_bot(bot1, bot2);

    if (game.get_draw()) {
        std::cout << "DRAW" << std::endl;
    } else {
        if (game.get_winner()) {
            std::cout << "WHITE WINS" << std::endl;
        } else {
            std::cout << "BLACK WINS" << std::endl;
        }
    }

    return 0;
}