#ifndef BOARD_H
#define BOARD_H

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
    int squares_to_edges[64][8];

    void precompute_squares_to_edges(int (*)[8]);
    void generate_sliding_moves(int, std::vector<Move> &);
    void generate_knight_moves(int, std::vector<Move> &);
    void generate_pawn_moves(int, std::vector<Move> &);
    void generate_king_moves(int, std::vector<Move> &);

  public:
    Board();
    void load_pos(const char *, unsigned char *);
    std::string write_fen();
    void draw_board();
    std::vector<Move> generate_legal_moves();
    unsigned char *get_board();
    bool is_move_legal(std::vector<Move> &, int, int);
    void move_piece(int, int);
    void change_turn();
    bool get_turn();
};

#endif // !BOARD_H
