cmake_minimum_required(VERSION 3.28)
project(ChessProject)
set(CMAKE_CXX_STANDARD 17)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

include_directories(include)

add_executable(ChessProject src/main.cpp src/board.cpp src/chessgame.cpp src/fen.cpp src/interface.cpp src/utils.cpp src/movegen.cpp src/bitboard.cpp)

add_executable(runTests test/test.cpp src/board.cpp src/chessgame.cpp src/fen.cpp src/utils.cpp src/movegen.cpp src/bitboard.cpp)

target_link_libraries(runTests GTest::GTest GTest::Main pthread)

enable_testing()
add_test(NAME Testing COMMAND runTests)
