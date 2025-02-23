#ifndef GAME_H
#define GAME_H
#include "../include/board.h"

class Game {
  private:
    Board *board;
    bool make_move(std::vector<Move> &, int, int);

  public:
    Game();
    ~Game();
    void run_game();
};

#endif // !GAME_H
