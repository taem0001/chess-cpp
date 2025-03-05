#include "../include/game.h"
#include "../include/fen.h"
#include "../include/move.h"
#include <iostream>

Game::Game() : board() {
    board.setup_board();
}

bool Game::make_move(std::vector<Move> &moves, int s, int e) {
    if (board.is_move_legal(moves, s, e)) {
        board.move_piece(s, e);
        board.change_turn();
        return true;
    }

    return false;
}

Board &Game::get_board() {
    return board;
}

void Game::run_game() {
    bool running = true;
    Move move;
    bool move_made;
    std::string pos;

    board.draw_board();

    while (running) {
        std::string fen = FenHandler::write_fen(board);
        fens.push_back(fen);
        std::cout << fen << std::endl;

        std::vector<Move> moves;
        moves = MoveGenerator::generate_legal_moves(board);
        MoveGenerator::handle_check(board, moves);

        const std::string player = board.get_turn() ? "White player's move: " : "Black player's move: ";
        std::cout << player << std::endl;

        do {
            std::cin >> pos;
            move = convert_pos(pos);
            move_made = make_move(moves, move.start_square, move.end_square);
        } while (!move_made);

        board.draw_board();
    }

    std::cout << "Game ended!" << std::endl;
}
