#ifndef INTERFACE_H
#define INTERFACE_H
#include "../include/chessgame.h"

class Interface {
private:
    ChessGame game;

public:
    Interface();
    void run_game();
};

#endif // !INTERFACE_H
