#include "include/bot.h"
#include "include/chesslogic.h"
#include "include/movegen.h"
#include "include/perft.h"
#include "include/utils.h"

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
            fen_string = after_fen.substr(0, m);
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
            UCIGoParams params = {};

            // Check for "infinite"
            if (command.find("infinite") != std::string::npos) {
                params.infinite = true;
            }

            // Check for "depth <x>"
            if (command.find("depth") != std::string::npos) {
                params.depth = get_uci_param(command, "depth");
            }

            // Check for "movetime <x>"
            if (command.find("movetime") != std::string::npos) {
                params.movetime = get_uci_param(command, "movetime");
            }

            // Check for "wtime <x>"
            if (command.find("wtime") != std::string::npos) {
                params.wtime = get_uci_param(command, "wtime");
            }

            // Check for "btime <x>"
            if (command.find("btime") != std::string::npos) {
                params.btime = get_uci_param(command, "btime");
            }

            // Check for "winc <x>"
            if (command.find("winc") != std::string::npos) {
                params.wtime = get_uci_param(command, "winc");
            }

            // Check for "binc <x>"
            if (command.find("binc") != std::string::npos) {
                params.btime = get_uci_param(command, "binc");
            }

            // Check for "movestogo <x>"
            if (command.find("movestogo") != std::string::npos) {
                params.movestogo = get_uci_param(command, "movestogo");
            }

            // Check for "perft <x>"
            if (command.find("perft") != std::string::npos) {
                int depth = get_uci_param(command, "perft");
                divide_perft(logic, depth);
            }

            // Whose turn it is
            int color = logic.get_turn() ? 1 : -1;

            /*
             * Decide search mode. Prioritize:
             * a) depth
             * b) movetime
             * c) wtime/btime
             * d) infinite
             */
            if (params.depth > 0) {
                bot.start_search(logic, color, params);
            } else if (params.wtime > 0 || params.btime > 0) {
                int remaining = (color == 1) ? params.wtime : params.btime;
                int increment = (color == 1) ? params.winc : params.binc;
                int moves_left = params.movestogo > 0 ? params.movestogo : 30;

                int est = (remaining / moves_left) + increment - 50;
                if (est < 1)
                    est = 1;
                
                params.movetime = est;
                bot.start_search(logic, color, params);
            } else if (params.infinite) {
                bot.start_search(logic, color, params);
            }
        }
    }
}

int main(void) {
    run_uci();
    return 0;
}