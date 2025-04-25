#ifndef BOT_H
#define BOT_H
#include "utils.h"
#include "chesslogic.h"
#include "movegen.h"

class Bot {
private:
    int evaluate(ChessLogic &);
    int negamax(ChessLogic &, int, int, int, int);
    u64 search_move(ChessLogic &, int, int);

public:
    Bot();
    u64 choose_move(ChessLogic, std::vector<u64> &);
};

#endif // !BOT_H