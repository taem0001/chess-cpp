cmake_minimum_required(VERSION 3.28)
project(ChessProject)

set(CMAKE_CXX_STANDARD 20)

# Include main project headers
include_directories(include)

# Unit test executable
add_executable(runTests
    test/test.cpp
    src/engine/board.cpp
    src/engine/chesslogic.cpp
    src/engine/fen.cpp
    src/engine/utils.cpp
    src/engine/movegen.cpp
    src/engine/bitboard.cpp
    src/engine/magic.cpp
    src/engine/perft.cpp
)

# UCI program
add_executable(runUCI
    src/engine/board.cpp
    src/engine/chesslogic.cpp
    src/engine/fen.cpp
    src/engine/utils.cpp
    src/engine/movegen.cpp
    src/engine/bitboard.cpp
    src/engine/magic.cpp
    src/engine/bot.cpp
    src/engine/perft.cpp
    src/uci.cpp
)