#include <iostream>
#include <unordered_map>
#include <tuple>
#include <vector>
#include <array>
#include <limits>
#include "game.h"

const int MAX_INT = std::numeric_limits<int>::max();
const int MIN_INT = std::numeric_limits<int>::min();

//actually making the move
void Game::engine_move() {
    //makes engines move
    std::array<int, 2> alg_moves = minimax(3);
    update_board(alg_moves[0], alg_moves[1]);
    check_draw();
    check_game_over();
}

//eval stuff
void Game::init_piece_val() {
    piece_val[0] = 0; //incase call with empty sqaure
    piece_val[1] = 10;
    piece_val[-1] = -10;
    piece_val[2] = 30;
    piece_val[-2] = -30;
    piece_val[3] = 30;
    piece_val[-3] = -30;
    piece_val[4] = 50;
    piece_val[-4] = -50;
    piece_val[5] = 90;
    piece_val[-5] = -90;
}
/*
std::array<int, 64> scale = {0, 0, 0, 0, 0, 0, 0, 0,
                        0, 1, 1, 1, 1, 1, 1, 0,
                        0, 1, 1, 1, 1, 1, 1, 0,
                        0, 1, 1, 2, 2, 1, 1, 0,
                        0, 1, 1, 2, 2, 1, 1, 0,
                        0, 1, 1, 1, 1, 1, 1, 0,
                        0, 1, 1, 1, 1, 1, 1, 0,
                        0, 0, 0, 0, 0, 0, 0, 0};*/

int Game::evaluate_board() {
    int score = 0;
    for (int i=0;i < 64; i++) {
        if (board.data[i] != 0) {
            score += piece_val[board.data[i]];
        }
    }
    return score;
}

//algorithm
std::array<int, 2> Game::minimax(int depth) {
    //returns an array [start_pos, end_pos] of best move to make for current players turn
    std::array<int, 2> result;
    int best;
    int eval;
    std::vector<std::vector<int>> moves = get_all_legal_moves();
    if (board.w_turn) {
        best = MIN_INT;
        for (int i=0; i < (int)moves.size(); i++) {
            for (int j=1; j < (int)moves[i].size(); j++) {
                Board old_board = update_board(moves[i][0], moves[i][j]);
                
                if (is_hash_limit()) { //hashing this board will result in a draw
                    eval = 0;
                } else {
                    eval = minimax_helper(depth-1); //recursively make more moves till depth reached
                }
                best = std::max(best, eval);

                undo_update_board(old_board);
                if (eval == best) {  //this was best run so far
                    result = {moves[i][0], moves[i][j]};
                }
            }
        }
    } else {
        best = MAX_INT;
        for (int i=0; i < (int)moves.size(); i++) {
            for (int j=1; j < (int)moves[i].size(); j++) {
                Board old_board = update_board(moves[i][0], moves[i][j]);

                if (is_hash_limit()) {
                    eval = 0;
                } else {
                    eval = minimax_helper(depth-1);
                }
                best = std::min(best, eval);

                undo_update_board(old_board);
                if (eval == best) {  //this was best run so far
                    result = {moves[i][0], moves[i][j]};
                }
            }
        }
    }
    return result;
}

int Game::minimax_helper(int depth) {
    //returns the min or max of the deeper calls/eval boards
    if (depth == 0) {
        return evaluate_board();
    }
    int best;
    int eval;
    std::vector<std::vector<int>> moves = get_all_legal_moves();
    if (board.w_turn) {
        if (moves.empty()) { //reached a stalemate
            if (!board.w_check) {
                return 0;
            }
        }
        best = MIN_INT;
        for (int i=0; i < (int)moves.size(); i++) {
            for (int j=1; j < (int)moves[i].size(); j++) {
                Board old_board = update_board(moves[i][0], moves[i][j]);

                eval = minimax_helper(depth-1);
                best = std::max(best, eval);

                undo_update_board(old_board);
            }
        }
    } else {
        if (moves.empty()) { //reached a stalemate
            if (!board.b_check) {
                return 0;
            }
        }
        best = MAX_INT;
        for (int i=0; i < (int)moves.size(); i++) {
            for (int j=1; j < (int)moves[i].size(); j++) {
                Board old_board = update_board(moves[i][0], moves[i][j]);

                eval = minimax_helper(depth-1);
                best = std::min(best, eval);

                undo_update_board(old_board);
            }
        }
    }
    return best;
}