#include "../include/movegen.h"

void MoveGenerator::init() { BitBoardGenerator::init(); }

std::vector<u16> MoveGenerator::generate_legal_moves(ChessGame &game) {
    std::vector<u16> moves;

    bool single_ = false;
    bool double_ = false;
    check_detection(game.get_board().get_bitboards(), &single_, &double_, game.get_turn());
    game.set_singlecheck(single_);
    game.set_doublecheck(double_);

    generate_king_moves(moves, game);
    if (game.get_doublecheck()) {
        return moves;
    }
    generate_pawn_pushes(moves, game);
    generate_pawn_captures(moves, game);
    generate_knight_moves(moves, game);
    generate_bishop_moves(moves, game);
    generate_rook_moves(moves, game);
    generate_queen_moves(moves, game);

    if (game.get_singlecheck()) {
        moves = handle_single_check(moves, game);
    }

    return moves;
}

void MoveGenerator::generate_pawn_pushes(std::vector<u16> &moves, ChessGame &game) {
    bool turn = game.get_turn();
    u64 *bitboards = game.get_board().get_bitboards();
    u64 pawns = turn ? bitboards[WHITE_PAWN] : bitboards[BLACK_PAWN];
    u64 pinned_pawns = pawns & BitBoardGenerator::generate_pinned_pieces_bitboard(bitboards, turn);
    int king_sq = turn ? first_bit(bitboards[WHITE_KING]) : first_bit(bitboards[BLACK_KING]);
    int from, to;
    u64 pin_ray;

    while (pawns) {
        from = first_bit(pawns);
        u64 pushes = BitBoardGenerator::generate_pawn_bitboard(bitboards, mask_piece[from], turn);

        pin_ray = 0xffffffffffffffff;
        if (mask_piece[from] & pinned_pawns) {
            int pinner_sq = BitBoardGenerator::get_pinning_piece_square(bitboards, from, turn);
            pin_ray = BitBoardGenerator::precomputed_in_between[king_sq][pinner_sq] | mask_piece[pinner_sq];
        }
        pushes &= pin_ray;

        while (pushes) {
            to = first_bit(pushes);
            int to_rank = to / 8;

            if ((turn && to_rank == 7) || (!turn && to_rank == 0)) {
                moves.push_back(define_move(from, to, knight_promotion));
                moves.push_back(define_move(from, to, bishop_promotion));
                moves.push_back(define_move(from, to, rook_promotion));
                moves.push_back(define_move(from, to, queen_promotion));
            } else {
                if ((turn && to - from == 16) || (!turn && from - to == 16)) {
                    moves.push_back(define_move(from, to, double_pawn_push));
                } else {
                    moves.push_back(define_move(from, to, quiet_move));
                }
            }
            pushes &= pushes - 1;
        }

        pawns &= pawns - 1;
    }
}

