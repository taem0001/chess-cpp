#ifndef CHESSGAME_H
#define CHESSGAME_H
#include "../include/chesslogic.h"
#include "../include/movegen.h"
#include "../include/fen.h"
#include "../include/bot.h"

class ChessGame {
private:
    ChessLogic logic;
    bool running;
    bool draw;
    bool winner;

public:
    ChessGame();
    void run_game(Bot &, bool);
    void run_game_bot(Bot &, Bot &);
    bool get_draw();
    bool get_winner();
};

#endif // !CHESSGAME_H
