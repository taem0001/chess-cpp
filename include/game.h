#ifndef GAME_H
#define GAME_H
#include "chesslogic.h"

class Game {
  private:
    ChessLogic game_logic;

  public:
    void chess_game();
};

#endif // !GAME_H
