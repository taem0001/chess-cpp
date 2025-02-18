#ifndef UTILS_H
#define UTILS_H

struct Position {
    unsigned int row;
    unsigned int col;
};
typedef struct Position Position;

enum class PieceColor {
    WHITE,
    BLACK
};
typedef enum PieceColor PieceColor;

int _abs(int);

#endif // !UTILS_H
