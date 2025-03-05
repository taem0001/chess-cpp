#ifndef UTILS_H
#define UTILS_H
#include <bitset>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <string>

#define STARTPOS "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"

typedef enum {
    ALL,
    WHITE,
    BLACK,
    WHITE_PAWN,
    WHITE_ROOK,
    WHITE_KNIGHT,
    WHITE_BISHOP,
    WHITE_QUEEN,
    WHITE_KING,
    BLACK_PAWN,
    BLACK_ROOK,
    BLACK_KNIGHT,
    BLACK_BISHOP,
    BLACK_QUEEN,
    BLACK_KING
} PieceType;

typedef uint64_t u64;
typedef uint8_t u8;

static const char symbols[] = {'P', 'R', 'N', 'B', 'Q', 'K', 'p', 'r', 'n', 'b', 'q', 'k'};

int first_bit(u64);
int last_bit(u64);
char get_symbol(u64 *, int);

#endif // !UTILS_H
