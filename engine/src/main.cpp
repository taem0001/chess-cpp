#include "../include/chessgame.h"
#include "../include/bot.h"

void game(void) {
    ChessGame game;
    Bot bot;
    char bot_color;
    do {
        std::cout << "What color should the bot play(w/b): ";
        std::cin >> bot_color; 
    } while (bot_color != 'w' && bot_color != 'W' && bot_color != 'b' && bot_color != 'B');

    if (bot_color == 'w' || bot_color == 'W') {
        game.run_game(bot, true);
    } else {
        game.run_game(bot, false);
    }
    
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
