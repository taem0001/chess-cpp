#include "../include/bot.h"

Bot::Bot() {
    MoveGenerator::init();
}

u64 Bot::choose_move(ChessLogic logic, std::vector<u64> &moves) {
    return search_move(logic, 4);
}

int Bot::evaluate(ChessLogic &logic) {
    u64 *bitboards = logic.get_board().get_bitboards();
    bool turn = logic.get_turn();

    u64 q_f = bitboards[turn ? WHITE_QUEEN : BLACK_QUEEN];
    u64 q_op = bitboards[turn ? BLACK_QUEEN : WHITE_QUEEN];
    u64 r_f = bitboards[turn ? WHITE_ROOK : BLACK_ROOK];
    u64 r_op = bitboards[turn ? BLACK_ROOK : WHITE_ROOK]; 
    u64 b_f = bitboards[turn ? WHITE_BISHOP : BLACK_BISHOP];
    u64 b_op = bitboards[turn ? BLACK_BISHOP : WHITE_BISHOP];
    u64 n_f = bitboards[turn ? WHITE_KNIGHT : BLACK_KNIGHT];
    u64 n_op = bitboards[turn ? BLACK_KNIGHT : WHITE_KNIGHT];
    u64 p_f = bitboards[turn ? WHITE_PAWN : BLACK_PAWN];
    u64 p_op = bitboards[turn ? BLACK_PAWN : WHITE_PAWN];

    int q_diff = __popcnt64(q_f) - __popcnt64(q_op);
    int r_diff = __popcnt64(r_f) - __popcnt64(r_op);    
    int b_diff = __popcnt64(b_f) - __popcnt64(b_op);
    int n_diff = __popcnt64(n_f) - __popcnt64(n_op);
    int p_diff = __popcnt64(p_f) - __popcnt64(p_op);

    int eval = 9 * q_diff + 5 * r_diff + 3 * b_diff + 3 * n_diff + 1 * p_diff;
    return eval;
}

int Bot::search(ChessLogic &logic, int depth) {
    if (depth == 0) {
        return evaluate(logic);
    }

    std::vector<u64> moves = MoveGenerator::generate_legal_moves(logic);
    if (moves.size() == 0) {
        if (logic.get_singlecheck() || logic.get_doublecheck()) {
            return -INF + depth;
        }
        return 0;
    }

    int result = -INF;

    for (u64 move : moves) {
        logic.make_move(move);
        int eval = -search(logic, depth - 1);
        logic.unmake_move(move);
        result = max(result, eval);
    }
    return result;
}

u64 Bot::search_move(ChessLogic &logic, int depth) {
    std::vector<u64> moves = MoveGenerator::generate_legal_moves(logic);
    u64 best_move;
    int best_eval = -INF;

    for (u64 move : moves) {
        logic.make_move(move);
        int eval = search(logic, depth - 1);
        logic.unmake_move(move);
        if (eval >= best_eval) {
            best_eval = eval;
            best_move = move;
        }
    }
    std::cout << best_eval << std::endl;
    return best_move;
}