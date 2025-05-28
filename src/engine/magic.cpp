#include "../include/magic.h"

const int BitTable[64] = {63, 30, 3,  32, 25, 41, 22, 33, 15, 50, 42, 13, 11, 53, 19, 34, 61, 29, 2,  51, 21, 43,
                          45, 10, 18, 47, 1,  54, 9,  57, 0,  35, 62, 31, 40, 4,  49, 5,  52, 26, 60, 6,  23, 44,
                          46, 27, 56, 16, 7,  39, 48, 24, 59, 14, 12, 55, 38, 28, 58, 20, 37, 17, 36, 8};

u64 random_u64() {
    static std::mt19937_64 rng(std::random_device{}());
    static std::uniform_int_distribution<u64> dist(0, UINT64_MAX);
    return dist(rng);
}

u64 random_u64_fewbits() { return random_u64() & random_u64() & random_u64(); }

int count_1s(u64 b) { return __popcnt64(b); }

int pop_1st_bit(u64 *bb) {
    u64 b = *bb ^ (*bb - 1);
    unsigned int fold = (unsigned)((b & 0xffffffff) ^ (b >> 32));
    *bb &= (*bb - 1);
    return BitTable[(fold * 0x783a9b23) >> 26];
}

u64 index_to_u64(int index, int bits, u64 m) {
    int i, j;
    u64 result = 0ULL;
    for (i = 0; i < bits; i++) {
        j = pop_1st_bit(&m);
        if (index & mask_piece[i]) {
            result |= mask_piece[j];
        }
    }
    return result;
}

int transform(u64 b, u64 magic, int bits) { return (int)((b * magic) >> (64 - bits)); }
