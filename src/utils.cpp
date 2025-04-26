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

#if defined(_MSC_VER)  // For MSVC (Windows)
    unsigned long index;
    _BitScanReverse64(&index, bitboard);
    return static_cast<int>(index);
#else  // For GCC/Clang (Linux/Unix)
    return 63 - __builtin_clzll(bitboard);
#endif
}

int max(int n1, int n2) {
    if (n1 > n2) {
        return n1;
    }
    return n2;
}

int get_piece_score(u64 *bitboards, int square) {
    int type;
    int score;

    for (int i = 3; i < 15; i++) {
        if (mask_piece[square] & bitboards[i]) {
            type = i;
        }
    }

    switch (type) {
    case WHITE_PAWN:
        score = pawn_value;
        break;
    case WHITE_KNIGHT:
        score = knight_value;
        break;
    case WHITE_BISHOP:
        score = bishop_value;
        break;
    case WHITE_ROOK:
        score = rook_value;
        break;
    case WHITE_QUEEN:
        score = queen_value;
        break;
    case BLACK_PAWN:
        score = pawn_value;
        break;
    case BLACK_KNIGHT:
        score = knight_value;
        break;
    case BLACK_BISHOP:
        score = bishop_value;
        break;
    case BLACK_ROOK:
        score = rook_value;
        break;
    case BLACK_QUEEN:
        score = queen_value;
        break;
    default:
        score = 0;
        break;
    }

    return score;
}

bool contains_move(std::vector<u64> &moves, int from, int to, u64 total_moves, u64 *move) {
    for (u64 move_t : moves) {
        int from_t = (int)get_from(move_t);
        int to_t = (int)get_to(move_t);
        if (from == from_t && to == to_t) {
            u64 flag = get_flag(move_t);
            *move = define_move((u64)from, (u64)to, flag, total_moves);
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
    return ' ';
}

u64 shift_north(u64 b) { return b << 8ULL; }
u64 shift_south(u64 b) { return b >> 8ULL; }
u64 shift_east(u64 b) { return (b << 1ULL) & clear_file[0]; }
u64 shift_north_east(u64 b) { return (b << 9ULL) & clear_file[0]; }
u64 shift_south_east(u64 b) { return (b >> 7ULL) & clear_file[0]; }
u64 shift_west(u64 b) { return (b >> 1ULL) & clear_file[7]; }
u64 shift_south_west(u64 b) { return (b >> 9ULL) & clear_file[7]; }
u64 shift_north_west(u64 b) { return (b << 7ULL) & clear_file[7]; }

u64 in_between(int sq1, int sq2) {
    const u64 m1 = u64(-1);
    const u64 a2a7 = 0x0001010101010100;
    const u64 b2g7 = 0x0040201008040200;
    const u64 h1b7 = 0x0002040810204080;
    u64 btwn, line, rank, file;

    btwn = (m1 << sq1) ^ (m1 << sq2);
    file = (sq2 & 7) - (sq1 & 7);
    rank = ((sq2 | 7) - sq1) >> 3;
    line = ((file & 7) - 1) & a2a7;
    line += 2 * (((rank & 7) - 1) >> 58);
    line += (((rank - file) & 15) - 1) & b2g7;
    line += (((rank + file) & 15) - 1) & h1b7;
    line *= btwn & -btwn;
    return line & btwn;
}

u64 define_move(u64 from, u64 to, u64 flag, u64 total_moves) {
    return ((total_moves & 0xffffffffffff) << 16) | ((flag & 0xf) << 12) | ((to & 0x3f) << 6) | (from & 0x3f);
}

u64 get_from(u64 move) { return move & 0x3f; }
u64 get_to(u64 move) { return (move >> 6) & 0x3f; }
u64 get_flag(u64 move) { return (move >> 12) & 0xf; }
u64 get_totalmoves(u64 move) { return (move >> 16) & 0xffffffffffff; }

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

void print_moves(std::vector<u64> &moves) {
    for (u64 move : moves) {
        std::cout << print_pos((int)get_from(move)) << print_pos((int)get_to(move)) << std::endl;
    }
    std::cout << std::endl;
}

void merge(std::vector<MoveScore> &vec, int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Temporary vectors
    std::vector<MoveScore> left_vec(n1), right_vec(n2);

    // Copy data
    for (int i = 0; i < n1; i++) {
        left_vec[i] = vec[left + i];
    }
    for (int i = 0; i < n2; i++) {
        right_vec[i] = vec[mid + 1 + i];
    }

    // Merge temporary vectors back
    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2) {
        if (left_vec[i].score >= right_vec[j].score) {
            vec[k++] = left_vec[i++];
        } else {
            vec[k++] = right_vec[j++];
        }
    }

    // Copy remaining elements
    while (i < n1) {
        vec[k++] = left_vec[i++];
    }
    while (j < n2) {
        vec[k++] = right_vec[j++];
    }
}

void merge_sort(std::vector<MoveScore> &vec, int left, int right) {
    if (left < right) {
        // Calculate mid point
        int mid = left + (right - left) / 2;

        // Sort halves
        merge_sort(vec, left, mid);
        merge_sort(vec, mid + 1, right);

        // Merge sorted halves
        merge(vec, left, mid, right);
    }
}

void moves_only_captures(std::vector<u64> &moves) {
    for (int i = 0; i < moves.size(); i++) {
        u64 move = moves[i];
        int flag = get_flag(move);

        if (flag != capture && flag != ep_capture) {
            moves.erase(moves.begin() + i);
        }
    }
}

std::string print_pos(int pos) {
    std::string res = "";
    int rank = pos / 8;
    int file = pos % 8;

    res += 'a' + file;
    res += '1' + rank;
    return res;
}
