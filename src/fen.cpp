#include "../include/fen.h"
#include "../include/piece.h"
#include <iostream>
#include <ostream>

// TODO: Finish implementing the entirety of the FEN-notation
void FenHandler::load_position(Board &chess_board, const std::string &fen) {
    int index = 0;
    int s_index = 0;
    int spaces = 0;
    unsigned char piece, temp;

    for (int i = 0; i < 64; i++) {
        chess_board.set_board_element(Piece::none, i);
    }

    while (s_index < fen.length()) {
        switch (fen[s_index]) {
            case 'p':
                piece = Piece::make_piece(Piece::pawn, Piece::black);
                if (index / 8 != 1) {
                    temp = Piece::set_piece_first_move(piece);
                    piece = temp;
                }
                chess_board.set_board_element(piece, index);
                index++;
                break;
            case 'n':
                piece = Piece::make_piece(Piece::knight, Piece::black);
                chess_board.set_board_element(piece, index);
                index++;
                break;
            case 'b':
                if (spaces == 0) {
                    piece = Piece::make_piece(Piece::bishop, Piece::black);
                    chess_board.set_board_element(piece, index);
                    index++;
                } else if (spaces == 1) {
                    chess_board.set_turn(false);
                }
                break;
            case 'r':
                piece = Piece::make_piece(Piece::rook, Piece::black);
                chess_board.set_board_element(piece, index);
                index++;
                break;
            case 'q':
                if (spaces == 0) {
                    piece = Piece::make_piece(Piece::queen, Piece::black);
                    chess_board.set_board_element(piece, index);
                    index++;
                } else if (spaces == 2) {
                    chess_board.set_black_castle_queen(true);
                }
                break;
            case 'k':
                if (spaces == 0) {
                    piece = Piece::make_piece(Piece::king, Piece::black);
                    chess_board.set_board_element(piece, index);
                    index++;
                } else if (spaces == 2) {
                    chess_board.set_black_castle_king(true);
                }
                break;
            case 'P':
                piece = Piece::make_piece(Piece::pawn, Piece::white);
                if (index / 8 != 6) {
                    temp = Piece::set_piece_first_move(piece);
                    piece = temp;
                }
                chess_board.set_board_element(piece, index);
                index++;
                break;
            case 'N':
                piece = Piece::make_piece(Piece::knight, Piece::white);
                chess_board.set_board_element(piece, index);
                index++;
                break;
            case 'B':
                piece = Piece::make_piece(Piece::bishop, Piece::white);
                chess_board.set_board_element(piece, index);
                index++;
                break;
            case 'R':
                piece = Piece::make_piece(Piece::rook, Piece::white);
                chess_board.set_board_element(piece, index);
                index++;
                break;
            case 'Q':
                if (spaces == 0) {
                    piece = Piece::make_piece(Piece::queen, Piece::white);
                    chess_board.set_board_element(piece, index);
                    index++;
                } else if (spaces == 2) {
                    chess_board.set_white_castle_queen(true);
                }
                break;
            case 'K':
                if (spaces == 0) {
                    piece = Piece::make_piece(Piece::king, Piece::white);
                    chess_board.set_board_element(piece, index);
                    index++;
                } else if (spaces == 2) {
                    chess_board.set_white_castle_king(true);
                }
                break;
            case '/':
                break;
            case '1' ... '8':
                if (spaces == 0) {
                    index += fen[s_index] - '0';
                }
                break;
            case ' ':
                spaces++;
                break;
            case 'w':
                if (spaces == 1) {
                    chess_board.set_turn(true);
                }
                break;
            case '-':
                if (spaces == 2) {
                    chess_board.set_white_castle_king(false);
                    chess_board.set_black_castle_king(false);
                    chess_board.set_white_castle_queen(false);
                    chess_board.set_black_castle_queen(false);
                }
                break;
            default:
                std::cerr << "Invalid FEN character: " << fen[s_index] << std::endl;
                return;
        }

        s_index++;
    }
}

// TODO: Finish implementing the entirety of the FEN-notation
std::string FenHandler::write_fen(Board &chess_board) {
    unsigned char *board = chess_board.get_board();
    int index = 0;
    int spacing = 0;
    std::string res = "";

    while (index < 64) {
        if (board[index] == Piece::none) {
            spacing++;
        } else {
            if (spacing > 0) {
                res += std::to_string(spacing);
                spacing = 0;
            }
            res += Piece::get_symbol(board[index]);
        }

        if (index % 8 == 7) {
            if (spacing > 0) {
                res += std::to_string(spacing);
                spacing = 0;
            }
            if (index != 63)
                res += "/";
        }

        index++;
    }

    res += " ";
    res += chess_board.get_turn() ? "w" : "b";
    res += " ";

    bool white_castle_king = chess_board.get_white_castle_king();
    bool white_castle_queen = chess_board.get_white_castle_queen();
    bool black_castle_king = chess_board.get_black_castle_king();
    bool black_castle_queen = chess_board.get_black_castle_queen();

    if (white_castle_king || white_castle_queen || black_castle_king || black_castle_queen) {
        res += white_castle_king ? "K" : "";
        res += white_castle_queen ? "Q" : "";
        res += black_castle_king ? "k" : "";
        res += black_castle_queen ? "q" : "";
    } else {
        res += "-";
    }

    return res;
}