void MoveGenerator::generate_pawn_captures(std::vector<u16> &moves, ChessGame &game) {
    bool turn = game.get_turn();
    u64 *bitboards = game.get_board().get_bitboards();
    u64 pawns = turn ? bitboards[WHITE_PAWN] : bitboards[BLACK_PAWN];
    u64 captures = BitBoardGenerator::generate_pawn_captures_bitboard(game, turn);
    u64 pinned_pawns = pawns & BitBoardGenerator::generate_pinned_pieces_bitboard(bitboards, turn);
    int king_sq = turn ? first_bit(bitboards[WHITE_KING]) : first_bit(bitboards[BLACK_KING]);
    int en_passant_sq = game.get_en_passant_sq();
    int from, to;
    u64 pin_ray;

    while (pawns) {
        from = first_bit(pawns);
        int file = from % 8;
        int rank = from / 8;

        pin_ray = 0xffffffffffffffff;
        if (pinned_pawns & mask_piece[from]) {
            int pinner_sq = BitBoardGenerator::get_pinning_piece_square(bitboards, from, turn);
            pin_ray = BitBoardGenerator::precomputed_in_between[king_sq][pinner_sq] | mask_piece[pinner_sq];
        }

        u64 rank_mask;
        if (turn) {
            rank_mask = rank < 7 ? mask_rank[rank + 1] : 0;
        } else {
            rank_mask = rank > 0 ? mask_rank[rank - 1] : 0;
        }

        if (file > 0) {
            u64 left_captures = captures & mask_file[file - 1] & rank_mask & pin_ray;

            while (left_captures) {
                to = first_bit(left_captures);
                int to_rank = to / 8;

                if ((turn && to_rank == 7) || (!turn && to_rank == 0)) {
                    moves.push_back(define_move(from, to, knight_promo_capture));
                    moves.push_back(define_move(from, to, bishop_promo_capture));
                    moves.push_back(define_move(from, to, rook_promo_capture));
                    moves.push_back(define_move(from, to, queen_promo_capture));
                } else {
                    moves.push_back(define_move(from, to, capture));
                }
                left_captures &= left_captures - 1;
            }
        }

        if (file < 7) {
            u64 right_captures = captures & mask_file[file + 1] & rank_mask & pin_ray;

            while (right_captures) {
                to = first_bit(right_captures);
                int to_rank = to / 8;

                if ((turn && to_rank == 7) || (!turn && to_rank == 0)) {
                    moves.push_back(define_move(from, to, knight_promo_capture));
                    moves.push_back(define_move(from, to, bishop_promo_capture));
                    moves.push_back(define_move(from, to, rook_promo_capture));
                    moves.push_back(define_move(from, to, queen_promo_capture));
                } else {
                    moves.push_back(define_move(from, to, capture));
                }
                right_captures &= right_captures - 1;
            }
        }

        if (en_passant_sq != -1) {
            u64 enp_mask = turn ? shift_north_west(mask_piece[from]) | shift_north_east(mask_piece[from])
                                : shift_south_west(mask_piece[from]) | shift_south_east(mask_piece[from]);
            if (mask_piece[en_passant_sq] & enp_mask & pin_ray) {
                u64 board_sim[15];
                for (int i = 0; i < 15; i++) {
                    board_sim[i] = bitboards[i];
                }
                to = en_passant_sq;
                int capture_sq = turn ? en_passant_sq - 8 : en_passant_sq + 8;
                for (int i = 3; i < 15; i++) {
                    if (mask_piece[from] & board_sim[i]) {
                        board_sim[i] &= ~mask_piece[from];
                        board_sim[i] |= mask_piece[to];
                    }
                    board_sim[i] &= ~mask_piece[capture_sq];
                }

                board_sim[WHITE] = board_sim[WHITE_PAWN] | board_sim[WHITE_ROOK] | board_sim[WHITE_BISHOP] |
                                   board_sim[WHITE_KNIGHT] | board_sim[WHITE_QUEEN] | board_sim[WHITE_KING];
                board_sim[BLACK] = board_sim[BLACK_PAWN] | board_sim[BLACK_ROOK] | board_sim[BLACK_BISHOP] |
                                   board_sim[BLACK_KNIGHT] | board_sim[BLACK_QUEEN] | board_sim[BLACK_KING];
                board_sim[ALL] = board_sim[WHITE] | board_sim[BLACK];

                u64 king_check = BitBoardGenerator::pieces_attacking_king(board_sim, turn);
                if (king_check == 0) {
                    moves.push_back(define_move(from, to, ep_capture));
                }
            }
        }

        pawns &= pawns - 1;
    }
}

void MoveGenerator::generate_knight_moves(std::vector<u16> &moves, ChessGame &game) {
    bool turn = game.get_turn();
    u64 *bitboards = game.get_board().get_bitboards();
    u64 knights = turn ? bitboards[WHITE_KNIGHT] : bitboards[BLACK_KNIGHT];
    u64 enemy = turn ? bitboards[BLACK] : bitboards[WHITE];
    u64 mask = turn ? ~bitboards[WHITE] : ~bitboards[BLACK];
    u64 pinned_knights = BitBoardGenerator::generate_pinned_pieces_bitboard(bitboards, turn) & knights;
    int from, to;
    while (knights) {
        from = first_bit(knights);
        u64 attacks = knight_attack_squares[from] & mask;
        if (pinned_knights & mask_piece[from]) {
            attacks = 0;
        }
        while (attacks) {
            to = first_bit(attacks);
            if (mask_piece[to] & enemy) {
                moves.push_back(define_move(from, to, capture));
            } else {
                moves.push_back(define_move(from, to, quiet_move));
            }
            attacks &= attacks - 1;
        }
        knights &= knights - 1;
    }
}

