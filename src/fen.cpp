#include "../include/fen.h"
#include <algorithm>
#include <string>

void FenHandler::load_fen(ChessGame &game, const std::string &fen) {
    u64 *bitboards = game.get_board().get_bitboards();
    int index = 56;
    int s_index = 0;
    int spaces = 0;
    int halfmove_clock;
    int fullmove_number;

    game.set_wk_castle(false);
    game.set_wq_castle(false);
    game.set_bk_castle(false);
    game.set_bq_castle(false);

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
        if (c == '-' && spaces == 3) {
            game.set_en_passant_sq(-1);
        }
        if (c >= 'a' && c <= 'h' && spaces == 3) {
            int col = c - 'a';
            if (s_index + 1 < fen.length() && isdigit(fen[s_index + 1])) {
                int row = fen[++s_index] - '1';
                game.set_en_passant_sq(col + row * 8);
            }
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

std::string FenHandler::write_fen(ChessGame &game) {
    std::string res = "";
    u64 *bitboards = game.get_board().get_bitboards();

    for (int rank = 7; rank >= 0; rank--) {
        int empty_count = 0;
        for (int file = 0; file < 8; file++) {
            char piece = ' ';

            for (int i = 3; i < 15; i++) {
                if (bitboards[i] & mask_piece[rank * 8 + file]) {
                    piece = symbols[i - 3];
                    break;
                }
            }

            if (piece == ' ') {
                empty_count++;
            } else {
                if (empty_count > 0) {
                    res += std::to_string(empty_count);
                    empty_count = 0;
                }
                res += piece;
            }
        }
        if (empty_count > 0) {
            res += std::to_string(empty_count);
        }
        if (rank > 0) {
            res += '/';
        }
    }

    res += ' ';
    res += game.get_turn() ? 'w' : 'b';
    res += ' ';

    if (!game.get_wk_castle() && !game.get_wq_castle() &&
        !game.get_bk_castle() && !game.get_bq_castle()) {
        res += '-';
    }
    if (game.get_wk_castle()) {
        res += 'K';
    }
    if (game.get_wq_castle()) {
        res += 'Q';
    }
    if (game.get_bk_castle()) {
        res += 'k';
    }
    if (game.get_bq_castle()) {
        res += 'q';
    }

    res += ' ';
    res += game.get_en_passant_sq() != -1 ? print_pos(game.get_en_passant_sq())
                                          : "-";

    res += ' ';
    res += std::to_string(game.get_half_moves());
    res += ' ';
    res += std::to_string(game.get_full_moves());

    return res;
}
