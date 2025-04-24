#ifndef BOT_H
#define BOT_H
#include "utils.h"
#include "chesslogic.h"
#include "movegen.h"

class Bot {
private:
    int evaluate(ChessLogic &);
    int search(ChessLogic &, int, int, int);

public:
    Bot();
    u32 choose_move(ChessLogic, std::vector<u32> &);
};

#endif // !BOT_H