#include <iostream>
#include <unordered_map>
#include <tuple>
#include <vector>
#include <array>
#include <limits>
#include "game.h"

const int MAX_INT = std::numeric_limits<int>::max();
const int MIN_INT = -MAX_INT;


//actually making the move
void Game::engine_move(int depth) {
    //makes engines move
    std::array<int, 2> alg_moves = get_best_move(depth);
    update_board(alg_moves[0], alg_moves[1]);
    check_draw();
    check_game_over();
}

//eval stuff
void Game::init_engine() {
    //piece values
    piece_val[0] = 0; //incase call with empty sqaure
    piece_val[1] = 10;
    piece_val[-1] = 10;
    piece_val[2] = 30;
    piece_val[-2] = 30;
    piece_val[3] = 30;
    piece_val[-3] = 30;
    piece_val[4] = 50;
    piece_val[-4] = 50;
    piece_val[5] = 90;
    piece_val[-5] = 90;

    //piece board evaluations
    eval_val[1] = {0, 0, 0, 0, 0, 0, 0, 0,
                   5, 5, 5, 5, 5, 5, 5, 5,
                   1, 1, 2, 3, 3, 2, 1, 1,
                   0, 0, 1, 2, 2, 1, 0, 0,
                   0, 0, 0, 2, 2, 0, 0, 0,
                   0, 0, -1, 0, 0, -1, 0, 0,
                   0, 1, 1, -2, -2, 1, 1, 0,
                   0, 0, 0, 0, 0, 0, 0, 0};

    eval_val[-1] = {0, 0, 0, 0, 0, 0, 0, 0,
                    0, 1, 1, -2, -2, 1, 1, 0,
                    0, 0, -1, 0, 0, -1, 0, 0,
                    0, 0, 0, 2, 2, 0, 0, 0,
                    0, 0, 1, 2, 2, 1, 0, 0,
                    1, 1, 2, 3, 3, 2, 1, 1,
                    5, 5, 5, 5, 5, 5, 5, 5,
                    0, 0, 0, 0, 0, 0, 0, 0};

    eval_val[2] = {-2, -1, -1, -1, -1, -1, -1, -2,
                   -1, 0, 0, 0, 0, 0, 0, -1,
                   -1, 0, 0, 1, 1, 0, 0, -1,
                   -1, 0, 0, 1, 1, 0, 0, -1,
                   -1, 0, 1, 1, 1, 1, 0, -1,
                   -1, 1, 1, 1, 1, 1, 1, -1,
                   -1, 0, 0, 0, 0, 0, 0, -1,
                   -2, -1, -1, -1, -1, -1, -1, -2};

    eval_val[-2] = {-2, -1, -1, -1, -1, -1, -1, -2,
                  -1, 0, 0, 0, 0, 0, 0, -1,
                  -1, 1, 1, 1, 1, 1, 1, -1,
                  -1, 0, 1, 1, 1, 1, 0, -1,
                  -1, 0, 0, 1, 1, 0, 0, -1,
                  -1, 0, 0, 1, 1, 0, 0, -1,
                  -1, 0, 0, 0, 0, 0, 0, -1,
                  -2, -1, -1, -1, -1, -1, -1, -2};

    eval_val[3] = {-5, -4, -3, -3, -3, -3, -4, -5,
                   -4, -2, 0, 0, 0, 0, -2, -4,
                   -3, 0, 1, 1, 1, 1, 0, -3,
                   -3, 0, 1, 2, 2, 1, 0, -3,
                   -3, 0, 1, 2, 2, 1, 0, -3,
                   -3, 0, 1, 1, 1, 1, 0, -3,
                   -4, -2, 0, 0, 0, 0, -2, -4,
                   -5, -4, -3, -3, -3, -3, -4, -5};

    eval_val[-3] = {-5, -4, -3, -3, -3, -3, -4, -5,
                   -4, -2, 0, 0, 0, 0, -2, -4,
                   -3, 0, 1, 1, 1, 1, 0, -3,
                   -3, 0, 1, 2, 2, 1, 0, -3,
                   -3, 0, 1, 2, 2, 1, 0, -3,
                   -3, 0, 1, 1, 1, 1, 0, -3,
                   -4, -2, 0, 0, 0, 0, -2, -4,
                   -5, -4, -3, -3, -3, -3, -4, -5};

    eval_val[4] = {0, 0, 0, 0, 0, 0, 0, 0,
                   0, 1, 1, 1, 1, 1, 1, 0,
                   0, 0, 0, 0, 0, 0, 0, 0,
                   0, 0, 0, 0, 0, 0, 0, 0,
                   0, 0, 0, 0, 0, 0, 0, 0,
                   0, 0, 0, 0, 0, 0, 0, 0,
                   0, 0, 0, 0, 0, 0, 0, 0,
                   0, 0, 0, 0, 0, 0, 0, 0};

    eval_val[-4] = {0, 0, 0, 0, 0, 0, 0, 0,
                   0, 0, 0, 0, 0, 0, 0, 0,
                   0, 0, 0, 0, 0, 0, 0, 0,
                   0, 0, 0, 0, 0, 0, 0, 0,
                   0, 0, 0, 0, 0, 0, 0, 0,
                   0, 0, 0, 0, 0, 0, 0, 0,
                   0, 1, 1, 1, 1, 1, 1, 0,
                   0, 0, 0, 0, 0, 0, 0, 0};

    eval_val[5] = {-2, -1, -1, 0, 0, -1, -1, -2,
                   -1, 0, 0, 0, 0, 0, 0, -1,
                   -1, 0, 0, 0, 0, 0, 0, -1,
                   0, 0, 0, 0, 0, 0, 0, 0,
                   0, 0, 0, 0, 0, 0, 0, 0,
                   -1, 0, 0, 0, 0, 0, 0, -1,
                   -1, 0, 0, 0, 0, 0, 0, -1,
                   -2, -1, -1, 0, 0, -1, -1, -2};

    eval_val[-5] = {-2, -1, -1, 0, 0, -1, -1, -2,
                   -1, 0, 0, 0, 0, 0, 0, -1,
                   -1, 0, 0, 0, 0, 0, 0, -1,
                   0, 0, 0, 0, 0, 0, 0, 0,
                   0, 0, 0, 0, 0, 0, 0, 0,
                   -1, 0, 0, 0, 0, 0, 0, -1,
                   -1, 0, 0, 0, 0, 0, 0, -1,
                   -2, -1, -1, 0, 0, -1, -1, -2};

    eval_val[6] = {-3, -4, -4, -5, -5, -4, -4, -3,
                   -3, -4, -4, -5, -5, -4, -4, -3,
                   -3, -4, -4, -5, -5, -4, -4, -3,
                   -3, -4, -4, -5, -5, -4, -4, -3,
                   -2, -3, -3, -4, -4, -3, -3, -2,
                   -1, -2, -2, -2, -2, -2, -2, -1,
                   2, 2, 0, 0, 0, 0, 2, 2,
                   2, 3, 1, 0, 0, 1, 3, 2};

    eval_val[-6] = {2, 3, 1, 0, 0, 1, 3, 2,
                    2, 2, 0, 0, 0, 0, 2, 2,
                    -1, -2, -2, -2, -2, -2, -2, -1,
                    -2, -3, -3, -4, -4, -3, -3, -2,
                    -3, -4, -4, -5, -5, -4, -4, -3,
                    -3, -4, -4, -5, -5, -4, -4, -3,
                    -3, -4, -4, -5, -5, -4, -4, -3,
                    -3, -4, -4, -5, -5, -4, -4, -3};
}

