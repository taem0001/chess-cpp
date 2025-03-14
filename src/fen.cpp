#include "../include/fen.h"

void FenHandler::load_fen(ChessGame &game, const std::string &fen) {
    u64 *bitboards = game.get_board().get_bitboards();
    int index = 56;
    int s_index = 0;
    int spaces = 0;

    while (s_index < fen.length()) {
        char c = fen[s_index];

        if (spaces == 0) {
            if (isdigit(c)) {
                index += (c - '0');
            } else if (c == '/') {
                index -= 16;
            } else {
                switch (c) {
                    case 'p':
                        bitboards[BLACK_PAWN] |= 1ULL << index;
                        break;
                    case 'r':
                        bitboards[BLACK_ROOK] |= 1ULL << index;
                        break;
                    case 'n':
                        bitboards[BLACK_KNIGHT] |= 1ULL << index;
                        break;
                    case 'b':
                        bitboards[BLACK_BISHOP] |= 1ULL << index;
                        break;
                    case 'q':
                        bitboards[BLACK_QUEEN] |= 1ULL << index;
                        break;
                    case 'k':
                        bitboards[BLACK_KING] |= 1ULL << index;
                        break;
                    case 'P':
                        bitboards[WHITE_PAWN] |= 1ULL << index;
                        break;
                    case 'R':
                        bitboards[WHITE_ROOK] |= 1ULL << index;
                        break;
                    case 'N':
                        bitboards[WHITE_KNIGHT] |= 1ULL << index;
                        break;
                    case 'B':
                        bitboards[WHITE_BISHOP] |= 1ULL << index;
                        break;
                    case 'Q':
                        bitboards[WHITE_QUEEN] |= 1ULL << index;
                        break;
                    case 'K':
                        bitboards[WHITE_KING] |= 1ULL << index;
                        break;
                    default:
                        break;
                }
                index++;
            }
        }
        if (c == ' ') {
            spaces++;
        }
        if (c == 'w' && spaces == 1) {
            game.set_turn(true);
        }
        if (c == 'b' && spaces == 1) {
            game.set_turn(false);
        }
        if (c == 'K' && spaces == 2) {
            game.set_wk_castle(true);
        }
        if (c == 'Q' && spaces == 2) {
            game.set_wq_castle(true);
        }
        if (c == 'k' && spaces == 2) {
            game.set_bk_castle(true);
        }
        if (c == 'q' && spaces == 2) {
            game.set_bq_castle(true);
        }
        if (c == '-' && spaces == 2) {
            game.set_wk_castle(false);
            game.set_wq_castle(false);
            game.set_bk_castle(false);
            game.set_bq_castle(false);
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
