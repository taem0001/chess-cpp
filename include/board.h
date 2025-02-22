#pragma once
#include <string>
#include <vector>

struct Move {
    unsigned char start_square;
    unsigned char end_square;
};
typedef struct Move Move;

class Board {
  private:
    unsigned char board[64];
    std::vector<Move> moves;
    bool white_to_move;

  public:
    Board();
    void load_pos(const char *, unsigned char *);
    std::string write_fen();
    void draw_board();
    int generate_legal_moves();
    unsigned char *get_board();
    std::vector<Move> get_move_list();
};
