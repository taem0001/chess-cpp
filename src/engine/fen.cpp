#include "../include/fen.h"

void FenHandler::load_fen(ChessLogic &logic, const std::string &fen) {
    u64 *bitboards = logic.get_board().get_bitboards();
    int *piece_on_square = logic.get_board().get_piece_on_squares();

    // Set all squares to be empty
    std::fill(bitboards, bitboards + 15, 0);
    std::fill(piece_on_square, piece_on_square + 64, -1);

    int index = 56;
    int s_index = 0;
    int spaces = 0;
    int half_m = 0;
    int full_m = 0;

    logic.set_wk_castle(false);
    logic.set_wq_castle(false);
    logic.set_bk_castle(false);
    logic.set_bq_castle(false);

    while (s_index < fen.length()) {
        char c = fen[s_index];

        if (spaces == 0) {
            if (isdigit(c)) {
                index += (c - '0');
            } else if (c == '/') {
                index -= 16;
            } else {
                int piece_type = -1;
                switch (c) {
                    case 'p':
                        piece_type = BLACK_PAWN;
                        break;
                    case 'r':
                        piece_type = BLACK_ROOK;
                        break;
                    case 'n':
                        piece_type = BLACK_KNIGHT;
                        break;
                    case 'b':
                        piece_type = BLACK_BISHOP;
                        break;
                    case 'q':
                        piece_type = BLACK_QUEEN;
                        break;
                    case 'k':
                        piece_type = BLACK_KING;
                        break;
                    case 'P':
                        piece_type = WHITE_PAWN;
                        break;
                    case 'R':
                        piece_type = WHITE_ROOK;
                        break;
                    case 'N':
                        piece_type = WHITE_KNIGHT;
                        break;
                    case 'B':
                        piece_type = WHITE_BISHOP;
                        break;
                    case 'Q':
                        piece_type = WHITE_QUEEN;
                        break;
                    case 'K':
                        piece_type = WHITE_KING;
                        break;
                    default:
                        break;
                }
                assert(piece_type == -1 || (piece_type >= 3 && piece_type < 15));

                if (piece_type != -1) {
                    bitboards[piece_type] |= mask_piece[index];
                    piece_on_square[index] = piece_type;
                }

                index++;
            }
        }
        if (c == ' ') {
            spaces++;
        }
        if (c == 'w' && spaces == 1) {
            logic.set_turn(true);
        }
        if (c == 'b' && spaces == 1) {
            logic.set_turn(false);
        }
        if (c == 'K' && spaces == 2) {
            logic.set_wk_castle(true);
        }
        if (c == 'Q' && spaces == 2) {
            logic.set_wq_castle(true);
        }
        if (c == 'k' && spaces == 2) {
            logic.set_bk_castle(true);
        }
        if (c == 'q' && spaces == 2) {
            logic.set_bq_castle(true);
        }
        if (c == '-' && spaces == 2) {
            logic.set_wk_castle(false);
            logic.set_wq_castle(false);
            logic.set_bk_castle(false);
            logic.set_bq_castle(false);
        }
        if (c == '-' && spaces == 3) {
            logic.set_en_passant_sq(-1);
        }
        if (c >= 'a' && c <= 'h' && spaces == 3) {
            int col = c - 'a';
            if (s_index + 1 < fen.length() && isdigit(fen[s_index + 1])) {
                int row = fen[++s_index] - '1';
                logic.set_en_passant_sq(col + row * 8);
            }
        }
        if (isdigit(c) && spaces == 4) {
            half_m = half_m * 10 + (c - '0');
        }
        if (isdigit(c) && spaces == 5) {
            full_m = full_m * 10 + (c - '0');
        }
        s_index++;
    }
    logic.set_halfmoves(half_m);
    logic.set_fullmoves(full_m);

    bitboards[WHITE] = bitboards[WHITE_PAWN] | bitboards[WHITE_ROOK] | bitboards[WHITE_BISHOP] |
                       bitboards[WHITE_KNIGHT] | bitboards[WHITE_QUEEN] | bitboards[WHITE_KING];
    bitboards[BLACK] = bitboards[BLACK_PAWN] | bitboards[BLACK_ROOK] | bitboards[BLACK_BISHOP] |
                       bitboards[BLACK_KNIGHT] | bitboards[BLACK_QUEEN] | bitboards[BLACK_KING];
    bitboards[ALL] = bitboards[WHITE] | bitboards[BLACK];
}

std::string FenHandler::write_fen(ChessLogic &logic) {
    std::string res = "";
    u64 *bitboards = logic.get_board().get_bitboards();

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
    res += logic.get_turn() ? 'w' : 'b';
    res += ' ';

    if (!logic.get_wk_castle() && !logic.get_wq_castle() && !logic.get_bk_castle() && !logic.get_bq_castle()) {
        res += '-';
    }
    if (logic.get_wk_castle()) {
        res += 'K';
    }
    if (logic.get_wq_castle()) {
        res += 'Q';
    }
    if (logic.get_bk_castle()) {
        res += 'k';
    }
    if (logic.get_bq_castle()) {
        res += 'q';
    }

    res += ' ';
    res += logic.get_en_passant_sq() != -1 ? print_pos(logic.get_en_passant_sq()) : "-";

    res += ' ';
    res += std::to_string(logic.get_halfmoves());
    res += ' ';
    res += std::to_string(logic.get_fullmoves());

    return res;
}
