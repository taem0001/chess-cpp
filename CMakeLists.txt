cmake_minimum_required(VERSION 3.28)
project(ChessProject)

set(CMAKE_CXX_STANDARD 17)

# Find Google Test (installed via package manager)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Include project headers
include_directories(include)

# Main executable
add_executable(ChessProject src/main.cpp src/game.cpp src/board.cpp src/piece.cpp src/utils.cpp)

# Test executable
add_executable(runPerftTests test/perft.cpp src/game.cpp src/board.cpp src/piece.cpp src/utils.cpp)
add_executable(runFenTests test/fen.cpp src/game.cpp src/board.cpp src/piece.cpp src/utils.cpp)

# Link Google Test and pthread
target_link_libraries(runPerftTests GTest::GTest GTest::Main pthread)
target_link_libraries(runFenTests GTest::GTest GTest::Main pthread)

# Add test execution
enable_testing()
add_test(NAME Testing COMMAND runTests)

