#include "../include/game.h"
#include <iostream>

Game::Game() {
    board = new Board();
    board->setup_board();
}

Game::~Game() {
    delete board;
}

bool Game::make_move(std::vector<Move> &moves, int s, int e) {
    if (board->is_move_legal(moves, s, e)) {
        board->move_piece(s, e);
        board->change_turn();
        return true;
    }

    return false;
}

Board *Game::get_board() {
    return board;
}

void Game::add_fen() {

}

void Game::run_game() {
    bool running = true;
    int s_row, s_col, e_row, e_col;
    int s_index, e_index;
    bool move_made;

    board->draw_board();

    while (running) {
        std::vector<Move> moves;
        moves = board->generate_legal_moves();

        for (Move move : moves) {
            std::cout << "{" << move.start_square << "," << move.end_square << "} ";
        }
        std::cout << std::endl;

        const char *player = board->get_turn() ? "White player's move: " : "Black player's move: ";
        std::cout << player << std::endl;

        do {
            std::cin >> s_row >> s_col;
            std::cin >> e_row >> e_col;
            s_row--, s_col--, e_row--, e_col--;
            s_index = s_row * 8 + s_col;
            e_index = e_row * 8 + e_col;
            move_made = make_move(moves, s_index, e_index);
        } while (!move_made);

        std::cout << "Move made!" << std::endl;

        board->draw_board();
    }

    std::cout << "Game ended!" << std::endl;
}
