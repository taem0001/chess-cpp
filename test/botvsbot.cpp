#include "../src/include/chessgame.h"

int main(void) {
    ChessGame game;
    Bot bot1, bot2;

    game.run_game_bot(bot1, bot2);

    if (game.get_draw()) {
        std::cout << "DRAW" << "\n";
    } else {
        if (game.get_winner()) {
            std::cout << "WHITE WINS" << "\n";
        } else {
            std::cout << "BLACK WINS" << "\n";
        }
    }

    return 0;
}