void MoveGenerator::generate_king_moves(std::vector<u16> &moves, ChessGame &game) {
    bool turn = game.get_turn();
    u64 *bitboards = game.get_board().get_bitboards();
    u64 king = turn ? bitboards[WHITE_KING] : bitboards[BLACK_KING];
    u64 enemy = turn ? bitboards[BLACK] : bitboards[WHITE];
    u64 attacks = BitBoardGenerator::generate_king_bitboard(game, turn);
    u64 castle = BitBoardGenerator::generate_castle_bitboard(game, turn);

    int from, to;
    u16 flag;
    if (king) {
        from = first_bit(king);
        while (attacks) {
            to = first_bit(attacks);
            if (simulate_check(bitboards, turn ? WHITE_KING : BLACK_KING, turn, from, to)) {
                flag = enemy & mask_piece[to] ? capture : quiet_move;
                moves.push_back(define_move(from, to, flag));
            }
            attacks &= attacks - 1;
        }

        while (castle) {
            to = first_bit(castle);
            flag = (to - from) > 0 ? king_castle : queen_castle;
            moves.push_back(define_move(from, to, flag));
            castle &= castle - 1;
        }
    }
}

void MoveGenerator::generate_bishop_moves(std::vector<u16> &moves, ChessGame &game) {
    bool turn = game.get_turn();
    u64 *bitboards = game.get_board().get_bitboards();
    u64 bishops = turn ? bitboards[WHITE_BISHOP] : bitboards[BLACK_BISHOP];
    u64 enemy = turn ? bitboards[BLACK] : bitboards[WHITE];
    u64 pinned_bishops = BitBoardGenerator::generate_pinned_pieces_bitboard(bitboards, turn) & bishops;
    int king_sq = turn ? first_bit(bitboards[WHITE_KING]) : first_bit(bitboards[BLACK_KING]);

    int from, to;
    u16 flag;
    while (bishops) {
        from = first_bit(bishops);
        u64 attacks = BitBoardGenerator::generate_bishop_bitboard(bitboards, from, turn);
        if (pinned_bishops & mask_piece[from]) {
            int pinner_sq = BitBoardGenerator::get_pinning_piece_square(bitboards, from, turn);
            u64 pin_ray = BitBoardGenerator::precomputed_in_between[king_sq][pinner_sq] | mask_piece[pinner_sq];
            attacks &= pin_ray;
        }

        while (attacks) {
            to = first_bit(attacks);
            flag = (enemy & mask_piece[to]) ? capture : quiet_move;
            moves.push_back(define_move(from, to, flag));
            attacks &= attacks - 1;
        }
        bishops &= bishops - 1;
    }
}

void MoveGenerator::generate_rook_moves(std::vector<u16> &moves, ChessGame &game) {
    bool turn = game.get_turn();
    u64 *bitboards = game.get_board().get_bitboards();
    u64 rooks = turn ? bitboards[WHITE_ROOK] : bitboards[BLACK_ROOK];
    u64 enemy = turn ? bitboards[BLACK] : bitboards[WHITE];
    u64 pinned_rooks = BitBoardGenerator::generate_pinned_pieces_bitboard(bitboards, turn) & rooks;
    int king_sq = turn ? first_bit(bitboards[WHITE_KING]) : first_bit(bitboards[BLACK_KING]);

    int from, to;
    u16 flag;
    while (rooks) {
        from = first_bit(rooks);
        u64 attacks = BitBoardGenerator::generate_rook_bitboard(bitboards, from, turn);
        if (pinned_rooks & mask_piece[from]) {
            int pinner_sq = BitBoardGenerator::get_pinning_piece_square(bitboards, from, turn);
            u64 pin_ray = BitBoardGenerator::precomputed_in_between[king_sq][pinner_sq] | mask_piece[pinner_sq];
            attacks &= pin_ray;
        }

        while (attacks) {
            to = first_bit(attacks);
            flag = (enemy & mask_piece[to]) ? capture : quiet_move;
            moves.push_back(define_move(from, to, flag));
            attacks &= attacks - 1;
        }
        rooks &= rooks - 1;
    }
}

