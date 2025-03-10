#ifndef UTILS_H
#define UTILS_H
#include <bitset>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#define STARTPOS "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w"

typedef enum {
    EMPTY,
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

typedef enum {
    NORTH,
    SOUTH,
    WEST,
    EAST,
    NORTH_WEST,
    NORTH_EAST,
    SOUTH_WEST,
    SOUTH_EAST
} Direction;

typedef uint64_t u64;
typedef uint16_t u16;
typedef uint8_t u8;

static const char symbols[] = {'P', 'R', 'N', 'B', 'Q', 'K',
                               'p', 'r', 'n', 'b', 'q', 'k'};

static const u64 clear_rank[] = {0xffffffffffffff00, 0xffffffffffff00ff,
                                 0xffffffffff00ffff, 0xffffffff00ffffff,
                                 0xffffff00ffffffff, 0xffff00ffffffffff,
                                 0xff00ffffffffffff, 0x00ffffffffffffff};
static const u64 mask_rank[] = {0x00000000000000ff, 0x000000000000ff00,
                                0x0000000000ff0000, 0x00000000ff000000,
                                0x000000ff00000000, 0x0000ff0000000000,
                                0x00ff000000000000, 0xff00000000000000};
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

static const u64 not_a_file = 0xfefefefefefefefe;
static const u64 not_h_file = 0x7f7f7f7f7f7f7f7f;
static const u64 not_ab_file = 0xfcfcfcfcfcfcfcfc;
static const u64 not_gh_file = 0x3f3f3f3f3f3f3f3f;

static const int deBruijn_lookup_table[] = {
    0,  1,  48, 2,  57, 49, 28, 3,  61, 58, 50, 42, 38, 29, 17, 4,
    62, 55, 59, 36, 53, 51, 43, 22, 45, 39, 33, 30, 24, 18, 12, 5,
    63, 47, 56, 27, 60, 41, 37, 16, 54, 35, 52, 21, 44, 32, 23, 11,
    46, 26, 40, 15, 34, 20, 31, 10, 25, 14, 19, 9,  13, 8,  7,  6};

int first_bit(u64);
char get_symbol(u64 *, int);
u64 shift_north(u64);
u64 shift_south(u64);
u64 shift_east(u64);
u64 shift_west(u64);
u64 shift_north_west(u64);
u64 shift_north_east(u64);
u64 shift_south_west(u64);
u64 shift_south_east(u64);
void print_bitboard(u64);

#endif // !UTILS_H
