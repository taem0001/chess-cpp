#pragma once
#include <string>
#include <vector>

struct Move {
    int start_square;
    int end_square;
};
typedef struct Move Move;

class Board {
  private:
    unsigned char board[64];
    bool white_to_move;
    constexpr static const int dir[] = {8, -8, -1, 1, 7, -7, 9, -9};
    static int squares_to_edges[64][8];

    static void precompute_squares_to_edges();
    void generate_sliding_moves(int, std::vector<Move> &);

  public:
    Board();
    void load_pos(const char *, unsigned char *);
    std::string write_fen();
    void draw_board();
    std::vector<Move> generate_legal_moves();
    unsigned char *get_board();
};
