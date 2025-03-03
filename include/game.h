#ifndef GAME_H
#define GAME_H
#include "../include/board.h"

class Game {
  private:
    const std::string start_pos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq";
    Board board;
    std::vector<std::string> fens;

  public:
    Game();
    void run_game();
    bool make_move(std::vector<Move> &, int, int);
    Board &get_board();
};

#endif // !GAME_H
