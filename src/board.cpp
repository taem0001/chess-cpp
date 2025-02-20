#include "../include/board.h"
#include "../include/piece.h"
#include <iostream>
#define START_POS "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR" // TODO: Add the rest of the FEN-notation

Board::Board() {
    load_start_pos(board);
}

void Board::load_start_pos(unsigned char *board) {
    load_pos(START_POS, board);
}

void Board::load_pos(const char *fen, unsigned char *board) {
    int index = 0;

    while (*fen) {
        switch (*fen) {
        case 'p':
            board[index] = Piece::make_piece(Piece::pawn, Piece::black);
            index++;
            break;
        case 'n':
            board[index] = Piece::make_piece(Piece::knight, Piece::black);
            index++;
            break;
        case 'b':
            board[index] = Piece::make_piece(Piece::bishop, Piece::black);
            index++;
            break;
        case 'r':
            board[index] = Piece::make_piece(Piece::rook, Piece::black);
            index++;
            break;
        case 'q':
            board[index] = Piece::make_piece(Piece::queen, Piece::black);
            index++;
            break;
        case 'k':
            board[index] = Piece::make_piece(Piece::king, Piece::black);
            index++;
            break;
        case 'P':
            board[index] = Piece::make_piece(Piece::pawn, Piece::white);
            index++;
            break;
        case 'N':
            board[index] = Piece::make_piece(Piece::knight, Piece::white);
            index++;
            break;
        case 'B':
            board[index] = Piece::make_piece(Piece::bishop, Piece::white);
            index++;
            break;
        case 'R':
            board[index] = Piece::make_piece(Piece::rook, Piece::white);
            index++;
            break;
        case 'Q':
            board[index] = Piece::make_piece(Piece::queen, Piece::white);
            index++;
            break;
        case 'K':
            board[index] = Piece::make_piece(Piece::king, Piece::white);
            index++;
            break;
        case '/':
            break;
        case '1':
            board[index] = Piece::none;
            index++;
            break;
        case '2':
            for (int i = index; i < index + 2; i++) {
                board[i] = Piece::none;
            }
            index += 2;
            break;
        case '3':
            for (int i = index; i < index + 3; i++) {
                board[i] = Piece::none;
            }
            index += 3;
            break;
        case '4':
            for (int i = index; i < index + 4; i++) {
                board[i] = Piece::none;
            }
            index += 4;
            break;
        case '5':
            for (int i = index; i < index + 5; i++) {
                board[i] = Piece::none;
            }
            index += 5;
            break;
        case '6':
            for (int i = index; i < index + 6; i++) {
                board[i] = Piece::none;
            }
            index += 6;
            break;
        case '7':
            for (int i = index; i < index + 7; i++) {
                board[i] = Piece::none;
            }
            index += 7;
            break;
        case '8':
            for (int i = index; i < index + 8; i++) {
                board[i] = Piece::none;
            }
            index += 8;
            break;
        }

        ++fen;
    }
}

void Board::draw_board() {
    for (int i = 0; i < 64; i++) {
        std::cout << Piece::get_symbol(board[i]) << " ";

        if (i % 8 == 7) {
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}
