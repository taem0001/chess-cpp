#ifndef FEN_H
#define FEN_H
#include "chessgame.h"

class FenHandler {
public:
    static void load_fen(ChessGame &, const std::string &);
};

#endif // !FEN_H
