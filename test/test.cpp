#include <gtest/gtest.h>
#include "../include/chessgame.h"
#include "../include/movegen.h"
#include "../include/utils.h"

class PerftTest : public testing::Test {
protected:
    void SetUp() override {
        MoveGenerator::init();
    }

private:
    ChessGame game;

public:
    u64 perft(int depth) {
        std::vector<u16> moves;
        int n_moves, i;
        u64 nodes = 0;

        if (depth == 0) {
            return 1ULL;
        }

        moves = MoveGenerator::generate_legal_moves(game);
        n_moves = moves.size();
        for (i = 0; i < n_moves; i++) {
            game.make_move(moves[i]);
            nodes += perft(depth - 1);
            game.unmake_move();
        }
        return nodes;
    }
};

TEST_F(PerftTest, InitPos) {
    EXPECT_EQ(perft(1), 20);
    EXPECT_EQ(perft(2), 400);
    EXPECT_EQ(perft(3), 8902);
    EXPECT_EQ(perft(4), 197281);
    EXPECT_EQ(perft(5), 4865609);
    EXPECT_EQ(perft(6), 119060324);
    EXPECT_EQ(perft(7), 3195901860);
    EXPECT_EQ(perft(8), 84998978956);
    EXPECT_EQ(perft(9), 2439530234167);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
