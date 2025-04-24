#include "../include/chessgame.h"

ChessGame::ChessGame() : logic(), running(true), draw(false), winner(false) { MoveGenerator::init(); }

void ChessGame::run_game(Bot &bot, bool bot_color) {
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
        } else if (moves.size() == 0 && (logic.get_singlecheck() || logic.get_doublecheck())) {
            winner = !turn;
            break;
        }

        if (bot_color == turn) {
            move = bot.choose_move(logic, moves);
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

void ChessGame::run_game_bot(Bot &bot1, Bot &bot2) {
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
            std::cout << "STALEMATE" << std::endl;
            draw = true;
            break;
        } else if (five_fold) {
            std::cout << "FIVEFOLD" << std::endl;
            draw = true;
            break;
        } else if (logic.fifty_move_rule()) {
            std::cout << "FIFTYMOVES" << std::endl;
            draw = true;
            break;
        } else if (moves.size() == 0 && (logic.get_singlecheck() || logic.get_doublecheck())) {
            winner = !turn;
            break;
        } else if (moves.size() == 0) {
            std::cout << "DEADPOS" << std::endl;
            draw = true;
            break;
        }

        if (turn) {
            move = bot1.choose_move(logic, moves);
        } else {
            move = bot2.choose_move(logic, moves);
        }

        std::cout << logic.get_halfmoves() << std::endl;

        logic.make_move(move);
        logic.draw_game();
    }
}

bool ChessGame::get_draw() { return draw; }
bool ChessGame::get_winner() { return winner; }