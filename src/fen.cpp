#include "../include/fen.h"

void FenHandler::load_fen(Board &board, const std::string &fen) {
    u64 *bitboards = board.get_bitboards();
    int index = 0;
    int s_index = 0;

    while (s_index < fen.length()) {
        switch (fen[s_index]) {
            case 'p':
                bitboards[BLACK_PAWN] |= 1ULL << index;
                index++;
                break;
            case 'r':
                bitboards[BLACK_ROOK] |= 1ULL << index;
                index++;
                break;
            case 'n':
                bitboards[BLACK_KNIGHT] |= 1ULL << index;
                index++;
                break;
            case 'b':
                bitboards[BLACK_BISHOP] |= 1ULL << index;
                index++;
                break;
            case 'q':
                bitboards[BLACK_QUEEN] |= 1ULL << index;
                index++;
                break;
            case 'k':
                bitboards[BLACK_KING] |= 1ULL << index;
                index++;
                break;
            case 'P':
                bitboards[WHITE_PAWN] |= 1ULL << index;
                index++;
                break;
            case 'R':
                bitboards[WHITE_ROOK] |= 1ULL << index;
                index++;
                break;
            case 'N':
                bitboards[WHITE_KNIGHT] |= 1ULL << index;
                index++;
                break;
            case 'B':
                bitboards[WHITE_BISHOP] |= 1ULL << index;
                index++;
                break;
            case 'Q':
                bitboards[WHITE_QUEEN] |= 1ULL << index;
                index++;
                break;
            case 'K':
                bitboards[WHITE_KING] |= 1ULL << index;
                index++;
                break;
            case '1' ... '8':
                index += fen[s_index] - '0';
                break;
            case '/':
                break;
        }
        s_index++;
    }

    bitboards[WHITE] = bitboards[WHITE_PAWN] | bitboards[WHITE_ROOK] |
                       bitboards[WHITE_BISHOP] | bitboards[WHITE_KNIGHT] |
                       bitboards[WHITE_QUEEN] | bitboards[WHITE_KING];
    bitboards[BLACK] = bitboards[BLACK_PAWN] | bitboards[BLACK_ROOK] |
                       bitboards[BLACK_BISHOP] | bitboards[BLACK_KNIGHT] |
                       bitboards[BLACK_QUEEN] | bitboards[BLACK_KING];
    bitboards[ALL] = bitboards[WHITE] | bitboards[BLACK];
}
