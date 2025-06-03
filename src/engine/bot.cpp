#include "../include/bot.h"

Bot::Bot() : stop_search(false), searching(false), result_move(0), current_depth(0), tt_table(TABLE_SIZE) { MoveGenerator::init(); }

void Bot::on_uci_stop() {
    if (!searching) return;

    {
        std::lock_guard<std::mutex> lk(stop_mtx);
        stop_search.store(true, std::memory_order_relaxed);
    }
    stop_cv.notify_one();

    if (search_thread.joinable()) {
        search_thread.join();
    }

    searching = false;
}

u16 Bot::get_result_move() const {
    return result_move;
}

void Bot::search_worker(ChessLogic logic, int color) {
    int depth = 1;
    u16 best_move = 0;

    while (!stop_search.load(std::memory_order_relaxed)) {
        auto moves = order_moves(logic, MoveGenerator::generate_legal_moves(logic));

        int current_best_score = -INF;
        u16 current_best_move  = 0;

        for (u16 move : moves) {
            if (stop_search.load(std::memory_order_relaxed)) {
                return;
            }

            logic.make_move(move);
            int score = -negamax(logic, depth - 1, -color, -INF, INF);
            logic.unmake_move(move);

            if (score > current_best_score) {
                current_best_score = score;
                current_best_move  = move;
            }
        }

        best_move = current_best_move;
        depth++;

        result_move = best_move;
        current_depth = depth - 1;
    }
}

void Bot::search_worker_to_depth(ChessLogic logic, int color, int limit) {
    int depth = 1;
    u16 best_move = 0;

    while (!stop_search.load(std::memory_order_relaxed) && depth <= limit) {
        auto moves = order_moves(logic, MoveGenerator::generate_legal_moves(logic));

        int current_best_score = -INF;
        u16 current_best_move  = 0;

        for (u16 move : moves) {
            if (stop_search.load(std::memory_order_relaxed)) {
                return;
            }

            logic.make_move(move);
            int score = -negamax(logic, depth - 1, -color, -INF, INF);
            logic.unmake_move(move);

            if (score > current_best_score) {
                current_best_score = score;
                current_best_move  = move;
            }
        }

        best_move = current_best_move;
        depth++;

        result_move = best_move;
        current_depth = depth - 1;
    }
}

void Bot::start_search(ChessLogic &logic, int color, const UCIGoParams &params) {
    if (searching) return;

    stop_search.store(false, std::memory_order_relaxed);
    ChessLogic logic_copy = logic; 
    searching = true;
    
    if (params.depth != -1) {
        search_thread = std::thread(&Bot::search_worker_to_depth, this, std::move(logic_copy), color, params.depth);
    } else if (params.infinite) {
        search_thread = std::thread(&Bot::search_worker, this, std::move(logic_copy), color);
    }
}

int Bot::evaluate(ChessLogic &logic, int color) {
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

    int material_diff = queen_value * q_diff + rook_value * r_diff + bishop_value * b_diff + knight_value * n_diff +
                        pawn_value * p_diff;
    int sq_eval = 0;
    
    // Evaluate white pieces' position
    while (p_w) {
        int sq = first_bit(p_w);
        sq_eval += pawn_heatmap[sq] * 10;
        p_w &= p_w - 1;
    }
    
    while (n_w) {
        int sq = first_bit(n_w);
        sq_eval += knight_heatmap[sq] * 10;
        n_w &= n_w - 1;
    }

    while (b_w) {
        int sq = first_bit(b_w);
        sq_eval += bishop_heatmap[sq] * 10;
        b_w &= b_w - 1;
    }

    while (r_w) {
        int sq = first_bit(r_w);
        sq_eval += rook_heatmap[sq] * 10;
        r_w &= r_w - 1;
    }

    while (q_w) {
        int sq = first_bit(q_w);
        sq_eval += queen_heatmap[sq] * 10;
        q_w &= q_w - 1;
    }

    int king_sq = first_bit(bitboards[WHITE_KING]);
    int total_pieces = *logic.get_total_pieces();
    if (total_pieces > 18) {
        sq_eval += king_opening_heatmap[king_sq];
    } else {
        sq_eval += king_endgame_heatmap[king_sq];
    }

    // Evaluate black pieces' position
    while (p_b) {
        int sq = first_bit(p_b);
        int msq = sq ^ 56;
        sq_eval -= pawn_heatmap[msq] * 10;
        p_b &= p_b - 1;
    }

    while (n_b) {
        int sq = first_bit(n_b);
        int msq = sq ^ 56;
        sq_eval -= knight_heatmap[msq] * 10;
        n_b &= n_b - 1;
    }

    while (b_b) {
        int sq = first_bit(b_b);
        int msq = sq ^ 56;
        sq_eval -= bishop_heatmap[msq] * 10;
        b_b &= b_b - 1;
    }

    while (r_b) {
        int sq = first_bit(r_b);
        int msq = sq ^ 56;
        sq_eval -= rook_heatmap[msq] * 10;
        r_b &= r_b - 1;
    }

    while (q_b) {
        int sq = first_bit(q_b);
        int msq = sq ^ 56;
        sq_eval -= queen_heatmap[msq] * 10;
        q_b &= q_b - 1;
    }

    king_sq = first_bit(bitboards[BLACK_KING]) ^ 56;
    if (total_pieces > 18) {
        sq_eval -= king_opening_heatmap[king_sq];
    } else {
        sq_eval -= king_endgame_heatmap[king_sq];
    }

    int total_eval = material_diff + sq_eval;

    return total_eval * color;
}

int Bot::negamax(ChessLogic &logic, int depth, int color, int alpha, int beta) {
    if (stop_search)
        return 0;

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
    if (stop_search)
        return 0;


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
    int static_eval = evaluate(logic, color);

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

bool Bot::probe_tt(u64 key, TTEntry &entry_out) {
    size_t index = key % TABLE_SIZE;
    TTEntry &entry = tt_table[index];
    if (entry.key == key) {
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

void Bot::clear_tt() { tt_table.clear(); }