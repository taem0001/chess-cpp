#include "../include/bot.h"

Bot::Bot() {
    MoveGenerator::init();
}

u64 Bot::choose_move(ChessLogic logic, std::vector<u64> &moves) {
    int color = logic.get_turn() ? 1 : -1;
    return search_move(logic, 4, color);
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

    return 9 * q_diff + 5 * r_diff + 3 * b_diff + 3 * n_diff + 1 * p_diff;
}

int Bot::negamax(ChessLogic &logic, int depth, int color, int alpha, int beta) {
    if (depth == 0) {
        return color * evaluate(logic);
    }

    std::vector<u64> moves = MoveGenerator::generate_legal_moves(logic);
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
    return best_move;
}