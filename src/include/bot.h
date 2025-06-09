#ifndef BOT_H
#define BOT_H
#include "utils.h"
#include "chesslogic.h"
#include "movegen.h"
#include "bitboard.h"

class Bot {
private:
    std::vector<TTEntry> tt_table;

    std::atomic<bool> stop_search;
    std::mutex stop_mtx;
    std::condition_variable stop_cv;
    std::thread search_thread;
    bool searching;
    u16 result_move;
    int current_depth;
    
    void search_worker(ChessLogic, int);
    void search_worker_to_depth(ChessLogic, int, int);

    bool probe_tt(u64, TTEntry &);
    void store_tt(u64, int, int, u8, u16);
    int evaluate(ChessLogic &);
    int negamax(ChessLogic &, int, int, int, int);
    int quiescence(ChessLogic &, int, int, int);
    int evaluate_passed_pawns(u64, u64);
    std::vector<u16> order_moves(ChessLogic &, std::vector<u16>);

public:
    Bot();
    u16 get_result_move() const;
    void start_search(ChessLogic &, int, const UCIGoParams &);
    bool on_uci_stop();
    void clear_tt();
    void print_best_move() const;
};

#endif // !BOT_H