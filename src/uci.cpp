#include "include/uci.h"

void run_uci() {
    Bot bot;
    ChessLogic logic;
    logic.load_pos(STARTPOS);
    MoveGenerator::init();

    std::string line;
    while (std::getline(std::cin, line)) {
        if (!line.compare("quit")) {
            return;
        } else if (!line.compare("uci")) {
            std::cout << "id name YellowEngine\n";
            std::cout << "id author Taemur Baig\n";
            std::cout << "uciok\n";
        } else if (line.rfind("position", 0) == 0) {
            int fen_start;
            std::string fen_part;

            if (line.find("startpos") != std::string::npos) {
                logic.load_pos(STARTPOS);

                fen_start = line.find("startpos") + 9;
                fen_part = line.substr(fen_start);

                int moves_index = fen_part.find("moves");
                std::string moves_string;

                // Get moves if they are defined
                if (moves_index != std::string::npos) {
                    moves_string = fen_part.substr(moves_index + 6);
                }

                if (!moves_string.empty()) {
                    std::istringstream iss(moves_string);
                    std::string move_str;
                    while (iss >> move_str) {
                        int from, to;
                        u16 move;
                        get_pos(move_str, &from, &to);
                        std::vector<u16> moves = MoveGenerator::generate_legal_moves(logic);
                        if (contains_move(moves, from, to, &move)) {
                            logic.make_move(move);
                        } else {
                            std::cout << "Invalid move sequence\n";
                            logic.load_pos(STARTPOS);
                            break;
                        }
                    }
                }
            } else if (line.find("fen") != std::string::npos) {
                fen_start = line.find("fen") + 4;
                fen_part = line.substr(fen_start);

                int moves_index = fen_part.find("moves");
                std::string fen_string;
                std::string moves_string;

                // Get moves if they are defined
                if (moves_index != std::string::npos) {
                    fen_string = fen_part.substr(0, moves_index - 1);
                    moves_string = fen_part.substr(moves_index + 6);
                } else {
                    fen_string = fen_part;
                }

                logic.load_pos(fen_string);

                if (!moves_string.empty()) {
                    std::istringstream iss(moves_string);
                    std::string move_str;
                    while (iss >> move_str) {
                        int from, to;
                        u16 move;
                        get_pos(move_str, &from, &to);
                        std::vector<u16> moves = MoveGenerator::generate_legal_moves(logic);
                        if (contains_move(moves, from, to, &move)) {
                            logic.make_move(move);
                        } else {
                            std::cout << "Invalid move sequence\n";
                            logic.load_pos(fen_string);
                            break;
                        }
                    }
                }
            } else {
                std::cout << "Invalid position command (expected 'startpos' or 'fen')\n";
            }
        } else if (!line.compare("isready")) {
            std::cout << "readyok\n";
        } else if (!line.compare("ucinewgame")) {
            bot.clear_tt();
        } else if (!line.compare("d")) {
            logic.draw_game();
        } else if (line.rfind("go", 0) == 0) {
            u16 best_move = bot.choose_move(logic);
            std::string uci_move = print_pos(get_from(best_move)) + print_pos(get_to(best_move));
            std::cout << "bestmove " << uci_move << "\n";
        }
    }
}

int main(void) {
    run_uci();
    return 0;
}