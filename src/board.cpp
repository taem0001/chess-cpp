#include "../include/board.h"
#include "../include/piece.h"
#include "../include/utils.h"
#include <iostream>
#include <string>
#define START_POS \
    "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R" // TODO: Add the rest of the
                                                        // FEN-notation

Board::Board() { load_start_pos(board); }

void Board::load_start_pos(unsigned char *board) { load_pos(START_POS, board); }

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

std::string Board::write_fen() {
    int index = 0;
    int spacing = 0;
    std::string res = "";

    while (index < 64) {
        switch (board[index]) {
        case 0:
            spacing++;
            break;
        default:
            res += evaluate_fen_char(spacing, Piece::get_symbol(board[index]));
            break;
        }

        if (index % 8 == 7 && index != 63) {
            if (spacing > 0) {
                res += std::to_string(spacing);
                spacing = 0;
            }
            res += "/";
        }

        index++;
    }

    return res;
}

void Board::draw_board() {
    for (int i = 0; i < 64; i++) {
        std::cout << Piece::get_symbol(board[i]) << " ";

        if (i % 8 == 7) {
            std::cout << std::endl;
        }
    }
}
