#pragma once

class Board {
  private:
    unsigned char board[64];

  public:
    Board();
    void load_start_pos(unsigned char *);
    void load_pos(const char *, unsigned char *);
    void draw_board();
};
