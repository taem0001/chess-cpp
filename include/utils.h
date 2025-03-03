#ifndef UTILS_H
#define UTILS_H

#include <string>

struct Move {
    int start_square;
    int end_square;
};
typedef struct Move Move;

bool in_bounds(int);
bool knight_in_bounds(int, int);
Move convert_pos(const std::string &);

#endif // !UTILS_H
