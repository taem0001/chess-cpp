#ifndef UTILS_H
#define UTILS_H
#include <bitset>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#define STARTPOS "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

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

static const u64 not_ab_file = 0xfcfcfcfcfcfcfcfc;
static const u64 not_gh_file = 0x3f3f3f3f3f3f3f3f;
static const u64 wk_castle_mask = 0x60;
static const u64 wq_castle_mask = 0xe;
static const u64 bk_castle_mask = wk_castle_mask << 56;
static const u64 bq_castle_mask = wq_castle_mask << 56;

static const u16 quiet_move = 0x0;
static const u16 double_pawn_push = 0x1;
static const u16 king_castle = 0x2;
static const u16 queen_castle = 0x3;
static const u16 capture = 0x4;
static const u16 ep_capture = 0x5;
static const u16 knight_promotion = 0x8;
static const u16 bishop_promotion = 0x9;
static const u16 rook_promotion = 0xa;
static const u16 queen_promotion = 0xb;
static const u16 knight_promo_capture = 0xc;
static const u16 bishop_promo_capture = 0xd;
static const u16 rook_promo_capture = 0xe;
static const u16 queen_promo_capture = 0xf;

static const int deBruijn_lookup_table[] = {
    0,  1,  48, 2,  57, 49, 28, 3,  61, 58, 50, 42, 38, 29, 17, 4,
    62, 55, 59, 36, 53, 51, 43, 22, 45, 39, 33, 30, 24, 18, 12, 5,
    63, 47, 56, 27, 60, 41, 37, 16, 54, 35, 52, 21, 44, 32, 23, 11,
    46, 26, 40, 15, 34, 20, 31, 10, 25, 14, 19, 9,  13, 8,  7,  6};

static const u64 knight_attack_squares[] = {
    0x0000000000020400, 0x0000000000050800, 0x00000000000A1100,
    0x0000000000142200, 0x0000000000284400, 0x0000000000508800,
    0x0000000000A01000, 0x0000000000402000, 0x0000000002040004,
    0x0000000005080008, 0x000000000A110011, 0x0000000014220022,
    0x0000000028440044, 0x0000000050880088, 0x00000000A0100010,
    0x0000000040200020, 0x0000000204000402, 0x0000000508000805,
    0x0000000A1100110A, 0x0000001422002214, 0x0000002844004428,
    0x0000005088008850, 0x000000A0100010A0, 0x0000004020002040,
    0x0000020400040200, 0x0000050800080500, 0x00000A1100110A00,
    0x0000142200221400, 0x0000284400442800, 0x0000508800885000,
    0x0000A0100010A000, 0x0000402000204000, 0x0002040004020000,
    0x0005080008050000, 0x000A1100110A0000, 0x0014220022140000,
    0x0028440044280000, 0x0050880088500000, 0x00A0100010A00000,
    0x0040200020400000, 0x0204000402000000, 0x0508000805000000,
    0x0A1100110A000000, 0x1422002214000000, 0x2844004428000000,
    0x5088008850000000, 0xA0100010A0000000, 0x4020002040000000,
    0x0400040200000000, 0x0800080500000000, 0x1100110A00000000,
    0x2200221400000000, 0x4400442800000000, 0x8800885000000000,
    0x100010A000000000, 0x2000204000000000, 0x0004020000000000,
    0x0008050000000000, 0x00110A0000000000, 0x0022140000000000,
    0x0044280000000000, 0x0088500000000000, 0x0010A00000000000,
    0x0020400000000000};

int first_bit(u64);
int last_bit(u64);
bool contains_move(std::vector<u16> &, int, int, u16 *);
bool get_pos(std::string &, int *, int *);
char get_symbol(u64 *, int);
u64 shift_north(u64);
u64 shift_south(u64);
u64 shift_east(u64);
u64 shift_west(u64);
u64 shift_north_west(u64);
u64 shift_north_east(u64);
u64 shift_south_west(u64);
u64 shift_south_east(u64);
u16 define_move(u16, u16, u16);
u16 get_from(u16);
u16 get_to(u16);
u16 get_flag(u16);
void print_bitboard(u64);
void print_moves(std::vector<u16> &);
std::string print_pos(int);

#endif // !UTILS_H
