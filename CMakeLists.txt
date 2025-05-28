cmake_minimum_required(VERSION 3.28)
project(ChessProject)

set(CMAKE_CXX_STANDARD 20)

# Include main project headers
include_directories(include)

# Main application
add_executable(runChess
    engine/main.cpp
    engine/board.cpp
    engine/chesslogic.cpp
    engine/fen.cpp
    engine/chessgame.cpp
    engine/utils.cpp
    engine/movegen.cpp
    engine/bitboard.cpp
    engine/magic.cpp
    engine/bot.cpp
)

# Bot vs Bot application
add_executable(runBot
    test/botvsbot.cpp
    engine/board.cpp
    engine/chesslogic.cpp
    engine/fen.cpp
    engine/chessgame.cpp
    engine/utils.cpp
    engine/movegen.cpp
    engine/bitboard.cpp
    engine/magic.cpp
    engine/bot.cpp
)

# Unit test executable
add_executable(runTests
    test/test.cpp
    engine/board.cpp
    engine/chesslogic.cpp
    engine/fen.cpp
    engine/utils.cpp
    engine/movegen.cpp
    engine/bitboard.cpp
    engine/magic.cpp
)

# UCI program
add_executable(runUCI
    engine/board.cpp
    engine/chesslogic.cpp
    engine/fen.cpp
    engine/utils.cpp
    engine/movegen.cpp
    engine/bitboard.cpp
    engine/magic.cpp
    engine/bot.cpp
    engine/uci.cpp
)