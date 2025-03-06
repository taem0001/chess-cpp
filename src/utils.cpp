#include "../include/utils.h"

int first_bit(u64 bitboard) {
    if (bitboard == 0) {
        return -1;
    }
    return __builtin_ctz(bitboard);
}

int last_bit(u64 bitboard) {
    if (bitboard == 0) {
        return -1;
    }
    return log2(bitboard);
}

char get_symbol(u64 *bitboards, int square) {
    for (int i = 4; i < 16; i++) {
        if (bitboards[i] & (1ULL << square)) {
            return symbols[i - 4];
        }
    }
    return '.';
}

u64 shift_north(u64 b) { return b << 8; }
u64 shift_south(u64 b) { return b >> 8; }
u64 shift_east(u64 b) { return (b << 1) & not_a_file; }
u64 shift_west(u64 b) { return (b >> 1) & not_h_file; }
u64 shift_north_east(u64 b) { return (b << 9) & not_a_file; }
u64 shift_north_west(u64 b) { return (b << 7) & not_h_file; }
u64 shift_south_east(u64 b) { return (b >> 7) & not_h_file; }
u64 shift_south_west(u64 b) { return (b >> 9) & not_a_file; }

void print_bitboard(u64 bitboard) {
    for (int rank = 7; rank >= 0; --rank) {
        for (int file = 0; file < 8; ++file) {
            int square = rank * 8 + file;
            std::cout << ((bitboard >> square) & 1) << " ";
        }
        std::cout << std::endl;;
    }
    std::cout << std::endl;
}
