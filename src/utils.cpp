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
    for (int i = 3; i < 15; i++) {
        if (bitboards[i] & (1ULL << square)) {
            return symbols[i - 3];
        }
    }
    return '.';
}
