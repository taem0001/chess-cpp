#include "../include/bot.h"

u32 Bot::choose_move(std::vector<u32> &moves) {
    int rand = std::rand() % moves.size();
    return moves.at(rand);
}