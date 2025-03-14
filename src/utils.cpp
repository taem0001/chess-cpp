#include "../include/utils.h"

int first_bit(u64 bitboard) {
    if (bitboard == 0) {
        return -1;
    }
    u64 isolated_bit = bitboard & -bitboard;
    u64 debruijn_index = (isolated_bit * 0x03f79d71b4cb0a89ULL) >> 58;
    return deBruijn_lookup_table[debruijn_index];
}

int last_bit(u64 bitboard) {
    if (bitboard == 0) {
        return -1;
    }
    return 63 - __builtin_clzll(bitboard);
}

char get_symbol(u64 *bitboards, int square) {
    for (int i = 3; i < 15; i++) {
        if (bitboards[i] & (1ULL << square)) {
            return symbols[i - 3];
        }
    }
    return '.';
}

u64 shift_north(u64 b) { return b << 8ULL; }
u64 shift_south(u64 b) { return b >> 8ULL; }
u64 shift_east(u64 b) { return (b << 1ULL) & not_a_file; }
u64 shift_west(u64 b) { return (b >> 1ULL) & not_h_file; }
u64 shift_north_east(u64 b) { return (b << 9ULL) & not_a_file; }
u64 shift_north_west(u64 b) { return (b << 7ULL) & not_h_file; }
u64 shift_south_east(u64 b) { return (b >> 7ULL) & not_h_file; }
u64 shift_south_west(u64 b) { return (b >> 9ULL) & not_a_file; }

void print_bitboard(u64 bitboard) {
    for (int rank = 7; rank >= 0; --rank) {
        for (int file = 0; file < 8; ++file) {
            int square = rank * 8 + file;
            std::cout << ((bitboard >> square) & 1) << " ";
        }
        std::cout << std::endl;
        ;
    }
    std::cout << std::endl;
}
