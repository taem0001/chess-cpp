#include "../include/board.h"
#include <gtest/gtest.h>

class FenTest : public ::testing::Test {
protected:
    Board board;

    std::string test_eq(const char *fen) {
        board.load_pos(fen);
        return board.write_fen();
    }
};

TEST_F(FenTest, StartPos) { EXPECT_EQ(test_eq("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq"), "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq"); }

TEST_F(FenTest, Pos1) { EXPECT_EQ(test_eq("rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq"), "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq"); }

TEST_F(FenTest, Pos2) { EXPECT_EQ(test_eq("rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq"), "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq"); }

TEST_F(FenTest, Pos3) { EXPECT_EQ(test_eq("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w -"), "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w -"); }

TEST_F(FenTest, Pos4) { EXPECT_EQ(test_eq("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq"), "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq"); }

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
