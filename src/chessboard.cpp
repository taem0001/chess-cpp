#include "../include/chessboard.h"
#include <iostream>

ChessBoard::ChessBoard() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            chessBoard[i][j] = nullptr;
        }
    }

    // Place pawns on the board
    for (int i = 0; i < 8; i++) {
        chessBoard[1][i] = new Pawn({1, (unsigned int)i}, PieceColor::BLACK);
        chessBoard[6][i] = new Pawn({6, (unsigned int)i}, PieceColor::WHITE);
    }

    // Place rooks on the board
    chessBoard[0][0] = new Rook({0, 0}, PieceColor::BLACK);
    chessBoard[0][7] = new Rook({0, 7}, PieceColor::BLACK);
    chessBoard[7][0] = new Rook({7, 0}, PieceColor::WHITE);
    chessBoard[7][7] = new Rook({7, 7}, PieceColor::WHITE);

    // Place knights on the board
    chessBoard[0][1] = new Knight({0, 1}, PieceColor::BLACK);
    chessBoard[0][6] = new Knight({0, 6}, PieceColor::BLACK);
    chessBoard[7][1] = new Knight({7, 1}, PieceColor::WHITE);
    chessBoard[7][6] = new Knight({7, 6}, PieceColor::WHITE);

    // Place bishops on the board
    chessBoard[0][2] = new Bishop({0, 2}, PieceColor::BLACK);
    chessBoard[0][5] = new Bishop({0, 5}, PieceColor::BLACK);
    chessBoard[7][2] = new Bishop({7, 2}, PieceColor::WHITE);
    chessBoard[7][5] = new Bishop({7, 5}, PieceColor::WHITE);

    // Place queens on the board
    chessBoard[0][3] = new Queen({0, 3}, PieceColor::BLACK);
    chessBoard[7][3] = new Queen({7, 3}, PieceColor::WHITE);

    // Place kings on the board
    chessBoard[0][4] = new King({0, 4}, PieceColor::BLACK);
    chessBoard[7][4] = new King({7, 4}, PieceColor::WHITE);
}

void ChessBoard::draw_board() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (chessBoard[i][j]) {
                std::cout << chessBoard[i][j]->draw_piece();
            } else {
                std::cout << "*";
            }
        }
        std::cout << std::endl;
    }
}
