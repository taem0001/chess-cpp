#include "../include/utils.h"

/// @brief Returns position of first set bit in 64-bit integer.
/// @param bitboard
/// @return
int first_bit(u64 bitboard) {
    if (bitboard == 0) {
        return 64;
    }
    u64 isolated_bit = bitboard & -bitboard;
    u64 debruijn_index = (isolated_bit * 0x03f79d71b4cb0a89ULL) >> 58;
    return deBruijn_lookup_table[debruijn_index];
}

/// @brief Returns position of last set bit in 64-bit integer.
/// @param bitboard
/// @return
int last_bit(u64 bitboard) {
    if (bitboard == 0) {
        return 64;
    }
    unsigned long index;
    _BitScanReverse64(&index, bitboard);
    return static_cast<int>(index);
}

/// @brief Returns the maximal value between two integers.
/// @param n1
/// @param n2
/// @return
int max(int n1, int n2) {
    if (n1 > n2) {
        return n1;
    }
    return n2;
}

/// @brief Returns the evaluation score of whatever is on the square.
/// @param bitboards
/// @param square
/// @return
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

/// @brief Computes the combination of castling opportunities.
/// @param bk Black king-side castle
/// @param bq Black queen-side castle
/// @param wk White king-side castle
/// @param wq White queen-side castle
/// @return An integer with bits set for the sides that both kings can castle.
int get_castling_mask(bool bk, bool bq, bool wk, bool wq) {
    return (int)(bk << 0) | (int)(bq << 1) | (int)(wk << 2) | (int)(wq << 3);
}

int get_promoted_piece_index(int flag, bool turn) {
    int result = -1;

    switch (flag) {
        case knight_promotion:
        case knight_promo_capture:
            result = turn ? WHITE_KNIGHT : BLACK_KNIGHT;
            break;
        case bishop_promotion:
        case bishop_promo_capture:
            result = turn ? WHITE_BISHOP : BLACK_BISHOP;
            break;
        case rook_promotion:
        case rook_promo_capture:
            result = turn ? WHITE_ROOK : BLACK_ROOK;
            break;
        case queen_promotion:
        case queen_promo_capture:
            result = turn ? WHITE_QUEEN : BLACK_QUEEN;
            break;
        default:
            break;
    }
    assert(result != -1);
    return result;
}

/// @brief Checks if the move is in the list of moves.
/// @brief If the move is in the list, then the move is assigned.
/// @param moves
/// @param from
/// @param to
/// @param move
/// @return
bool contains_move(std::vector<u16> &moves, int from, int to, u16 *move) {
    assert(from >= 0 && from < 64);
    assert(to >= 0 && to < 64);

    for (u16 move_t : moves) {
        int from_t = (int)get_from(move_t);
        int to_t = (int)get_to(move_t);
        if (from == from_t && to == to_t) {
            int flag = (int)get_flag(move_t);
            *move = define_move(from, to, flag);
            return true;
        }
    }
    return false;
}

/// @brief Converts the move string e.g. "e2e4" to the corresponding row and file for both
/// @param pos
/// @param from
/// @param to
/// @return
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

    assert(*from >= 0 && *from < 64);
    assert(*to >= 0 && *to < 64);

    return true;
}

/// @brief Gets the symbol for the piece on the square.
/// @param bitboards
/// @param square
/// @return
char get_symbol(u64 *bitboards, int square) {
    for (int i = 3; i < 15; i++) {
        if (bitboards[i] & (1ULL << square)) {
            return symbols[i - 3];
        }
    }
    return ' ';
}

/// @brief Gets the symbol for the piece which the pawn is being promoted to.
/// @param flag 
/// @return 
char get_promotion_symbol(int flag) {
    char result;
    switch (flag)
    {
    case knight_promotion:
    case knight_promo_capture:
        result = 'n';
        break;
    case bishop_promotion:
    case bishop_promo_capture:
        result = 'b';
        break;
    case rook_promotion:
    case rook_promo_capture:
        result = 'r';
        break;
    case queen_promotion:
    case queen_promo_capture:
        result = 'q';
        break;
    default:
        result = '\0';
        break;
    }

    return result;
}

/// @brief Shifts the bitboard north.
/// @param b
/// @return
u64 shift_north(u64 b) { return b << 8ULL; }

