#include "../include/chessgame.h"
#include "../include/movegen.h"
#include "../include/utils.h"
#include <gtest/gtest.h>

class PerftTest : public testing::Test {
protected:
    void SetUp() override { MoveGenerator::init(); }

public:
    u64 perft(ChessGame &game, int depth) {
        std::vector<u16> moves = MoveGenerator::generate_legal_moves(game);

        if (depth == 0) {
            return 1;
        }

        u64 nodes = 0;
        for (u16 move : moves) {
            game.make_move(move);

            u64 move_nodes = perft(game, depth - 1);
            nodes += move_nodes;

            game.unmake_move(move);
            std::cout << print_pos((int)get_from(move)) << print_pos((int)get_to(move)) << ": " << move_nodes
                      << std::endl;
        }

        return nodes;
    }
};

TEST_F(PerftTest, InitPos) {
    ChessGame game;
    game.load_pos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    //    EXPECT_EQ(perft(game, 1), 20);
    EXPECT_EQ(perft(game, 2), 400);
    //    EXPECT_EQ(perft(game, 3), 8902);
    //    EXPECT_EQ(perft(game, 4), 197281);
    //    EXPECT_EQ(perft(game, 5), 4865609);
    //    EXPECT_EQ(perft(game, 6), 119060324);
    //    EXPECT_EQ(perft(game, 7), 3195901860);
    //    EXPECT_EQ(perft(game, 8), 84998978956);
    //    EXPECT_EQ(perft(game, 9), 2439530234167);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
