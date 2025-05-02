#include "../include/chessgame.h"

ChessGame::ChessGame() : logic(), running(true), draw(false), winner(false) { MoveGenerator::init(); }

void ChessGame::run_game(Bot &bot, bool bot_color) {
    bool turn;
    bool correct_pos;
    bool valid_move;
    bool five_fold;
    int from, to;
    u64 move;
    std::string player, pos;

    logic.draw_game();

    while (running) {
        five_fold = logic.fivefold_repitition();
        turn = logic.get_turn();
        std::vector<u64> moves = MoveGenerator::generate_legal_moves(logic);

        if (moves.size() == 0 && (!logic.get_singlecheck() && !logic.get_doublecheck())) {
            draw = true;
            break;
        } else if (five_fold) {
            draw = true;
            break;
        } else if (logic.fifty_move_rule()) {
            draw = true;
            break;
        } else if (moves.size() == 0 && (logic.get_singlecheck() || logic.get_doublecheck())) {
            winner = !turn;
            break;
        }

        if (bot_color == turn) {
            auto t1 = std::chrono::high_resolution_clock::now();
            move = bot.choose_move(logic, moves);
            auto t2 = std::chrono::high_resolution_clock::now();

            auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
            std::cout << "Time elapsed: " << ms_int.count() << "ms" << std::endl;
        } else {
            do {
                player = turn ? "White player's move: " : "Black player's move: ";
                std::cout << player;
                std::cin >> pos;
                correct_pos = get_pos(pos, &from, &to);
                valid_move = contains_move(moves, from, to, logic.get_totalmoves(), &move);
                int flag = get_flag(move);
                if (flag == queen_promotion || flag == rook_promotion || flag == bishop_promotion ||
                    flag == knight_promotion || flag == queen_promo_capture || flag == rook_promo_capture ||
                    flag == bishop_promo_capture || flag == knight_promo_capture) {
                    char c;
                    do {
                        std::cout << "Choose promoting piece: ";
                        std::cin >> c;
                    } while (c != 'Q' && c != 'q' && c != 'N' && c != 'n' && c != 'R' && c != 'r' && c != 'B' &&
                             c != 'b');
                    if (c == 'Q' || c == 'q') {
                        u64 from = get_from(move);
                        u64 to = get_to(move);
                        u64 flag = queen_promotion;
                        u64 total_moves = get_totalmoves(move);

                        move = define_move(from, to, flag, total_moves);
                    } else if (c == 'N' || c == 'n') {
                        u64 from = get_from(move);
                        u64 to = get_to(move);
                        u64 flag = knight_promotion;
                        u64 total_moves = get_totalmoves(move);

                        move = define_move(from, to, flag, total_moves);
                    } else if (c == 'R' || c == 'r') {
                        u64 from = get_from(move);
                        u64 to = get_to(move);
                        u64 flag = rook_promotion;
                        u64 total_moves = get_totalmoves(move);

                        move = define_move(from, to, flag, total_moves);
                    } else if (c == 'B' || c == 'b') {
                        u64 from = get_from(move);
                        u64 to = get_to(move);
                        u64 flag = bishop_promotion;
                        u64 total_moves = get_totalmoves(move);

                        move = define_move(from, to, flag, total_moves);
                    }
                }
            } while (!correct_pos || !valid_move);
        }

        logic.make_move(move);
        logic.draw_game();
    }
}

void ChessGame::run_game_bot(Bot &bot1, Bot &bot2) {
    bool turn;
    bool correct_pos;
    bool valid_move;
    bool five_fold;
    int from, to;
    u64 move;
    std::string player, pos;

    logic.draw_game();

    while (running) {
        five_fold = logic.fivefold_repitition();
        turn = logic.get_turn();
        std::vector<u64> moves = MoveGenerator::generate_legal_moves(logic);

        if (moves.size() == 0 && (!logic.get_singlecheck() && !logic.get_doublecheck())) {
            std::cout << "STALEMATE" << std::endl;
            draw = true;
            break;
        } else if (five_fold) {
            std::cout << "FIVEFOLD" << std::endl;
            draw = true;
            break;
        } else if (logic.fifty_move_rule()) {
            std::cout << "FIFTYMOVES" << std::endl;
            draw = true;
            break;
        } else if (moves.size() == 0 && (logic.get_singlecheck() || logic.get_doublecheck())) {
            winner = !turn;
            break;
        } else if (moves.size() == 0) {
            std::cout << "DEADPOS" << std::endl;
            draw = true;
            break;
        }

        if (turn) {
            move = bot1.choose_move(logic, moves);
        } else {
            move = bot2.choose_move(logic, moves);
        }

        std::cout << logic.get_halfmoves() << std::endl;

        logic.make_move(move);
        logic.draw_game();
    }
}

bool ChessGame::get_draw() { return draw; }
bool ChessGame::get_winner() { return winner; }