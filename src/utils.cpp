#include "../include/utils.h"

std::string evaluate_fen_char(int &spacing, char piece) {
    std::string res = "";
    if (spacing > 0) {
        res += std::to_string(spacing);
        spacing = 0;
    }
    res += piece;
    return res;
}
