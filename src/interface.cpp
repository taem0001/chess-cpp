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
        turn = game.get_turn();
        std::vector<u16> moves = MoveGenerator::generate_legal_moves(game);

        if (moves.size() == 0) {
            break;
        }

        print_moves(moves);
        std::cout << moves.size() << std::endl;

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

        std::map<u16, std::string> fens = game.get_fens();
        for (const auto& pair : fens) {
            std::cout << print_pos((int)get_from(pair.first)) << print_pos((int)get_to(pair.first)) << ": " << pair.second << std::endl;
        }
    }
    player = turn ? "White player in checkmate" : "Black player in checkmate";
    std::cout << player << std::endl;
}
