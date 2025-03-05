#ifndef GAME_H
#define GAME_H
#include "../include/board.h"

class Game {
private:
    Board board;
    std::vector<std::string> fens;

public:
    Game();
    void run_game();
    bool make_move(std::vector<Move> &, int, int);
    Board &get_board();
};

#endif // !GAME_H
