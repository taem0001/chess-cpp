#include <gtest/gtest.h>

#include "../include/chesslogic.h"
#include "../include/movegen.h"
#include "../include/utils.h"

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
            std::cout << move_str << ": " << nodes << std::endl;
            total_nodes += nodes;
        }
        std::cout << "Total nodes: " << total_nodes << std::endl;
        return total_nodes;
    }
};

/* TEST_F(PerftTest, InitPos) {
    ChessLogic logic;
    logic.load_pos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    EXPECT_EQ(perft(logic, 1), 20);
    EXPECT_EQ(perft(logic, 2), 400);
    EXPECT_EQ(perft(logic, 3), 8902);
    EXPECT_EQ(perft(logic, 4), 197281);
    EXPECT_EQ(perft(logic, 5), 4865609);
    EXPECT_EQ(perft(logic, 6), 119060324);
    EXPECT_EQ(perft(logic, 7), 3195901860);
    EXPECT_EQ(perft(logic, 8), 84998978956);
    EXPECT_EQ(perft(logic, 9), 2439530234167);
} */

/* TEST_F(PerftTest, InitPosDivide) {
    ChessLogic logic;
    logic.load_pos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    EXPECT_EQ(divide_perft(logic, 1), 20);
    EXPECT_EQ(divide_perft(logic, 2), 400);
    EXPECT_EQ(divide_perft(logic, 3), 8902);
    EXPECT_EQ(divide_perft(logic, 4), 197281);
    EXPECT_EQ(divide_perft(logic, 5), 4865609);
    EXPECT_EQ(divide_perft(logic, 6), 119060324);
    EXPECT_EQ(divide_perft(logic, 7), 3195901860);
    EXPECT_EQ(divide_perft(logic, 8), 84998978956);
    EXPECT_EQ(divide_perft(logic, 9), 2439530234167);
} */

/* TEST_F(PerftTest, KiwipetePos) {
    ChessLogic logic;
    logic.load_pos("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");

    EXPECT_EQ(perft(logic, 1), 48);
    EXPECT_EQ(perft(logic, 2), 2039);
    EXPECT_EQ(perft(logic, 3), 97862);
    EXPECT_EQ(perft(logic, 4), 4085603);
    EXPECT_EQ(perft(logic, 5), 193690690);
    EXPECT_EQ(perft(logic, 6), 8031647685);
} */

/* TEST_F(PerftTest, KiwipetePosDivide) {
    ChessLogic logic;
    logic.load_pos("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");

    EXPECT_EQ(divide_perft(logic, 1), 48);
    EXPECT_EQ(divide_perft(logic, 2), 2039);
    EXPECT_EQ(divide_perft(logic, 3), 97862);
    EXPECT_EQ(divide_perft(logic, 4), 4085603);
    EXPECT_EQ(divide_perft(logic, 5), 193690690);
    EXPECT_EQ(divide_perft(logic, 6), 8031647685);
} */

/* TEST_F(PerftTest, Position3Divide) {
    ChessLogic logic;
    logic.load_pos("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");

    EXPECT_EQ(divide_perft(logic, 1), 14);
    EXPECT_EQ(divide_perft(logic, 2), 191);
    EXPECT_EQ(divide_perft(logic, 3), 2812);
    EXPECT_EQ(divide_perft(logic, 4), 43238);
    EXPECT_EQ(divide_perft(logic, 5), 674624);
    EXPECT_EQ(divide_perft(logic, 6), 11030083);
    EXPECT_EQ(divide_perft(logic, 7), 178633661);
    EXPECT_EQ(divide_perft(logic, 8), 3009794393);
} */

/* TEST_F(PerftTest, Position4Divide) {
    ChessLogic logic;
    logic.load_pos("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");

    EXPECT_EQ(divide_perft(logic, 1), 6);
    EXPECT_EQ(divide_perft(logic, 2), 264);
    EXPECT_EQ(divide_perft(logic, 3), 9467);
    EXPECT_EQ(divide_perft(logic, 4), 422333);
    EXPECT_EQ(divide_perft(logic, 5), 15833292);
    EXPECT_EQ(divide_perft(logic, 6), 706045033);
} */

/* TEST_F(PerftTest, Position5Divide) {
    ChessLogic logic;
    logic.load_pos("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");

    EXPECT_EQ(divide_perft(logic, 1), 44);
    EXPECT_EQ(divide_perft(logic, 2), 1486);
    EXPECT_EQ(divide_perft(logic, 3), 62379);
    EXPECT_EQ(divide_perft(logic, 4), 2103487);
    EXPECT_EQ(divide_perft(logic, 5), 89941194);
} */

TEST_F(PerftTest, Position6Divide) {
    ChessLogic logic;
    logic.load_pos("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");

    EXPECT_EQ(divide_perft(logic, 1), 46);
    EXPECT_EQ(divide_perft(logic, 2), 2079);
    EXPECT_EQ(divide_perft(logic, 3), 89890);
    EXPECT_EQ(divide_perft(logic, 4), 3894594);
    EXPECT_EQ(divide_perft(logic, 5), 164075551);
    EXPECT_EQ(divide_perft(logic, 6), 6923051137);
    EXPECT_EQ(divide_perft(logic, 7), 287188994746);
    EXPECT_EQ(divide_perft(logic, 8), 11923589843526);
    EXPECT_EQ(divide_perft(logic, 9), 490154852788714);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
