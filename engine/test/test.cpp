#include "../include/chesslogic.h"
#include "../include/movegen.h"
#include "../include/utils.h"
#include <gtest/gtest.h>

class PerftTest : public testing::Test {
protected:
    void SetUp() override { MoveGenerator::init(); }

public:
    u64 perft(ChessLogic &logic, int depth) {
        std::vector<u64> moves = MoveGenerator::generate_legal_moves(logic);

        if (depth == 1) {
            return moves.size();
        }
        if (depth == 0) {
            return 1;
        }

        u64 total_nodes = 0;
        for (u64 move : moves) {
            logic.make_move(move);
            u64 nodes = perft(logic, depth - 1);
            logic.unmake_move(move);
            total_nodes += nodes;
        }
        return total_nodes;
    }

    u64 divide_perft(ChessLogic &logic, int depth) {
        std::vector<u64> moves = MoveGenerator::generate_legal_moves(logic);
        u64 total_nodes = 0;

        for (u64 move : moves) {
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
};

// Depth 1
TEST_F(PerftTest, StartPos1) {
    ChessLogic logic;
    logic.load_pos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    EXPECT_EQ(perft(logic, 1), 20);
}

TEST_F(PerftTest, KiwipetePos1) {
    ChessLogic logic;
    logic.load_pos("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    EXPECT_EQ(perft(logic, 1), 48);
}

TEST_F(PerftTest, Position3_1) {
    ChessLogic logic;
    logic.load_pos("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
    EXPECT_EQ(perft(logic, 1), 14);
}

TEST_F(PerftTest, Position4_1) {
    ChessLogic logic;
    logic.load_pos("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
    EXPECT_EQ(perft(logic, 1), 6);
}

TEST_F(PerftTest, Position5_1) {
    ChessLogic logic;
    logic.load_pos("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
    EXPECT_EQ(perft(logic, 1), 44);
}

TEST_F(PerftTest, Position6_1) {
    ChessLogic logic;
    logic.load_pos("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
    EXPECT_EQ(perft(logic, 1), 46);
}

// Depth 2
TEST_F(PerftTest, StartPos2) {
    ChessLogic logic;
    logic.load_pos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    EXPECT_EQ(perft(logic, 2), 400);
}

TEST_F(PerftTest, KiwipetePos2) {
    ChessLogic logic;
    logic.load_pos("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    EXPECT_EQ(perft(logic, 2), 2039);
}

TEST_F(PerftTest, Position3_2) {
    ChessLogic logic;
    logic.load_pos("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
    EXPECT_EQ(perft(logic, 2), 191);
}

TEST_F(PerftTest, Position4_2) {
    ChessLogic logic;
    logic.load_pos("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
    EXPECT_EQ(perft(logic, 2), 264);
}

TEST_F(PerftTest, Position5_2) {
    ChessLogic logic;
    logic.load_pos("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
    EXPECT_EQ(perft(logic, 2), 1486);
}

TEST_F(PerftTest, Position6_2) {
    ChessLogic logic;
    logic.load_pos("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
    EXPECT_EQ(perft(logic, 2), 2079);
}

// Depth 3
TEST_F(PerftTest, StartPos3) {
    ChessLogic logic;
    logic.load_pos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    EXPECT_EQ(perft(logic, 3), 8902);
}

TEST_F(PerftTest, KiwipetePos3) {
    ChessLogic logic;
    logic.load_pos("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    EXPECT_EQ(perft(logic, 3), 97862);
}

TEST_F(PerftTest, Position3_3) {
    ChessLogic logic;
    logic.load_pos("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
    EXPECT_EQ(perft(logic, 3), 2812);
}

TEST_F(PerftTest, Position4_3) {
    ChessLogic logic;
    logic.load_pos("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
    EXPECT_EQ(perft(logic, 3), 9467);
}

TEST_F(PerftTest, Position5_3) {
    ChessLogic logic;
    logic.load_pos("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
    EXPECT_EQ(perft(logic, 3), 62379);
}

TEST_F(PerftTest, Position6_3) {
    ChessLogic logic;
    logic.load_pos("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
    EXPECT_EQ(perft(logic, 3), 89890);
}

// Depth 4
TEST_F(PerftTest, StartPos4) {
    ChessLogic logic;
    logic.load_pos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    EXPECT_EQ(perft(logic, 4), 197281);
}

TEST_F(PerftTest, KiwipetePos4) {
    ChessLogic logic;
    logic.load_pos("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    EXPECT_EQ(perft(logic, 4), 4085603);
}

TEST_F(PerftTest, Position3_4) {
    ChessLogic logic;
    logic.load_pos("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
    EXPECT_EQ(perft(logic, 4), 43238);
}

TEST_F(PerftTest, Position4_4) {
    ChessLogic logic;
    logic.load_pos("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
    EXPECT_EQ(perft(logic, 4), 422333);
}

TEST_F(PerftTest, Position5_4) {
    ChessLogic logic;
    logic.load_pos("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
    EXPECT_EQ(perft(logic, 4), 2103487);
}

TEST_F(PerftTest, Position6_4) {
    ChessLogic logic;
    logic.load_pos("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
    EXPECT_EQ(perft(logic, 4), 3894594);
}

// Depth 5
TEST_F(PerftTest, StartPos5) {
    ChessLogic logic;
    logic.load_pos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    EXPECT_EQ(perft(logic, 5), 4865609);
}

TEST_F(PerftTest, KiwipetePos5) {
    ChessLogic logic;
    logic.load_pos("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    EXPECT_EQ(perft(logic, 5), 193690690);
}

TEST_F(PerftTest, Position3_5) {
    ChessLogic logic;
    logic.load_pos("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
    EXPECT_EQ(perft(logic, 5), 674624);
}

TEST_F(PerftTest, Position4_5) {
    ChessLogic logic;
    logic.load_pos("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
    EXPECT_EQ(perft(logic, 5), 15833292);
}

TEST_F(PerftTest, Position5_5) {
    ChessLogic logic;
    logic.load_pos("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
    EXPECT_EQ(perft(logic, 5), 89941194);
}

TEST_F(PerftTest, Position6_5) {
    ChessLogic logic;
    logic.load_pos("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
    EXPECT_EQ(perft(logic, 5), 164075551);
}

// Depth 6
TEST_F(PerftTest, StartPos6) {
    ChessLogic logic;
    logic.load_pos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    EXPECT_EQ(perft(logic, 6), 119060324);
}

TEST_F(PerftTest, KiwipetePos6) {
    ChessLogic logic;
    logic.load_pos("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    EXPECT_EQ(perft(logic, 6), 8031647685);
}

TEST_F(PerftTest, Position3_6) {
    ChessLogic logic;
    logic.load_pos("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
    EXPECT_EQ(perft(logic, 6), 11030083);
}

TEST_F(PerftTest, Position4_6) {
    ChessLogic logic;
    logic.load_pos("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
    EXPECT_EQ(perft(logic, 6), 706045033);
}

TEST_F(PerftTest, Position6_6) {
    ChessLogic logic;
    logic.load_pos("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
    EXPECT_EQ(perft(logic, 6), 6923051137);
}

// Depth 7
TEST_F(PerftTest, Position3_7) {
    ChessLogic logic;
    logic.load_pos("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
    EXPECT_EQ(perft(logic, 7), 178633661);
}

// Depth 8
TEST_F(PerftTest, Position3_8) {
    ChessLogic logic;
    logic.load_pos("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
    EXPECT_EQ(perft(logic, 8), 3009794393);
}

/* TEST_F(PerftTest, StartPos1Divide) {
    ChessLogic logic;
    logic.load_pos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    EXPECT_EQ(divide_perft(logic, 1), 20);
}

TEST_F(PerftTest, StartPos2Divide) {
    ChessLogic logic;
    logic.load_pos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    EXPECT_EQ(divide_perft(logic, 2), 400);
}

TEST_F(PerftTest, StartPos3Divide) {
    ChessLogic logic;
    logic.load_pos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    EXPECT_EQ(divide_perft(logic, 3), 8902);
}

TEST_F(PerftTest, StartPos4Divide) {
    ChessLogic logic;
    logic.load_pos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    EXPECT_EQ(divide_perft(logic, 4), 197281);
}

TEST_F(PerftTest, StartPos5Divide) {
    ChessLogic logic;
    logic.load_pos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    EXPECT_EQ(divide_perft(logic, 5), 4865609);
}

TEST_F(PerftTest, StartPos6Divide) {
    ChessLogic logic;
    logic.load_pos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    EXPECT_EQ(divide_perft(logic, 6), 119060324);
} */

/* TEST_F(PerftTest, KiwipetePos1Divide) {
    ChessLogic logic;
    logic.load_pos("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    EXPECT_EQ(divide_perft(logic, 1), 48);
}

TEST_F(PerftTest, KiwipetePos2Divide) {
    ChessLogic logic;
    logic.load_pos("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    EXPECT_EQ(divide_perft(logic, 2), 2039);
}

TEST_F(PerftTest, KiwipetePos3Divide) {
    ChessLogic logic;
    logic.load_pos("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    EXPECT_EQ(divide_perft(logic, 3), 97862);
}

TEST_F(PerftTest, KiwipetePos4Divide) {
    ChessLogic logic;
    logic.load_pos("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    EXPECT_EQ(divide_perft(logic, 4), 4085603);
}

TEST_F(PerftTest, KiwipetePos5Divide) {
    ChessLogic logic;
    logic.load_pos("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    EXPECT_EQ(divide_perft(logic, 5), 193690690);
}

TEST_F(PerftTest, KiwipetePos6Divide) {
    ChessLogic logic;
    logic.load_pos("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    EXPECT_EQ(divide_perft(logic, 6), 8031647685);
} */

/* TEST_F(PerftTest, Position3Divide1) {
    ChessLogic logic;
    logic.load_pos("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
    EXPECT_EQ(divide_perft(logic, 1), 14);
}

TEST_F(PerftTest, Position3Divide2) {
    ChessLogic logic;
    logic.load_pos("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
    EXPECT_EQ(divide_perft(logic, 2), 191);
}

TEST_F(PerftTest, Position3Divide3) {
    ChessLogic logic;
    logic.load_pos("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
    EXPECT_EQ(divide_perft(logic, 3), 2812);
}

TEST_F(PerftTest, Position3Divide4) {
    ChessLogic logic;
    logic.load_pos("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
    EXPECT_EQ(divide_perft(logic, 4), 43238);
}

TEST_F(PerftTest, Position3Divide5) {
    ChessLogic logic;
    logic.load_pos("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
    EXPECT_EQ(divide_perft(logic, 5), 674624);
}

TEST_F(PerftTest, Position3Divide6) {
    ChessLogic logic;
    logic.load_pos("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
    EXPECT_EQ(divide_perft(logic, 6), 11030083);
}

TEST_F(PerftTest, Position3Divide7) {
    ChessLogic logic;
    logic.load_pos("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
    EXPECT_EQ(divide_perft(logic, 7), 178633661);
}

TEST_F(PerftTest, Position3Divide8) {
    ChessLogic logic;
    logic.load_pos("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
    EXPECT_EQ(divide_perft(logic, 8), 3009794393);
} */

/* TEST_F(PerftTest, Position4Divide1) {
    ChessLogic logic;
    logic.load_pos("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
    EXPECT_EQ(divide_perft(logic, 1), 6);
}

TEST_F(PerftTest, Position4Divide2) {
    ChessLogic logic;
    logic.load_pos("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
    EXPECT_EQ(divide_perft(logic, 2), 264);
}

TEST_F(PerftTest, Position4Divide3) {
    ChessLogic logic;
    logic.load_pos("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
    EXPECT_EQ(divide_perft(logic, 3), 9467);
}

TEST_F(PerftTest, Position4Divide4) {
    ChessLogic logic;
    logic.load_pos("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
    EXPECT_EQ(divide_perft(logic, 4), 422333);
}

TEST_F(PerftTest, Position4Divide5) {
    ChessLogic logic;
    logic.load_pos("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
    EXPECT_EQ(divide_perft(logic, 5), 15833292);
}

TEST_F(PerftTest, Position4Divide6) {
    ChessLogic logic;
    logic.load_pos("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
    EXPECT_EQ(divide_perft(logic, 6), 706045033);
} */

/* TEST_F(PerftTest, Position5Divide1) {
    ChessLogic logic;
    logic.load_pos("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
    EXPECT_EQ(divide_perft(logic, 1), 44);
}

TEST_F(PerftTest, Position5Divide2) {
    ChessLogic logic;
    logic.load_pos("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
    EXPECT_EQ(divide_perft(logic, 2), 1486);
}

TEST_F(PerftTest, Position5Divide3) {
    ChessLogic logic;
    logic.load_pos("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
    EXPECT_EQ(divide_perft(logic, 3), 62379);
}

TEST_F(PerftTest, Position5Divide4) {
    ChessLogic logic;
    logic.load_pos("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
    EXPECT_EQ(divide_perft(logic, 4), 2103487);
}

TEST_F(PerftTest, Position5Divide5) {
    ChessLogic logic;
    logic.load_pos("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
    EXPECT_EQ(divide_perft(logic, 5), 89941194);
} */

/* TEST_F(PerftTest, Position6Divide1) {
    ChessLogic logic;
    logic.load_pos("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
    EXPECT_EQ(divide_perft(logic, 1), 46);
}

TEST_F(PerftTest, Position6Divide2) {
    ChessLogic logic;
    logic.load_pos("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
    EXPECT_EQ(divide_perft(logic, 2), 2079);
}

TEST_F(PerftTest, Position6Divide3) {
    ChessLogic logic;
    logic.load_pos("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
    EXPECT_EQ(divide_perft(logic, 3), 89890);
}

TEST_F(PerftTest, Position6Divide4) {
    ChessLogic logic;
    logic.load_pos("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
    EXPECT_EQ(divide_perft(logic, 4), 3894594);
}

TEST_F(PerftTest, Position6Divide5) {
    ChessLogic logic;
    logic.load_pos("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
    EXPECT_EQ(divide_perft(logic, 5), 164075551);
}

TEST_F(PerftTest, Position6Divide6) {
    ChessLogic logic;
    logic.load_pos("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
    EXPECT_EQ(divide_perft(logic, 6), 6923051137);
} */

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
