#include "../include/movegen.h"

// TODO: Implement legal movegen

void MoveGenerator::init() { BitBoardGenerator::init(); }

std::vector<u16> MoveGenerator::generate_legal_moves(ChessGame &game) {
    std::vector<u16> legal_moves;
    std::vector<u16> moves = generate_pseudolegal_moves(game);
    return moves;
}

std::vector<u16> MoveGenerator::generate_pseudolegal_moves(ChessGame &game) {
    std::vector<u16> moves;
    generate_pawn_moves(moves, game);
    generate_knight_moves(moves, game);
    generate_king_moves(moves, game);
    generate_bishop_moves(moves, game);
    generate_rook_moves(moves, game);
    generate_queen_moves(moves, game);
    return moves;
}

void MoveGenerator::generate_pawn_moves(std::vector<u16> &moves,
                                        ChessGame &game) {
    bool turn = game.get_turn();
    u64 *bitboards = game.get_board().get_bitboards();
    u64 pawns = turn ? bitboards[WHITE_PAWN] : bitboards[BLACK_PAWN];
    u64 pushes = BitBoardGenerator::generate_pawn_bitboard(bitboards, turn);
    u64 captures =
        BitBoardGenerator::generate_pawn_captures_bitboard(game, turn);
    u64 pinned_pawns =
        pawns &
        BitBoardGenerator::generate_pinned_pieces_bitboard(bitboards, turn);
    int en_passant_sq = game.get_en_passant_sq();
    int king_sq = turn ? first_bit(bitboards[WHITE_KING])
                       : first_bit(bitboards[BLACK_KING]);
    int from, to;
    u16 flag;
    u64 pin_ray;
    while (pawns) {
        from = first_bit(pawns);

        int file = from % 8;
        int rank = from / 8;

        u64 file_pushes = pushes & mask_file[file];
        if (pinned_pawns & mask_piece[from]) {
            int pinner_sq = BitBoardGenerator::get_pinning_piece_square(
                bitboards, from, turn);
            pin_ray =
                BitBoardGenerator::precomputed_in_between[king_sq][pinner_sq] |
                mask_piece[pinner_sq];
            file_pushes &= pin_ray;
        }
        while (file_pushes) {
            to = first_bit(file_pushes);
            int to_rank = to / 8;
            if ((turn && to_rank == 7) || (!turn && to_rank == 0)) {
                moves.push_back(define_move(from, to, knight_promotion));
                moves.push_back(define_move(from, to, bishop_promotion));
                moves.push_back(define_move(from, to, rook_promotion));
                moves.push_back(define_move(from, to, queen_promotion));
            } else {
                flag = abs(to - from) == 16 ? double_pawn_push : quiet_move;
                moves.push_back(define_move(from, to, flag));
            }
            file_pushes &= file_pushes - 1;
        }

        u64 rank_mask;
        if (turn) {
            rank_mask = rank < 7 ? mask_rank[rank + 1] : 0;
        } else {
            rank_mask = rank > 0 ? mask_rank[rank - 1] : 0;
        }
        if (file > 0) {
            u64 left_captures = captures & mask_file[file - 1] & rank_mask;
            if (pinned_pawns & mask_piece[from]) {
                left_captures &= pin_ray;
            }
            while (left_captures) {
                to = first_bit(left_captures);
                flag = to == en_passant_sq ? ep_capture : capture;
                moves.push_back(define_move(from, to, flag));
                left_captures &= left_captures - 1;
            }
        }

        if (file < 7) {
            u64 right_captures = captures & mask_file[file + 1] & rank_mask;
            if (pinned_pawns & mask_piece[from]) {
                right_captures &= pin_ray;
            }
            while (right_captures) {
                to = first_bit(right_captures);
                flag = to == en_passant_sq ? ep_capture : capture;
                moves.push_back(define_move(from, to, flag));
                right_captures &= right_captures - 1;
            }
        }
        pawns &= pawns - 1;
    }
}

void MoveGenerator::generate_knight_moves(std::vector<u16> &moves,
                                          ChessGame &game) {
    bool turn = game.get_turn();
    u64 *bitboards = game.get_board().get_bitboards();
    u64 knights = turn ? bitboards[WHITE_KNIGHT] : bitboards[BLACK_KNIGHT];
    u64 enemy = turn ? bitboards[BLACK] : bitboards[WHITE];
    u64 attacks = BitBoardGenerator::generate_knight_bitboard(bitboards, turn);
    u64 pinned_knights =
        BitBoardGenerator::generate_pinned_pieces_bitboard(bitboards, turn) &
        knights;
    int king_sq = turn ? first_bit(bitboards[WHITE_KING])
                       : first_bit(bitboards[BLACK_KING]);
    int from, to;
    u16 flag;
    while (knights) {
        from = first_bit(knights);
        u64 attacks_t = attacks & knight_attack_squares[from];
        if (pinned_knights & mask_piece[from]) {
            attacks_t = 0;
        }
        while (attacks_t) {
            to = first_bit(attacks_t);
            flag = enemy & mask_piece[to] ? capture : quiet_move;
            moves.push_back(define_move(from, to, flag));
            attacks_t &= attacks_t - 1;
        }
        knights &= knights - 1;
    }
}