/// @brief Shifts the bitboard south.
/// @param b
/// @return
u64 shift_south(u64 b) { return b >> 8ULL; }

/// @brief Shifts the bitboard east.
/// @param b
/// @return
u64 shift_east(u64 b) { return (b << 1ULL) & clear_file[0]; }

/// @brief Shifts the bitboard north east.
/// @param b
/// @return
u64 shift_north_east(u64 b) { return (b << 9ULL) & clear_file[0]; }

/// @brief Shifts the bitboard south east.
/// @param b
/// @return
u64 shift_south_east(u64 b) { return (b >> 7ULL) & clear_file[0]; }

/// @brief Shifts the bitboard west.
/// @param b
/// @return
u64 shift_west(u64 b) { return (b >> 1ULL) & clear_file[7]; }

/// @brief Shifts the bitboard south west.
/// @param b
/// @return
u64 shift_south_west(u64 b) { return (b >> 9ULL) & clear_file[7]; }

/// @brief Shifts the bitboard north west.
/// @param b
/// @return
u64 shift_north_west(u64 b) { return (b << 7ULL) & clear_file[7]; }

/// @brief Computes the bitboard with straight or diagonal squares between the two squares.
/// @param sq1
/// @param sq2
/// @return
u64 in_between(int sq1, int sq2) {
    assert(sq1 >= 0 && sq1 < 64);
    assert(sq2 >= 0 && sq2 < 64);

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

/// @brief Encodes a move from the from- and to-square plus the move flag.
/// @param from
/// @param to
/// @param flag
/// @return 16-bit integer that encodes a move.
u16 define_move(int from, int to, int flag) {
    assert(from >= 0 && from < 64);
    assert(to >= 0 && to < 64);
    assert(flag >= 0 && flag < 16);

    u16 result = ((flag & 0xf) << 12) | ((to & 0x3f) << 6) | (from & 0x3f);
    return result;
}

/// @brief Gets the from square from a defined move.
/// @param move
/// @return 16-bit integer only containing the from square.
u16 get_from(u16 move) { return move & 0x3f; }

/// @brief Gets the to square from a defined move.
/// @param move
/// @return 16-bit integer only containing the to square.
u16 get_to(u16 move) { return (move >> 6) & 0x3f; }

/// @brief Gets the move flag from the defined move.
/// @param move
/// @return 16-bit integer only containing the move flag.
u16 get_flag(u16 move) { return (move >> 12) & 0xf; }

/// @brief Prints the bitboard in a board layout.
/// @param bitboard
void print_bitboard(u64 bitboard) {
    for (int rank = 7; rank >= 0; --rank) {
        for (int file = 0; file < 8; ++file) {
            int square = rank * 8 + file;
            std::cout << ((bitboard >> square) & 1) << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

/// @brief Prints the entire list of moves.
/// @param moves
void print_moves(std::vector<u16> &moves) {
    for (u64 move : moves) {
        std::cout << print_pos((int)get_from(move)) << print_pos((int)get_to(move)) << "\n";
    }
    std::cout << "\n";
}

/// @brief Merges two vectors into one.
/// @param vec
/// @param left
/// @param mid
/// @param right
void merge(std::vector<MoveScore> &vec, int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Temporary vectors
    std::vector<MoveScore> left_vec(n1), right_vec(n2);

    // Copy data
    for (i = 0; i < n1; i++) {
        left_vec[i] = vec[left + i];
    }
    for (j = 0; j < n2; j++) {
        right_vec[j] = vec[mid + 1 + j];
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

/// @brief Sorts the vector using merge sort algorithm.
/// @param vec
/// @param left
/// @param right
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

/// @brief Prints a single square as a position e.g. square 8 would be "a2".
/// @param pos
/// @return A string describing the square.
std::string print_pos(int pos) {
    std::string res = "";
    int rank = pos / 8;
    int file = pos % 8;

    res += 'a' + file;
    res += '1' + rank;
    return res;
}

/// @brief Filters out all quiet moves.
/// @param moves 
void only_noisy_moves(std::vector<u16> &moves) {
    for (int i = moves.size() - 1; i >= 0; i--) {
        int flag = (int)get_flag(moves[i]);

        if (flag == quiet_move || flag == double_pawn_push) {
            moves.erase(moves.begin() + i);
        }
    }
}
