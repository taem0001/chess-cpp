#include "../include/movegen.h"

// TOOD: Implement sliding pieces movegen

void MoveGenerator::init() { BitBoardGenerator::init(); }

std::vector<u16> MoveGenerator::generate_pseudolegal_moves(ChessGame &game) {
    std::vector<u16> moves;
    generate_pawn_moves(moves, game);
    generate_knight_moves(moves, game);
    generate_king_moves(moves, game);
    generate_bishop_moves(moves, game);
    generate_rook_moves(moves, game);
    generate_queen_moves(moves, game);;
    return moves;
}

void MoveGenerator::generate_pawn_moves(std::vector<u16> &moves,
                                        ChessGame &game) {
    bool turn = game.get_turn();
    u64 *bitboards = game.get_board().get_bitboards();
    u64 pawns = turn ? bitboards[WHITE_PAWN] : bitboards[BLACK_PAWN];
    u64 pushes = turn ? BitBoardGenerator::generate_white_pawn_bitboard(game)
                      : BitBoardGenerator::generate_black_pawn_bitboard(game);
    u64 captures =
        turn ? BitBoardGenerator::generate_white_pawn_captures_bitboard(game)
             : BitBoardGenerator::generate_black_pawn_captures_bitboard(game);

    int en_passant_sq = game.get_en_passant_sq();
    int from, to;
    u16 flag;
    while (pawns) {
        from = first_bit(pawns);
        int file = from % 8;
        int rank = from / 8;
        u64 file_pushes = pushes & mask_file[file];
        while (file_pushes) {
            to = first_bit(file_pushes);
            flag = abs(to - from) == 16 ? double_pawn_push : quiet_move;
            moves.push_back(define_move(from, to, flag));
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
            while (left_captures) {
                to = first_bit(left_captures);
                flag = to == en_passant_sq ? ep_capture : capture;
                moves.push_back(define_move(from, to, flag));
                left_captures &= left_captures - 1;
            }
        }

        if (file < 7) {
            u64 right_captures = captures & mask_file[file + 1] & rank_mask;
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
    u64 attacks = turn
                      ? BitBoardGenerator::generate_white_knight_bitboard(game)
                      : BitBoardGenerator::generate_black_knight_bitboard(game);

    int from, to;
    u16 flag;
    while (knights) {
        from = first_bit(knights);
        u64 attacks_t = attacks & knight_attack_squares[from];
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
    u64 attacks = turn ? BitBoardGenerator::generate_white_king_bitboard(game)
                       : BitBoardGenerator::generate_black_king_bitboard(game);
    u64 castle = turn ? BitBoardGenerator::generate_white_castle_bitboard(game)
                      : BitBoardGenerator::generate_black_castle_bitboard(game);
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
    u64 attacks = turn
                      ? BitBoardGenerator::generate_white_bishop_bitboard(game)
                      : BitBoardGenerator::generate_black_bishop_bitboard(game);

    int from, to;
    u16 flag;
    while (bishops) {
        from = first_bit(bishops);
        u64 mask = BitBoardGenerator::precomputed_bishop[from];
        u64 attacks_t = attacks & mask;

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
    u64 attacks = turn ? BitBoardGenerator::generate_white_rook_bitboard(game)
                       : BitBoardGenerator::generate_black_rook_bitboard(game);

    int from, to;
    u16 flag;
    while (rooks) {
        from = first_bit(rooks);
        u64 mask = BitBoardGenerator::precomputed_rook[from];
        u64 attacks_t = attacks & mask;

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
    u64 attacks = turn ? BitBoardGenerator::generate_white_queen_bitboard(game)
                       : BitBoardGenerator::generate_black_queen_bitboard(game);

    int from, to;
    u16 flag;
    if (queen) {
        from = first_bit(queen);
        while (attacks) {
            to = first_bit(attacks);
            flag = (enemy & mask_piece[to]) ? capture : quiet_move;
            moves.push_back(define_move(from, to, flag));
            attacks &= attacks - 1;
        }
    }
}
