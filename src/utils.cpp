#include "../include/utils.h"

int first_bit(u64 bitboard) {
    if (bitboard == 0) {
        return 64;
    }
    u64 isolated_bit = bitboard & -bitboard;
    u64 debruijn_index = (isolated_bit * 0x03f79d71b4cb0a89ULL) >> 58;
    return deBruijn_lookup_table[debruijn_index];
}

int last_bit(u64 bitboard) {
    if (bitboard == 0) {
        return 64;
    }
    return 63 - __builtin_clzll(bitboard);
}

bool contains_move(std::vector<u16> &moves, int from, int to, u16 *move) {
    for (u16 move_t : moves) {
        int from_t = get_from(move_t);
        int to_t = get_to(move_t);
        if (from == from_t && to == to_t) {
            u16 flag = get_flag(move_t);
            *move = define_move(from, to, flag);
            return true;
        }
    }
    return false;
}

bool get_pos(std::string &pos, int *from, int *to) {
    if (pos.length() != 4) {
        return false;
    }
    if (pos[0] < 'a' || pos[0] > 'h') {
        return false;
    }
    if (pos[2] < 'a' || pos[2] > 'h') {
        return false;
    }
    if (pos[1] < '1' || pos[1] > '8') {
        return false;
    }
    if (pos[3] < '1' || pos[3] > '8') {
        return false;
    }

    int file = pos[0] - 'a';
    int rank = pos[1] - '1';
    *from = rank * 8 + file;
    file = pos[2] - 'a';
    rank = pos[3] - '1';
    *to = rank * 8 + file;
    return true;
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
u64 shift_east(u64 b) { return (b << 1ULL) & clear_file[0]; }
u64 shift_north_east(u64 b) { return (b << 9ULL) & clear_file[0]; }
u64 shift_south_east(u64 b) { return (b >> 7ULL) & clear_file[0]; }
u64 shift_west(u64 b) { return (b >> 1ULL) & clear_file[7]; }
u64 shift_south_west(u64 b) { return (b >> 9ULL) & clear_file[7]; }
u64 shift_north_west(u64 b) { return (b << 7ULL) & clear_file[7]; }

u16 define_move(u16 from, u16 to, u16 flag) {
    return ((flag & 0xf) << 12) | ((to & 0x3f) << 6) | (from & 0x3f);
}

u16 get_from(u16 move) { return move & 0x3f; }
u16 get_to(u16 move) { return (move >> 6) & 0x3f; }
u16 get_flag(u16 move) { return (move >> 12) & 0xf; }

void print_bitboard(u64 bitboard) {
    for (int rank = 7; rank >= 0; --rank) {
        for (int file = 0; file < 8; ++file) {
            int square = rank * 8 + file;
            std::cout << ((bitboard >> square) & 1) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void print_moves(std::vector<u16> &moves) {
    for (u16 move : moves) {
        std::cout << "{" << print_pos((int)get_from(move))
                  << print_pos((int)get_to(move)) << ";" << get_flag(move)
                  << "} ";
    }
    std::cout << std::endl;
}

std::string print_pos(int pos) {
    std::string res = "";
    int rank = pos / 8;
    int file = pos % 8;

    res += 'a' + file;
    res += '1' + rank;
    return res;
}
