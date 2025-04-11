cmake_minimum_required(VERSION 3.28)
project(ChessProject)

set(CMAKE_CXX_STANDARD 20)

# Include main project headers
include_directories(include)

# Automatically download and add GoogleTest
include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)

# Required for Windows/MSVC to avoid runtime conflicts
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

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
)

# Link test executable with GoogleTest and pthread if needed
target_link_libraries(runTests
    gtest_main
)

enable_testing()
add_test(NAME Testing COMMAND runTests)
