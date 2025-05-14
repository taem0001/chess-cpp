#include "../include/chesslogic.h"
#include "../include/movegen.h"
#include "../include/utils.h"
#include <windows.h>
#include <iomanip>

std::string convert_position(const std::string &fen) {
    std::string res = "";
    if (fen.compare("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") == 0) {
        res = "POSITION: 1";
    }
    if (fen.compare("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1") == 0) {
        res = "POSITION: 2";
    }
    if (fen.compare("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1") == 0) {
        res = "POSITION: 3";
    }
    if (fen.compare("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1") == 0) {
        res = "POSITION: 4";
    }
    if (fen.compare("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8") == 0) {
        res = "POSITION: 5";
    }
    if (fen.compare("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10") == 0) {
        res = "POSITION: 6";
    }
    assert(res.compare("") != 0);
    return res;
}

void print_result(const std::string& status, const std::string& fen, int depth, long long time_us, u64 expected, u64 actual) {
    std::cout << std::left << std::setw(8) << status;
    std::cout << std::setw(12) << convert_position(fen);
    std::cout << " | DEPTH: " << std::setw(2) << depth;
    std::cout << " | TIME: " << std::setw(10) << time_us << " microseconds";
    std::cout << " | EXPECTED: " << std::setw(14) << expected;
    std::cout << " | GOT: " << std::setw(14) << actual << "\n";
}

u64 perft(ChessLogic &logic, int depth) {
    std::vector<u16> moves = MoveGenerator::generate_legal_moves(logic);

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

u64 divide_perft(ChessLogic &logic, int depth) {
    std::vector<u16> moves = MoveGenerator::generate_legal_moves(logic);
    u64 total_nodes = 0;

    for (u16 move : moves) {
        logic.make_move(move);
        u64 nodes = perft(logic, depth - 1);
        logic.unmake_move(move);

        std::string move_str = print_pos((int)get_from(move)) + print_pos((int)get_to(move));
        std::cout << move_str << ": " << nodes << "\n";
        total_nodes += nodes;
    }
    std::cout << "Total nodes: " << total_nodes << "\n";
    return total_nodes;
}

void run_perft_test(const std::string &fen, int depth, u64 expected) {
    ChessLogic logic;
    logic.load_pos(fen);
    bool passed;

    auto start = std::chrono::high_resolution_clock::now();

    u64 nodes = perft(logic, depth);

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    passed = nodes == expected;
    std::string status;
    if (passed) {
        status = "\033[1;32mPASSED\033[0m   ";
    } else {
        status = "\033[1;31mFAILED\033[0m   ";
    }
    print_result(status, fen, depth, elapsed.count(), expected, nodes);
}

void enable_virtual_terminal_processing() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

int main(void) {
    enable_virtual_terminal_processing();
    MoveGenerator::init();

    auto start = std::chrono::high_resolution_clock::now();

    // Depth 1
    run_perft_test("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 1, 20);
    run_perft_test("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 1, 48);
    run_perft_test("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 1, 14);
    run_perft_test("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 1, 6);
    run_perft_test("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 1, 44);
    run_perft_test("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10", 1, 46);

    // Depth 2
    run_perft_test("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 2, 400);
    run_perft_test("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 2, 2039);
    run_perft_test("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 2, 191);
    run_perft_test("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 2, 264);
    run_perft_test("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 2, 1486);
    run_perft_test("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10", 2, 2079);


    // Depth 3
    run_perft_test("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 3, 8902);
    run_perft_test("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 3, 97862);
    run_perft_test("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 3, 2812);
    run_perft_test("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 3, 9467);
    run_perft_test("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 3, 62379);
    run_perft_test("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10", 3, 89890);

    // Depth 4
    run_perft_test("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 4, 197281);
    run_perft_test("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 4, 4085603);
    run_perft_test("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 4, 43238);
    run_perft_test("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 4, 422333);
    run_perft_test("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 4, 2103487);
    run_perft_test("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10", 4, 3894594);

    // Depth 5
    run_perft_test("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 5, 4865609);
    run_perft_test("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 5, 193690690);
    run_perft_test("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 5, 674624);
    run_perft_test("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 5, 15833292);
    run_perft_test("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 5, 89941194);
    run_perft_test("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10", 5, 164075551);

    // Depth 6
    run_perft_test("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 6, 119060324);
    run_perft_test("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 6, 8031647685);
    run_perft_test("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 6, 11030083);
    run_perft_test("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 6, 706045033);
    run_perft_test("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10", 6, 6923051137);

    // Depth 7
    run_perft_test("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 7, 178633661);

    // Depth 8
    run_perft_test("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 8, 3009794393);

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start);

    std::cout << "TOTAL TIME: " << elapsed.count() << " seconds\n";
}