#ifndef FEN_H
#define FEN_H
#include "board.h"

class FenHandler {
public:
    static void load_fen(Board &, const std::string &);
};

#endif // !FEN_H