void MoveGenerator::generate_queen_moves(std::vector<u16> &moves, ChessGame &game) {
    bool turn = game.get_turn();
    u64 *bitboards = game.get_board().get_bitboards();
    u64 queen = turn ? bitboards[WHITE_QUEEN] : bitboards[BLACK_QUEEN];
    u64 enemy = turn ? bitboards[BLACK] : bitboards[WHITE];
    u64 pinned_queens = BitBoardGenerator::generate_pinned_pieces_bitboard(bitboards, turn) & queen;
    int king_sq = turn ? first_bit(bitboards[WHITE_KING]) : first_bit(bitboards[BLACK_KING]);

    int from, to;
    u16 flag;
    while (queen) {
        from = first_bit(queen);
        u64 attacks = BitBoardGenerator::generate_queen_bitboard(bitboards, from, turn);
        if (pinned_queens & mask_piece[from]) {
            int pinner_sq = BitBoardGenerator::get_pinning_piece_square(bitboards, from, turn);
            u64 pin_ray = BitBoardGenerator::precomputed_in_between[king_sq][pinner_sq] | mask_piece[pinner_sq];
            attacks &= pin_ray;
        }

        while (attacks) {
            to = first_bit(attacks);
            flag = (enemy & mask_piece[to]) ? capture : quiet_move;
            moves.push_back(define_move(from, to, flag));
            attacks &= attacks - 1;
        }
        queen &= queen - 1;
    }
}

void MoveGenerator::check_detection(u64 *bitboards, bool *single_check, bool *double_check, bool turn) {
    u64 attacks_to_king = BitBoardGenerator::pieces_attacking_king(bitboards, turn);
    int check_count = std::__popcount(attacks_to_king);
    *single_check = check_count == 1;
    *double_check = check_count > 1;
}

std::vector<u16> MoveGenerator::handle_single_check(std::vector<u16> &moves, ChessGame &game) {
    std::vector<u16> check_moves;
    bool turn = game.get_turn();
    u64 *bitboards = game.get_board().get_bitboards();

    u64 pieces_attacking_king = BitBoardGenerator::pieces_attacking_king(bitboards, turn);

    int attacking_piece_sq = first_bit(pieces_attacking_king);
    int king_sq = first_bit(turn ? bitboards[WHITE_KING] : bitboards[BLACK_KING]);

    int en_passant_sq = game.get_en_passant_sq();
    int en_passant_piece_sq = (en_passant_sq != -1) ? (turn ? en_passant_sq + 8 : en_passant_sq - 8) : -1;

    bool is_bishop = pieces_attacking_king & (turn ? bitboards[BLACK_BISHOP] | bitboards[BLACK_QUEEN]
                                                   : bitboards[WHITE_BISHOP] | bitboards[WHITE_QUEEN]);
    bool is_rook = pieces_attacking_king & (turn ? bitboards[BLACK_ROOK] | bitboards[BLACK_QUEEN]
                                                 : bitboards[WHITE_ROOK] | bitboards[WHITE_QUEEN]);

    u64 in_between = BitBoardGenerator::precomputed_in_between[king_sq][attacking_piece_sq];

    for (u16 move : moves) {
        int from = (int)get_from(move);
        int to = (int)get_to(move);

        if (from == king_sq) {
            check_moves.push_back(move);
            continue;
        }

        if (to == attacking_piece_sq) {
            check_moves.push_back(move);
            continue;
        }

        if (to == en_passant_sq && en_passant_piece_sq == attacking_piece_sq) {
            check_moves.push_back(move);
            continue;
        }

        if (in_between & mask_piece[to]) {
            check_moves.push_back(move);
        }
    }

    return check_moves;
}

bool MoveGenerator::simulate_check(u64 *bitboards, PieceType type, bool turn, int from, int to) {
    u64 sim[15];
    for (int i = 0; i < 15; i++) {
        sim[i] = bitboards[i];
    }

    for (int i = 3; i < 15; i++) {
        if (mask_piece[to] & sim[i]) {
            sim[i] &= ~mask_piece[to];
        }
    }

    sim[type] &= ~mask_piece[from];
    sim[type] |= mask_piece[to];

    sim[WHITE] =
        sim[WHITE_PAWN] | sim[WHITE_ROOK] | sim[WHITE_BISHOP] | sim[WHITE_KNIGHT] | sim[WHITE_QUEEN] | sim[WHITE_KING];
    sim[BLACK] =
        sim[BLACK_PAWN] | sim[BLACK_ROOK] | sim[BLACK_BISHOP] | sim[BLACK_KNIGHT] | sim[BLACK_QUEEN] | sim[BLACK_KING];
    sim[ALL] = sim[WHITE] | sim[BLACK];

    return BitBoardGenerator::pieces_attacking_king(sim, turn) == 0;
}
