#ifndef INTERFACE_H
#define INTERFACE_H
#include "../include/chessgame.h"
#include "../include/movegen.h"

class Interface {
private:
    ChessGame game;

public:
    Interface();
    void run_game();
};

#endif // !INTERFACE_H
