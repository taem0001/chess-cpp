#ifndef FEN_H
#define FEN_H

#include "board.h"

class FenHandler {
public:
    static void load_position(Board &, const std::string &);
    static std::string write_fen(Board &);
};

#endif // !FEN_H
