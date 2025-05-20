#ifndef FEN_H
#define FEN_H
#include "chesslogic.h"

class FenHandler {
public:
    static void load_fen(ChessLogic &, const std::string &);
    static std::string write_fen(ChessLogic &);
};

#endif // !FEN_H
