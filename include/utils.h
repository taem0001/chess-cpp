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

static const char symbols[] = {'P', 'R', 'N', 'B', 'Q', 'K',
                               'p', 'r', 'n', 'b', 'q', 'k'};

static const u64 clear_rank[] = {0x00ffffffffffffff, 0xff00ffffffffffff,
                                 0xffff00ffffffffff, 0xffffff00ffffffff,
                                 0xffffffff00ffffff, 0xffffffffff00ffff,
                                 0xffffffffffff00ff, 0xffffffffffffff00};
static const u64 mask_rank[] = {0xff00000000000000, 0x00ff000000000000,
                                0x0000ff0000000000, 0x000000ff00000000,
                                0x00000000ff000000, 0x0000000000ff0000,
                                0x000000000000ff00, 0x00000000000000ff};
static const u64 clear_file[] = {0xfefefefefefefefe, 0xfdfdfdfdfdfdfdfd,
                                 0xfbfbfbfbfbfbfbfb, 0xf7f7f7f7f7f7f7f7,
                                 0xefefefefefefefef, 0xdfdfdfdfdfdfdfdf,
                                 0xbfbfbfbfbfbfbfbf, 0x7f7f7f7f7f7f7f7f};
static const u64 mask_file[] = {0x0101010101010101, 0x0202020202020202,
                                0x0404040404040404, 0x0808080808080808,
                                0x1010101010101010, 0x2020202020202020,
                                0x4040404040404040, 0x8080808080808080};
static const u64 mask_piece[] = {0x1,
                                 0x2,
                                 0x4,
                                 0x8,
                                 0x10,
                                 0x20,
                                 0x40,
                                 0x80,
                                 0x100,
                                 0x200,
                                 0x400,
                                 0x800,
                                 0x1000,
                                 0x2000,
                                 0x4000,
                                 0x8000,
                                 0x10000,
                                 0x20000,
                                 0x40000,
                                 0x80000,
                                 0x100000,
                                 0x200000,
                                 0x400000,
                                 0x800000,
                                 0x1000000,
                                 0x2000000,
                                 0x4000000,
                                 0x8000000,
                                 0x10000000,
                                 0x20000000,
                                 0x40000000,
                                 0x80000000,
                                 0x100000000,
                                 0x200000000,
                                 0x400000000,
                                 0x800000000,
                                 0x1000000000,
                                 0x2000000000,
                                 0x4000000000,
                                 0x8000000000,
                                 0x10000000000,
                                 0x20000000000,
                                 0x40000000000,
                                 0x80000000000,
                                 0x100000000000,
                                 0x200000000000,
                                 0x400000000000,
                                 0x800000000000,
                                 0x1000000000000,
                                 0x2000000000000,
                                 0x4000000000000,
                                 0x8000000000000,
                                 0x10000000000000,
                                 0x20000000000000,
                                 0x40000000000000,
                                 0x80000000000000,
                                 0x100000000000000,
                                 0x200000000000000,
                                 0x400000000000000,
                                 0x800000000000000,
                                 0x1000000000000000,
                                 0x2000000000000000,
                                 0x4000000000000000,
                                 0x8000000000000000};

int first_bit(u64);
int last_bit(u64);
char get_symbol(u64 *, int);

#endif // !UTILS_H
