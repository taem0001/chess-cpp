#ifndef BOT_H
#define BOT_H
#include "utils.h"
#include "chesslogic.h"
#include "movegen.h"

class Bot {
private:
    std::vector<TTEntry> tt_table;

    int evaluate(ChessLogic &);
    int negamax(ChessLogic &, int, int, int, int);
    u16 search_move(ChessLogic &, int, int);
    std::vector<u16> order_moves(ChessLogic &, std::vector<u16>);

public:
    Bot();
    u16 choose_move(ChessLogic, std::vector<u16> &);
};

#endif // !BOT_H