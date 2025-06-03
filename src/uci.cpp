#include "include/perft.h"
#include "include/utils.h"
#include "include/movegen.h"
#include "include/chesslogic.h"
#include "include/bot.h"

int get_uci_param(const std::string &command, const std::string &param) {
    int pos = command.find(param);
    int num_start = pos + param.length();
    while (num_start < command.size() && command[num_start] == ' ')
        ++num_start;

    int result = std::stoi(command.substr(num_start));

    return result;
}

void handle_position(ChessLogic &logic, const std::string &position) {
    size_t p = position.find("startpos");
    if (p != std::string::npos) {
        logic.load_pos(STARTPOS);

        size_t after = p + strlen("startpos"); //  = p + 8
        if (after >= position.size()) {
            return;
        }
        if (position[after] == ' ')
            ++after;

        std::string fen_part = position.substr(after);
        size_t mvpos = fen_part.find("moves");
        if (mvpos == std::string::npos) {
            return;
        }

        size_t start_of_list = mvpos + strlen("moves ");
        std::string moves_list = fen_part.substr(start_of_list);

        std::istringstream iss(moves_list);
        std::string mv;
        while (iss >> mv) {
            int from, to;
            u16 move;
            get_pos(mv, &from, &to);
            auto legal = MoveGenerator::generate_legal_moves(logic);
            if (contains_move(legal, from, to, &move)) {
                logic.make_move(move);
            } else {
                std::cout << "Invalid move sequence: " << mv << "\n";
                logic.load_pos(STARTPOS);
                return;
            }
        }
        return;
    }

    size_t pf = position.find("fen");
    if (pf != std::string::npos) {
        size_t i = pf + strlen("fen ");
        if (i >= position.size()) {
            std::cout << "Incomplete 'position fen' command\n";
            return;
        }

        std::string after_fen = position.substr(i);
        size_t m = after_fen.find(" moves ");
        std::string fen_string, moves_string;
        if (m != std::string::npos) {
            fen_string   = after_fen.substr(0, m);
            moves_string = after_fen.substr(m + strlen(" moves "));
        } else {
            fen_string = after_fen;
        }

        logic.load_pos(fen_string);

        if (!moves_string.empty()) {
            std::istringstream iss(moves_string);
            std::string mv;
            while (iss >> mv) {
                int from, to;
                u16 move;
                get_pos(mv, &from, &to);
                auto legal = MoveGenerator::generate_legal_moves(logic);
                if (contains_move(legal, from, to, &move)) {
                    logic.make_move(move);
                } else {
                    std::cout << "Invalid move: " << mv << "\n";
                    logic.load_pos(fen_string);
                    return;
                }
            }
        }
        return;
    }

    std::cout << "Invalid position command "
              << "(expected 'position startpos' or 'position fen …')\n";
}

void run_uci() {
    Bot bot;
    ChessLogic logic;
    logic.load_pos(STARTPOS);
    MoveGenerator::init();

    std::string line;
    while (std::getline(std::cin, line)) {
        if (!line.compare("quit")) {
            bot.on_uci_stop();
            std::cout << "\n";
            return;
        } else if (!line.compare("stop")) {
            if (bot.on_uci_stop()) 
                bot.print_best_move();
        } else if (!line.compare("uci")) {
            std::cout << "id name YellowEngine\n";
            std::cout << "id author Taemur Baig\n";
            std::cout << "uciok\n";
        } else if (line.rfind("position", 0) == 0) {
            std::string position = line.substr(9);
            handle_position(logic, position);
        } else if (!line.compare("isready")) {
            std::cout << "readyok\n";
        } else if (!line.compare("ucinewgame")) {
            bot.clear_tt();
        } else if (!line.compare("d")) {
            logic.draw_game();
        } else if (line.rfind("go", 0) == 0) {
            std::string command = line.substr(3);
            if (command.find("perft") != std::string::npos) {
                int depth_start = command.find("perft") + 6;
                int depth = std::stoi(command.substr(depth_start));
                divide_perft(logic, depth);
            } else if (command.find("infinite") != std::string::npos) {
                UCIGoParams params = {true, -1, -1, -1, -1, -1};

                int color = logic.get_turn() ? 1 : -1;
                bot.start_search(logic, color, params);
            } else if (command.find("depth") != std::string::npos) {
                UCIGoParams params = {false, -1, -1, -1, -1, -1};

                int limit = get_uci_param(command, "depth");
                int color = logic.get_turn() ? 1 : -1;

                params.depth = limit;
                bot.start_search(logic, color, params);
            } else if (command.find("movetime") != std::string::npos) {
                UCIGoParams params = {false, -1, -1, -1, -1, -1};

                int time = get_uci_param(command, "movetime");
                int color = logic.get_turn() ? 1 : -1;

                params.movetime = time;
                bot.start_search(logic, color, params);
            }
        }
    }
}

int main(void) {
    run_uci();
    return 0;
}