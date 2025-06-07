#include "../include/perft.h"

u64 perft(ChessLogic &logic, int depth) {
    auto moves = MoveGenerator::generate_legal_moves(logic);

    if (depth == 1) {
        return moves.size();
    }
    if (depth == 0) {
        return 1;
    }

    u64 total_nodes = 0;
    for (u16 move : moves) {
        logic.make_move(move);
        u64 nodes = perft(logic, depth - 1);
        logic.unmake_move(move);
        total_nodes += nodes;
    }
    return total_nodes;
}

void divide_perft(ChessLogic logic, int depth) {
    auto moves = MoveGenerator::generate_legal_moves(logic);
    u64 total_nodes = 0;

    for (u16 move : moves) {
        logic.make_move(move);
        u64 nodes = perft(logic, depth - 1);
        logic.unmake_move(move);

        std::string move_str = print_pos((int)get_from(move)) + print_pos((int)get_to(move));
        std::cout << move_str << ": " << nodes << "\n" << std::flush;
        total_nodes += nodes;
    }
    std::cout << "\nNodes searched: " << total_nodes << "\n" << std::flush;
    std::cout << "\n" << std::flush;
}