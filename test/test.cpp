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
        u64 nodes = 0;

        if (depth == 0) {
            return 1ULL;
        }

        moves = MoveGenerator::generate_legal_moves(game);
        for (u16 move : moves) {
            game.make_move(move);
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
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
