#include "../include/bot.h"

u32 Bot::choose_move(std::vector<u32> &moves) {
    std::srand(std::time(0));
    int rand = std::rand() % moves.size();
    return moves.at(rand);
}