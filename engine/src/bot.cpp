#include "../include/bot.h"

Bot::Bot() { MoveGenerator::init(); }

u64 Bot::choose_move(ChessLogic logic, std::vector<u64> &moves) {
    int color = logic.get_turn() ? 1 : -1;
    return search_move(logic, 5, color);
}

int Bot::evaluate(ChessLogic &logic) {
    u64 *bitboards = logic.get_board().get_bitboards();

    u64 q_w = bitboards[WHITE_QUEEN];
    u64 q_b = bitboards[BLACK_QUEEN];
    u64 r_w = bitboards[WHITE_ROOK];
    u64 r_b = bitboards[BLACK_ROOK];
    u64 b_w = bitboards[WHITE_BISHOP];
    u64 b_b = bitboards[BLACK_BISHOP];
    u64 n_w = bitboards[WHITE_KNIGHT];
    u64 n_b = bitboards[BLACK_KNIGHT];
    u64 p_w = bitboards[WHITE_PAWN];
    u64 p_b = bitboards[BLACK_PAWN];

    int q_diff = __popcnt64(q_w) - __popcnt64(q_b);
    int r_diff = __popcnt64(r_w) - __popcnt64(r_b);
    int b_diff = __popcnt64(b_w) - __popcnt64(b_b);
    int n_diff = __popcnt64(n_w) - __popcnt64(n_b);
    int p_diff = __popcnt64(p_w) - __popcnt64(p_b);

    return queen_value * q_diff + rook_value * r_diff + bishop_value * b_diff + knight_value * n_diff +
           pawn_value * p_diff;
}

int Bot::negamax(ChessLogic &logic, int depth, int color, int alpha, int beta) {
    if (depth == 0) {
        return evaluate(logic);
    }

    std::vector<u64> unorderd_moves = MoveGenerator::generate_legal_moves(logic);
    std::vector<u64> moves = order_moves(logic, unorderd_moves);
    if (moves.size() == 0) {
        if (logic.get_singlecheck() || logic.get_doublecheck()) {
            return -INF + depth;
        }
        return 0;
    }

    int best_score = -INF;

    for (u64 move : moves) {
        logic.make_move(move);
        int score = -negamax(logic, depth - 1, -color, -beta, -alpha);
        logic.unmake_move(move);

        best_score = max(best_score, score);
        alpha = max(alpha, score);
        if (alpha >= beta) {
            break;
        }
    }
    return best_score;
}

std::vector<u64> Bot::order_moves(ChessLogic &logic, std::vector<u64> moves) {
    std::vector<MoveScore> move_scores;
    std::vector<u64> result;
    u64 *bitboards = logic.get_board().get_bitboards();

    for (u64 move : moves) {
        int move_score = 0;
        int from = get_from(move);
        int to = get_to(move);
        int flag = get_flag(move);

        // Reward captures and especially between low-ranking and high-ranking pieces
        if (flag == capture || flag == ep_capture) {
            move_score += 10 * get_piece_score(bitboards, to) - get_piece_score(bitboards, from);
        }

        // Reward promotions
        if (flag == queen_promotion || flag == queen_promo_capture) {
            move_score += 2000;
        }
        if (flag == rook_promotion || flag == rook_promo_capture) {
            move_score += 1200;
        }
        if (flag == knight_promotion || flag == knight_promo_capture) {
            move_score += 1500;
        }
        if (flag == bishop_promotion || flag == bishop_promo_capture) {
            move_score += 1000;
        }

        // Reward castling
        if (flag == king_castle) {
            move_score += 500;
        }
        if (flag == queen_castle) {
            move_score += 450;
        }

        move_scores.push_back({move_score, move});
    }

    int size = move_scores.size();
    merge_sort(move_scores, 0, size - 1);

    for (MoveScore move_score : move_scores) {
        result.push_back(move_score.move);
    }

    return result;
}

u64 Bot::search_move(ChessLogic &logic, int depth, int color) {
    std::vector<u64> moves = MoveGenerator::generate_legal_moves(logic);
    u64 best_move;
    int best_score = -INF;

    for (u64 move : moves) {
        logic.make_move(move);
        int score = -negamax(logic, depth - 1, -color, -INF, INF);
        logic.unmake_move(move);
        if (score >= best_score) {
            best_score = score;
            best_move = move;
        }
    }
    std::cout << print_pos(get_from(best_move)) << print_pos(get_to(best_move)) << "\n";
    return best_move;
}