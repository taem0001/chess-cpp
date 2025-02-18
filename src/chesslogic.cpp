#include "../include/chesslogic.h"

ChessLogic::ChessLogic() {
    board = new ChessBoard();
    white_turn = true;
}

ChessLogic::~ChessLogic() {
    delete board;
}

ChessBoard *ChessLogic::get_board() {
    return board;
}

bool ChessLogic::make_move(Position start, Position end) {
    Piece *piece = board->get_piece(start);

    if (!piece) {
        return false;
    }

    if (piece->get_color() != (white_turn ? PieceColor::WHITE : PieceColor::BLACK)) {
        return false;
    }

    if (piece->is_valid_move(end, *board->get_board())) {
        board->move_piece(start, end);
        white_turn = !white_turn;
        return true;
    }

    return false;
}
