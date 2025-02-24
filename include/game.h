#ifndef GAME_H
#define GAME_H
#include "../include/board.h"

class Game {
  private:
    Board *board;
    std::vector<std::string> fens;

  public:
    Game();
    ~Game();
    void run_game();
    void add_fen();
    bool make_move(std::vector<Move> &, int, int);
    bool unmake_move(Move &);
    Board *get_board();
};

#endif // !GAME_H
