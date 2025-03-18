#include "../include/interface.h"

Interface::Interface() : game() { MoveGenerator::init(); }

void Interface::run_game() {
    bool turn;
    bool correct_pos;
    bool valid_move;
    int from, to;
    u16 move;
    std::string player, pos;

    game.draw_game();

    while (true) {
        std::cout << FenHandler::write_fen(game) << std::endl;
        std::vector<u16> moves =
            MoveGenerator::generate_pseudolegal_moves(game);

        turn = game.get_turn();

        do {
            player = turn ? "White player's move: " : "Black player's move: ";
            std::cout << player;
            std::cin >> pos;
            correct_pos = get_pos(pos, &from, &to);
            valid_move = contains_move(moves, from, to, &move);
        } while (!correct_pos || !valid_move);

        game.make_move(move);
        game.draw_game();
        game.change_turn();
    }
}