int Game::evaluate_board() {
    //returns a integer evaluation board, higher is better for given player
    int score = 0;
    if (board.w_turn) {
        for (int i=0;i < 64; i++) {
            if (board.data[i] != 0) {
                if (board.data[i] > 0) {
                    score += piece_val[board.data[i]] + eval_val[board.data[i]][i]; //add our pieces and subtract enemies
                } else {
                    score -= piece_val[board.data[i]] + eval_val[board.data[i]][i];
                }
            }
        }
    } else {
        for (int i=0;i < 64; i++) {
            if (board.data[i] != 0) {
                if (board.data[i] < 0) {
                    score += piece_val[board.data[i]] + eval_val[board.data[i]][i];
                } else {
                    score -= piece_val[board.data[i]] + eval_val[board.data[i]][i];
                }
            }
        }
    }
    return score;
}

//algorithm
int Game::negamax(int depth, int alpha, int beta) {
    //simplified minimax to search for and return the best evaluation possible
    if (depth == 0) {
        return evaluate_board();
    }
    int value = MIN_INT;
    std::vector<std::vector<int>> moves = get_all_legal_moves();

    if (moves.empty()) {
        if (!board.b_check && !board.w_check) { //no check so stalemate
            return 0;
        } //if not will return MIN_INT
    }

    for (int i=0; i < (int)moves.size(); i++) {
        for (int j=1; j < (int)moves[i].size(); j++) {
            Board old_board = update_board(moves[i][0], moves[i][j]);
            
            value = std::max(value, -negamax(depth-1, -beta, -alpha)); //recursively make more moves till depth reached
            alpha = std::max(alpha, value);

            undo_update_board(old_board);

            if (alpha >= beta) {
                return value; //prune
            }
        }
    }
    return value;
}

std::array<int, 2> Game::get_best_move(int depth) {
    //shell function for negamax that operates similarly, while saving the move corresponding to best evaluation and returning
    std::vector<std::vector<int>> moves = get_all_legal_moves();
    std::array<int, 2> result;
    int value = MIN_INT;
    int temp;

    for (int i=0; i < (int)moves.size(); i++) {
        for (int j=1; j < (int)moves[i].size(); j++) {
            Board old_board = update_board(moves[i][0], moves[i][j]);
            
            if (is_prev_board()) { //returning to a previous board, can result in draw and not productive so considers -5 for player
                temp = -5;
            } else {
                temp = -negamax(depth-1, MIN_INT, MAX_INT); //recursively make more moves till depth reached
            }

            undo_update_board(old_board);

            //add move if better than previous value
            if (temp >= value) {
                value = temp;
                result = {moves[i][0], moves[i][j]};
            }
        }
    }
    return result;
}