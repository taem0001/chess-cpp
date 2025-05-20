cmake_minimum_required(VERSION 3.28)
project(ChessProject)

set(CMAKE_CXX_STANDARD 20)

# Include main project headers
include_directories(include)

# Main application
add_executable(runChess
    src/main.cpp
    src/board.cpp
    src/chesslogic.cpp
    src/fen.cpp
    src/chessgame.cpp
    src/utils.cpp
    src/movegen.cpp
    src/bitboard.cpp
    src/magic.cpp
    src/bot.cpp
)

# Bot vs Bot application
add_executable(runBot
    test/botvsbot.cpp
    src/board.cpp
    src/chesslogic.cpp
    src/fen.cpp
    src/chessgame.cpp
    src/utils.cpp
    src/movegen.cpp
    src/bitboard.cpp
    src/magic.cpp
    src/bot.cpp
)

# Unit test executable
add_executable(runTests
    test/test.cpp
    src/board.cpp
    src/chesslogic.cpp
    src/fen.cpp
    src/utils.cpp
    src/movegen.cpp
    src/bitboard.cpp
    src/magic.cpp
)