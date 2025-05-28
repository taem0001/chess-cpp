#include "../include/bot.h"

Bot::Bot() : tt_table(TABLE_SIZE), nodes_searched(0), tt_hits(0) { MoveGenerator::init(); }

u16 Bot::choose_move(ChessLogic logic, std::vector<u16> &moves) {
    int color = logic.get_turn() ? 1 : -1;
    return search_move(logic, 1000, color);
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
    nodes_searched++;
    int alpha_orig = alpha;

    // Check if position is in transposition table
    u64 key = logic.get_zobrist_hash();
    TTEntry entry;
    if (probe_tt(key, entry) && entry.depth >= depth) {
        if (entry.flag == EXACT)
            return entry.score;
        if (entry.flag == LOWERBOUND && entry.score >= beta)
            return entry.score;
        if (entry.flag == UPPERBOUND && entry.score <= alpha)
            return entry.score;
    }

    if (depth == 0)
        return quiescence(logic, alpha, beta, color);

    std::vector<u16> moves = order_moves(logic, MoveGenerator::generate_legal_moves(logic));
    if (moves.size() == 0) {
        if (logic.get_singlecheck() || logic.get_doublecheck()) {
            return -INF + depth;
        }
        return 0;
    }

    int best_score = -INF;
    u16 best_move = 0;

    for (u64 move : moves) {
        logic.make_move(move);
        int score = -negamax(logic, depth - 1, -color, -beta, -alpha);
        logic.unmake_move(move);

        if (score > best_score) {
            best_score = score;
            best_move = move;
        }

        alpha = max(alpha, score);
        if (alpha >= beta)
            break;
    }

    u8 flag;
    if (best_score <= alpha_orig)
        flag = UPPERBOUND;
    else if (best_score >= beta)
        flag = LOWERBOUND;
    else
        flag = EXACT;

    store_tt(key, best_score, depth, flag, best_move);

    return best_score;
}

int Bot::quiescence(ChessLogic &logic, int alpha, int beta, int color) {
    nodes_searched++;
    
    u64 key = logic.get_zobrist_hash();
    TTEntry entry;

    if (probe_tt(key, entry)) {
        if (entry.flag == EXACT)
            return entry.score;
        if (entry.flag == LOWERBOUND && entry.score >= beta)
            return entry.score;
        if (entry.flag == UPPERBOUND && entry.score <= alpha)
            return entry.score;
    }
    int static_eval = color * evaluate(logic);

    int best_val = static_eval;
    if (best_val >= beta)
        return best_val;
    if (best_val > alpha)
        alpha = best_val;

    std::vector<u16> moves = MoveGenerator::generate_legal_moves(logic);
    only_noisy_moves(moves);

    for (u16 move : moves) {
        logic.make_move(move);
        int score = -quiescence(logic, -beta, -alpha, -color);
        logic.unmake_move(move);

        if (score >= beta)
            return score;
        if (score > best_val)
            best_val = score;
        if (score > alpha)
            alpha = score;
    }

    store_tt(key, best_val, 0, EXACT, 0);

    return best_val;
}

std::vector<u16> Bot::order_moves(ChessLogic &logic, std::vector<u16> moves) {
    std::vector<MoveScore> move_scores;
    std::vector<u16> result;
    u64 *bitboards = logic.get_board().get_bitboards();

    u64 key = logic.get_zobrist_hash();
    TTEntry entry;
    u16 tt_move = 0;

    if (probe_tt(key, entry)) {
        tt_move = entry.best_move;
    }

    for (u16 move : moves) {
        int move_score = 0;
        int from = (int)get_from(move);
        int to = (int)get_to(move);
        int flag = (int)get_flag(move);

        // If move is found from transposition table reward it
        if (tt_move == move) {
            move_score += 10000;
        }

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

u16 Bot::search_move(ChessLogic &logic, int thinktime, int color) {
    using namespace std::chrono;

    auto start = steady_clock::now();
    u16 best_move = 0;
    int best_score = -INF;
    int depth = 1;

    const int time_buffer = thinktime * 95 / 100;

    while (true) {
        nodes_searched = 0;

        auto now = steady_clock::now();
        int elapsed = duration_cast<milliseconds>(now - start).count();
        if (elapsed >= time_buffer) {
            break;
        }

        std::vector<u16> moves = order_moves(logic, MoveGenerator::generate_legal_moves(logic));

        int current_best_score = -INF;
        u16 current_best_move = 0;
        bool completed = true;

        for (u16 move : moves) {
            now = steady_clock::now();
            int elapsed = duration_cast<milliseconds>(now - start).count();
            if (elapsed >= time_buffer) {
                completed = false;
                break;
            }

            logic.make_move(move);
            int score = -negamax(logic, depth - 1, -color, -INF, INF);
            logic.unmake_move(move);

            if (score > current_best_score) {
                current_best_score = score;
                current_best_move = move;
            }
        }

        if (completed) {
            best_move = current_best_move;
            best_score = current_best_score;
            depth++;
        } else {
            break;
        }
    }

    std::cout << "Depth searched: " << depth << "\n";
    std::cout << "Nodes searched: " << nodes_searched << "\n";
    std::cout << "TT hits: " << tt_hits << "\n";
    return best_move;
}

bool Bot::probe_tt(u64 key, TTEntry &entry_out) {
    size_t index = key % TABLE_SIZE;
    TTEntry &entry = tt_table[index];
    if (entry.key == key) {
        tt_hits++;
        entry_out = entry;
        return true;
    }
    return false;
}

void Bot::store_tt(u64 key, int score, int depth, u8 flag, u16 best_move) {
    size_t index = key % TABLE_SIZE;
    TTEntry &entry = tt_table[index];

    bool replace = false;
    if (entry.key != key)
        replace = true;
    else if (depth > entry.depth)
        replace = true;
    else if (depth == entry.depth && flag == EXACT && entry.flag != EXACT)
        replace = true;

    if (replace) {
        entry.key = key;
        entry.score = score;
        entry.depth = depth;
        entry.flag = flag;
        entry.best_move = best_move;
    }
}

void Bot::clear_tt() {
    tt_table.clear();
}