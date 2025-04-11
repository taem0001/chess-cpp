#include "../include/chessgame.h"

ChessGame::ChessGame() : logic(), bot(), running(true), draw(false), winner(false) { MoveGenerator::init(); }

void ChessGame::run_game() {
    bool turn;
    bool correct_pos;
    bool valid_move;
    bool five_fold;
    int from, to;
    u32 move;
    std::string player, pos;

    logic.draw_game();

    while (running) {
        five_fold = logic.fivefold_repitition();
        turn = logic.get_turn();
        std::vector<u32> moves = MoveGenerator::generate_legal_moves(logic);

        if (moves.size() == 0 && (!logic.get_singlecheck() && !logic.get_doublecheck())) {
            draw = true;
            break;
        } else if (five_fold) {
            draw = true;
            break;
        } else if (logic.fifty_move_rule()) {
            draw = true;
            break;
        } else if (moves.size() == 0 && (logic.get_singlecheck() && logic.get_doublecheck())) {
            winner = !turn;
            break;
        }

        if (bot_color == turn) {
            move = bot.choose_move(moves);
        } else {
            do {
                player = turn ? "White player's move: " : "Black player's move: ";
                std::cout << player;
                std::cin >> pos;
                correct_pos = get_pos(pos, &from, &to);
                valid_move = contains_move(moves, from, to, logic.get_totalmoves(), &move);
            } while (!correct_pos || !valid_move);
        }

        logic.make_move(move);
        logic.draw_game();
    }
}

void ChessGame::set_bot_color(bool b) { bot_color = b; }
bool ChessGame::get_draw() { return draw; }
bool ChessGame::get_winner() { return winner; }