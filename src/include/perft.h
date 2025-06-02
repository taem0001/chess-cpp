#ifndef PERFT_H
#define PERFT_H
#include "chesslogic.h"
#include "movegen.h"

u64 perft(ChessLogic &, int);
void divide_perft(ChessLogic, int);

#endif // !PERFT_H