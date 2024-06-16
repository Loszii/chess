#include <iostream>
#include <unordered_map>
#include <tuple>
#include <vector>
#include <array>
#include <limits>
#include "game.h"

//https://www.chessprogramming.org/Simplified_Evaluation_Function

const int MAX_INT = std::numeric_limits<int>::max();
const int MIN_INT = -MAX_INT;

//actually making the move
void Game::engine_move(int depth) {
    //makes engines move
    std::array<int, 2> alg_moves = get_best_move(depth);
    save_board();
    update_board(alg_moves[0], alg_moves[1]);
    check_draw();
    check_game_over();
}

//eval stuff
//whites eval tables, rotate to get blacks
const std::array<int, 64> pawn_eval = { 0,  0,  0,  0,  0,  0,  0,  0,
                                        50, 50, 50, 50, 50, 50, 50, 50,
                                        10, 10, 20, 30, 30, 20, 10, 10,
                                        5,  5, 10, 25, 25, 10,  5,  5,
                                        0,  0,  0, 20, 20,  0,  0,  0,
                                        5, -5,-10,  0,  0,-10, -5,  5,
                                        5, 10, 10,-20,-20, 10, 10,  5,
                                        0,  0,  0,  0,  0,  0,  0,  0};

const std::array<int, 64> bishop_eval = {-20,-10,-10,-10,-10,-10,-10,-20,
                                        -10,  0,  0,  0,  0,  0,  0,-10,
                                        -10,  0,  5, 10, 10,  5,  0,-10,
                                        -10,  5,  5, 10, 10,  5,  5,-10,
                                        -10,  0, 10, 10, 10, 10,  0,-10,
                                        -10, 10, 10, 10, 10, 10, 10,-10,
                                        -10,  5,  0,  0,  0,  0,  5,-10,
                                        -20,-10,-10,-10,-10,-10,-10,-20};

const std::array<int, 64> knight_eval = {-50,-40,-30,-30,-30,-30,-40,-50,
                                        -40,-20,  0,  0,  0,  0,-20,-40,
                                        -30,  0, 10, 15, 15, 10,  0,-30,
                                        -30,  5, 15, 20, 20, 15,  5,-30,
                                        -30,  0, 15, 20, 20, 15,  0,-30,
                                        -30,  5, 10, 15, 15, 10,  5,-30,
                                        -40,-20,  0,  5,  5,  0,-20,-40,
                                        -50,-40,-30,-30,-30,-30,-40,-50};

const std::array<int, 64> rook_eval = {  0,  0,  0,  0,  0,  0,  0,  0,
                                        5, 10, 10, 10, 10, 10, 10,  5,
                                        -5,  0,  0,  0,  0,  0,  0, -5,
                                        -5,  0,  0,  0,  0,  0,  0, -5,
                                        -5,  0,  0,  0,  0,  0,  0, -5,
                                        -5,  0,  0,  0,  0,  0,  0, -5,
                                        -5,  0,  0,  0,  0,  0,  0, -5,
                                        0,  0,  0,  5,  5,  0,  0,  0};

const std::array<int, 64> queen_eval = {-20,-10,-10, -5, -5,-10,-10,-20,
                                        -10,  0,  0,  0,  0,  0,  0,-10,
                                        -10,  0,  5,  5,  5,  5,  0,-10,
                                        -5,  0,  5,  5,  5,  5,  0, -5,
                                        0,  0,  5,  5,  5,  5,  0, -5,
                                        -10,  5,  5,  5,  5,  5,  0,-10,
                                        -10,  0,  5,  0,  0,  0,  0,-10,
                                        -20,-10,-10, -5, -5,-10,-10,-20};

const std::array<int, 64> king_eval = {-30,-40,-40,-50,-50,-40,-40,-30,
                                        -30,-40,-40,-50,-50,-40,-40,-30,
                                        -30,-40,-40,-50,-50,-40,-40,-30,
                                        -30,-40,-40,-50,-50,-40,-40,-30,
                                        -20,-30,-30,-40,-40,-30,-30,-20,
                                        -10,-20,-20,-20,-20,-20,-20,-10,
                                        20, 20,  0,  0,  0,  0, 20, 20,
                                        20, 30, 10,  0,  0, 10, 30, 20};

//endgame
const std::array<int, 64> eg_king_eval = {-50,-40,-30,-20,-20,-30,-40,-50,
                                        -30,-20,-10,  0,  0,-10,-20,-30,
                                        -30,-10, 20, 30, 30, 20,-10,-30,
                                        -30,-10, 30, 40, 40, 30,-10,-30,
                                        -30,-10, 30, 40, 40, 30,-10,-30,
                                        -30,-10, 20, 30, 30, 20,-10,-30,
                                        -30,-30,  0,  0,  0,  0,-30,-30,
                                        -50,-30,-30,-30,-30,-30,-30,-50};
           

std::array<int, 64> swap_eval_side(std::array<int, 64> position_eval) {
    //helper function to swap the evaluation board from white to black
    std::array<int, 64> result;
    for (int i=0; i < 64; i++) {
        result[i] = position_eval[63-i];
    }
    return result;
}

void Game::init_engine() {
    //sets the values for pieces and their positions

    //piece values
    piece_val[0] = 0; //incase call with empty sqaure
    piece_val[1] = 100;
    piece_val[-1] = 100;
    piece_val[2] = 330;
    piece_val[-2] = 330;
    piece_val[3] = 320;
    piece_val[-3] = 320;
    piece_val[4] = 500;
    piece_val[-4] = 500;
    piece_val[5] = 900;
    piece_val[-5] = 900;

    //piece board evaluations
    eval_val[1] =  pawn_eval;
    eval_val[-1] = swap_eval_side(pawn_eval);
    eval_val[2] = bishop_eval;
    eval_val[-2] = swap_eval_side(bishop_eval);
    eval_val[3] = knight_eval;
    eval_val[-3] = swap_eval_side(knight_eval);
    eval_val[4] = rook_eval;
    eval_val[-4] = swap_eval_side(rook_eval);
    eval_val[5] = queen_eval;
    eval_val[-5] = swap_eval_side(queen_eval);
    eval_val[6] = king_eval;
    eval_val[-6] = swap_eval_side(king_eval);

}

void Game::init_end_game() {
    //swaps the kings evaluation board to one better suited for the end game
    eval_val[6] = eg_king_eval;
    eval_val[-6] = swap_eval_side(eg_king_eval);
}

int Game::evaluate_board() {
    //returns a integer evaluation board, higher is better for current player
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
            
            if (is_prev_board()) { //returning to a previous board, can result in draw and not productive so considers slightly negative
                temp = -100;
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