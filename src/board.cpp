#include "../include/board.h"
#include "../include/fen.h"

void Board::init() {
    FenHandler::load_fen(*this, STARTPOS);
}

u64 *Board::get_bitboards() {
    return bitboards;
}
