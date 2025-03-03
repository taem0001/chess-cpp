#ifndef BOARD_H
#define BOARD_H

#include "utils.h"
#include <string>
#include <vector>

class Board {
  private:
    unsigned char board[64];
    int squares_to_edges[64][8];
    bool white_to_move;
    bool black_castle_queen, black_castle_king, white_castle_queen, white_castle_king;
    void precompute_squares_to_edges(int (*)[8]);

  public:
    void setup_board();
    void draw_board();
    unsigned char *get_board();
    bool is_move_legal(std::vector<Move> &, int, int);
    void move_piece(int, int);
    void change_turn();
    int (*get_precomputed_distances())[8];
    void set_board_element(unsigned char, int);
    void set_turn(bool);
    void set_black_castle_queen(bool);
    void set_black_castle_king(bool);
    void set_white_castle_queen(bool);
    void set_white_castle_king(bool);
    bool get_turn();
    bool get_black_castle_queen();
    bool get_black_castle_king();
    bool get_white_castle_queen();
    bool get_white_castle_king();
};

#endif // !BOARD_H
