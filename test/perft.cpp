#include "../include/game.h"
#include <cstdint>
#include <gtest/gtest.h>

class PerftTest : public ::testing::Test {
  protected:
    Game game;

    uint64_t perft(int depth) {
        std::vector<Move> moves;
        int n_moves, i;
        uint64_t nodes = 0;

        moves = game.get_board()->generate_legal_moves();
        n_moves = moves.size();

        if (depth == 1) {
            return (uint64_t)n_moves;
        }

        for (i = 0; i < n_moves; i++) {
            game.make_move(moves, moves.at(i).start_square, moves.at(i).end_square);
            nodes += perft(depth - 1);
            game.get_board()->move_piece(moves.at(i).end_square, moves.at(i).start_square);
        }
        return nodes;
    }
};

TEST_F(PerftTest, PerftInitPos) {
    EXPECT_EQ(perft(1), 20);
    EXPECT_EQ(perft(2), 400);
    EXPECT_EQ(perft(3), 8902);
    EXPECT_EQ(perft(4), 197281);
    EXPECT_EQ(perft(5), 4865609);
    EXPECT_EQ(perft(6), 119060324);
    //    EXPECT_EQ(perft(7), 3195901860);
    //    EXPECT_EQ(perft(8), 84998978956);
    //    EXPECT_EQ(perft(9), 2439530234167);
    //    EXPECT_EQ(perft(10), 69352859712417);
    //    EXPECT_EQ(perft(11), 2097651003696806);
    //    EXPECT_EQ(perft(12), 62854969236701747);
    //    EXPECT_EQ(perft(13), 1981066775000396239);
}

TEST_F(PerftTest, PerftPos2) {
    game.get_board()->load_pos("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq");
    EXPECT_EQ(perft(1), 48);
    EXPECT_EQ(perft(2), 2039);
    EXPECT_EQ(perft(3), 97862);
    EXPECT_EQ(perft(4), 4085603);
    EXPECT_EQ(perft(5), 193690690);
    //    EXPECT_EQ(perft(6), 8031647685);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
