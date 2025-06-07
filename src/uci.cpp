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
    auto apply_list = [&](const std::string &moves_list, const std::string &base_fen) -> bool {
        // (re)load the base position so we can reset on error
        logic.load_pos(base_fen);

        std::istringstream iss(moves_list);
        std::string token;
        while (iss >> token) {
            // Extract promotion if present
            std::string mv = token;
            char promo = '\0';
            if (mv.size() == 5) {
                promo = mv[4]; // 'q', 'r', 'b' or 'n'
                mv = mv.substr(0, 4);
            } else if (mv.size() != 4) {
                std::cout << "Invalid move sequence: " << token << "\n";
                logic.load_pos(base_fen);
                return false;
            }

            int from, to;
            get_pos(mv, &from, &to);

            // Find a legal move that matches from/to and promotion
            auto legal = MoveGenerator::generate_legal_moves(logic);
            bool applied = false;
            for (u16 m : legal) {
                if ((int)get_from(m) == from && (int)get_to(m) == to) {
                    // check promotion bit
                    char this_promo = get_promotion_symbol(get_flag(m));
                    if (this_promo == promo) {
                        logic.make_move(m);
                        applied = true;
                        break;
                    }
                }
            }

            if (!applied) {
                std::cout << "Invalid move sequence: " << token << "\n";
                logic.load_pos(base_fen);
                return false;
            }
        }
        return true;
    };

    // CASE A: startpos
    if (position.rfind("startpos", 0) == 0) {
        // anything after "startpos "?
        size_t after = 8; // strlen("startpos")
        if (position.size() > after && position[after] == ' ')
            ++after;
        std::string rest = (after < position.size() ? position.substr(after) : "");
        size_t mvpos = rest.find("moves ");
        if (mvpos == std::string::npos) {
            logic.load_pos(STARTPOS);
            return;
        }
        std::string moves_list = rest.substr(mvpos + 6);
        apply_list(moves_list, STARTPOS);
        return;
    }

    // CASE B: fen
    size_t pf = position.find("fen ");
    if (pf != std::string::npos) {
        size_t after = pf + 4;
        if (after >= position.size()) {
            std::cout << "Incomplete 'position fen' command\n";
            return;
        }
        std::string rest = position.substr(after);
        size_t mvpos = rest.find(" moves ");
        std::string fen = (mvpos == std::string::npos ? rest : rest.substr(0, mvpos));
        std::string moves_list = (mvpos == std::string::npos ? "" : rest.substr(mvpos + 7));
        // load the FEN first
        logic.load_pos(fen);
        if (!moves_list.empty()) {
            apply_list(moves_list, fen);
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
            std::cout << "\n" << std::flush;
            return;
        } else if (!line.compare("stop")) {
            if (bot.on_uci_stop())
                bot.print_best_move();
        } else if (!line.compare("uci")) {
            std::cout << "id name YellowEngine\n" << std::flush;
            std::cout << "id author Taemur Baig\n" << std::flush;
            std::cout << "uciok\n" << std::flush;
        } else if (line.rfind("position", 0) == 0) {
            std::string position = line.substr(9);
            handle_position(logic, position);
        } else if (!line.compare("isready")) {
            std::cout << "readyok\n" << std::flush;
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
                params.winc = get_uci_param(command, "winc");
            }

            // Check for "binc <x>"
            if (command.find("binc") != std::string::npos) {
                params.binc = get_uci_param(command, "binc");
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
            } else if (params.movetime > 0) {
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