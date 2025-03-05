#ifndef UTILS_H
#define UTILS_H

#include <string>

struct Move {
    int start_square;
    int end_square;
    bool white;
};
typedef struct Move Move;

bool in_bounds(int);
bool knight_in_bounds(int, int);
Move convert_pos(const std::string &);
void precompute_squares_to_edges(int (*)[8]);
int find_king_pos(unsigned char *, unsigned char);

#endif // !UTILS_H
