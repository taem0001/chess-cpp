#include "../include/uci.h"

void run_uci() {
    Bot bot;
    ChessLogic logic;

    std::string line;
    while (std::getline(std::cin, line)) {
        if (!line.compare("quit")) {
            return;
        } else if (!line.compare("uci")) {
            std::cout << "id name YellowEngine\n";
            std::cout << "id author Taemur Baig\n";
        }
    }
}

int main(void) {
    run_uci();
    return 0;
}