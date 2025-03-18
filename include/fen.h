#ifndef FEN_H
#define FEN_H
#include "chessgame.h"

class FenHandler {
public:
    static void load_fen(ChessGame &, const std::string &);
    static std::string write_fen(ChessGame &);
};

#endif // !FEN_H
