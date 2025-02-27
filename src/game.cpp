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
    fens.push_back(board->write_fen());
    std::cout << board->write_fen() << std::endl;
}

void Game::run_game() {
    bool running = true;
    Move move;
    bool move_made;
    std::string pos;

    board->draw_board();

    while (running) {
        add_fen();
        std::vector<Move> moves;
        moves = board->generate_legal_moves();

        const char *player = board->get_turn() ? "White player's move: " : "Black player's move: ";
        std::cout << player << std::endl;

        do {
            std::cin >> pos;
            move = board->convert_pos(pos);
            move_made = make_move(moves, move.start_square, move.end_square);
        } while (!move_made);

        std::cout << "Move made!" << std::endl;

        board->draw_board();
    }

    std::cout << "Game ended!" << std::endl;
}
