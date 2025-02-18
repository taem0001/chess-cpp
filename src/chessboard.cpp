#include "../include/chessboard.h"
#include <iostream>

ChessBoard::ChessBoard() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            board[i][j] = nullptr;
        }
    }

    // Place pawns on the board
    for (int i = 0; i < 8; i++) {
        board[1][i] = new Pawn({1, (unsigned int)i}, PieceColor::BLACK);
        board[6][i] = new Pawn({6, (unsigned int)i}, PieceColor::WHITE);
    }

    // Place rooks on the board
    board[0][0] = new Rook({0, 0}, PieceColor::BLACK);
    board[0][7] = new Rook({0, 7}, PieceColor::BLACK);
    board[7][0] = new Rook({7, 0}, PieceColor::WHITE);
    board[7][7] = new Rook({7, 7}, PieceColor::WHITE);

    // Place knights on the board
    board[0][1] = new Knight({0, 1}, PieceColor::BLACK);
    board[0][6] = new Knight({0, 6}, PieceColor::BLACK);
    board[7][1] = new Knight({7, 1}, PieceColor::WHITE);
    board[7][6] = new Knight({7, 6}, PieceColor::WHITE);

    // Place bishops on the board
    board[0][2] = new Bishop({0, 2}, PieceColor::BLACK);
    board[0][5] = new Bishop({0, 5}, PieceColor::BLACK);
    board[7][2] = new Bishop({7, 2}, PieceColor::WHITE);
    board[7][5] = new Bishop({7, 5}, PieceColor::WHITE);

    // Place queens on the board
    board[0][3] = new Queen({0, 3}, PieceColor::BLACK);
    board[7][3] = new Queen({7, 3}, PieceColor::WHITE);

    // Place kings on the board
    board[0][4] = new King({0, 4}, PieceColor::BLACK);
    board[7][4] = new King({7, 4}, PieceColor::WHITE);
}

ChessBoard::~ChessBoard() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            delete board[i][j];
        }
    }
}

Piece *ChessBoard::get_piece(Position pos) {
    return board[pos.row][pos.col];
}

Piece *(*ChessBoard::get_board())[8][8] {
    return &board;
}

void ChessBoard::move_piece(Position start, Position end) {
    Piece *piece = get_piece(start);
    if (piece && piece->is_valid_move(end, board)) {
        board[end.row][end.col] = piece;
        board[end.row][end.col]->set_pos(end.row, end.col);
        board[start.row][start.col ] = nullptr;
    }
}

void ChessBoard::draw_board() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j]) {
                std::cout << board[i][j]->draw_piece();
            } else {
                std::cout << "*";
            }
        }
        std::cout << std::endl;
    }
}
