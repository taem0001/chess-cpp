cmake_minimum_required(VERSION 3.28)
project(ChessProject)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
add_executable(ChessProject src/main.cpp src/chessboard.cpp src/piece.cpp src/chesslogic.cpp src/game.cpp src/utils.cpp)