void MoveGenerator::generate_king_moves(std::vector<u16> &moves,
                                        ChessGame &game) {
    bool turn = game.get_turn();
    u64 *bitboards = game.get_board().get_bitboards();
    u64 king = turn ? bitboards[WHITE_KING] : bitboards[BLACK_KING];
    u64 enemy = turn ? bitboards[BLACK] : bitboards[WHITE];
    u64 attacks = BitBoardGenerator::generate_king_bitboard(game, turn);
    u64 castle = BitBoardGenerator::generate_castle_bitboard(game, turn);
    u64 enemy_attacks =
        BitBoardGenerator::generate_attacks_bitboard(game, !turn);
    attacks &= ~enemy_attacks;
    int to;
    u16 flag;
    if (king) {
        int from = first_bit(king);
        while (attacks) {
            to = first_bit(attacks);
            flag = enemy & mask_piece[to] ? capture : quiet_move;
            moves.push_back(define_move(from, to, flag));
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

void MoveGenerator::generate_bishop_moves(std::vector<u16> &moves,
                                          ChessGame &game) {
    bool turn = game.get_turn();
    u64 *bitboards = game.get_board().get_bitboards();
    u64 bishops = turn ? bitboards[WHITE_BISHOP] : bitboards[BLACK_BISHOP];
    u64 enemy = turn ? bitboards[BLACK] : bitboards[WHITE];
    u64 attacks = BitBoardGenerator::generate_bishop_bitboard(bitboards, turn);
    u64 pinned_bishops =
        BitBoardGenerator::generate_pinned_pieces_bitboard(bitboards, turn) &
        bishops;
    int king_sq = turn ? first_bit(bitboards[WHITE_KING])
                       : first_bit(bitboards[BLACK_KING]);

    int from, to;
    u16 flag;
    while (bishops) {
        from = first_bit(bishops);
        u64 mask = BitBoardGenerator::precomputed_bishop[from];
        u64 attacks_t = attacks & mask;
        if (pinned_bishops & mask_piece[from]) {
            int pinner_sq = BitBoardGenerator::get_pinning_piece_square(
                bitboards, from, turn);
            u64 pin_ray =
                BitBoardGenerator::precomputed_in_between[king_sq][pinner_sq] |
                mask_piece[pinner_sq];
            attacks_t &= pin_ray;
        }

        while (attacks_t) {
            to = first_bit(attacks_t);
            flag = (enemy & mask_piece[to]) ? capture : quiet_move;
            moves.push_back(define_move(from, to, flag));
            attacks_t &= attacks_t - 1;
        }
        bishops &= bishops - 1;
    }
}

void MoveGenerator::generate_rook_moves(std::vector<u16> &moves,
                                        ChessGame &game) {
    bool turn = game.get_turn();
    u64 *bitboards = game.get_board().get_bitboards();
    u64 rooks = turn ? bitboards[WHITE_ROOK] : bitboards[BLACK_ROOK];
    u64 enemy = turn ? bitboards[BLACK] : bitboards[WHITE];
    u64 attacks = BitBoardGenerator::generate_rook_bitboard(bitboards, turn);
    u64 pinned_rooks =
        BitBoardGenerator::generate_pinned_pieces_bitboard(bitboards, turn) &
        rooks;
    int king_sq = turn ? first_bit(bitboards[WHITE_KING])
                       : first_bit(bitboards[BLACK_KING]);

    int from, to;
    u16 flag;
    while (rooks) {
        from = first_bit(rooks);
        u64 mask = BitBoardGenerator::precomputed_rook[from];
        u64 attacks_t = attacks & mask;
        if (pinned_rooks & mask_piece[from]) {
            int pinner_sq = BitBoardGenerator::get_pinning_piece_square(
                bitboards, from, turn);
            u64 pin_ray =
                BitBoardGenerator::precomputed_in_between[king_sq][pinner_sq] |
                mask_piece[pinner_sq];
            attacks_t &= pin_ray;
        }

        while (attacks_t) {
            to = first_bit(attacks_t);
            flag = (enemy & mask_piece[to]) ? capture : quiet_move;
            moves.push_back(define_move(from, to, flag));
            attacks_t &= attacks_t - 1;
        }
        rooks &= rooks - 1;
    }
}

void MoveGenerator::generate_queen_moves(std::vector<u16> &moves,
                                         ChessGame &game) {
    bool turn = game.get_turn();
    u64 *bitboards = game.get_board().get_bitboards();
    u64 queen = turn ? bitboards[WHITE_QUEEN] : bitboards[BLACK_QUEEN];
    u64 enemy = turn ? bitboards[BLACK] : bitboards[WHITE];
    u64 attacks = BitBoardGenerator::generate_queen_bitboard(bitboards, turn);
    u64 pinned_queens =
        BitBoardGenerator::generate_pinned_pieces_bitboard(bitboards, turn) &
        queen;
    int king_sq = turn ? first_bit(bitboards[WHITE_KING])
                       : first_bit(bitboards[BLACK_KING]);

    int from, to;
    u16 flag;
    while (queen) {
        from = first_bit(queen);
        u64 mask = BitBoardGenerator::precomputed_bishop[from] |
                   BitBoardGenerator::precomputed_rook[from];
        u64 attacks_t = attacks & mask;
        if (pinned_queens & mask_piece[from]) {
            int pinner_sq = BitBoardGenerator::get_pinning_piece_square(
                bitboards, from, turn);
            u64 pin_ray =
                BitBoardGenerator::precomputed_in_between[king_sq][pinner_sq] |
                mask_piece[pinner_sq];
            attacks_t &= pin_ray;
        }

        while (attacks_t) {
            to = first_bit(attacks_t);
            flag = (enemy & mask_piece[to]) ? capture : quiet_move;
            moves.push_back(define_move(from, to, flag));
            attacks_t &= attacks_t - 1;
        }
        queen &= queen - 1;
    }
}
