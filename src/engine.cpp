#include <iostream>
#include <unordered_map>
#include <tuple>
#include <vector>
#include <array>
#include <random>
#include "game.h"

//actually making the move
void Game::engine_move() {
    //makes engines move
    std::array<int, 2> alg_moves = minimax(3);
    update_board(alg_moves[0], alg_moves[1]);
    check_draw();
    check_game_over();
}

//evaluation and minimiax algorithm here

int Game::evaluate_board() {
    int score = 0;
    for (int i=0;i < 64; i++) {
        score += board.data[i];
    }
    return score;
}

std::array<int, 2> Game::minimax(int depth) {
    //returns an array [start_pos, end_pos] of best move to make for current players turn
    std::array<int, 2> result;
    int best;
    int eval;
    std::vector<std::vector<int>> moves = get_all_legal_moves();
    if (board.w_turn) {
        best = -(int)INFINITY;
        for (int i=0; i < (int)moves.size(); i++) {
            for (int j=1; j < (int)moves[i].size(); j++) {
                Board old_board = update_board(moves[i][0], moves[i][j]);
                //do something
                /*
                if (check_draw()) { //to prevent 3 repition draw when ahead / for now too slow needs work
                    eval = 0;
                } else {
                    eval = minimax_helper(depth-1);
                }*/
                eval = minimax_helper(depth-1);
                best = std::max(best, eval);

                //undo_hash_board(); //unhashes board that check_draw hashed
                undo_update_board(old_board);
                if (eval == best) {  //this was best run so far
                    result = {moves[i][0], moves[i][j]};
                }
            }
        }
    } else {
        best = (int)INFINITY;
        for (int i=0; i < (int)moves.size(); i++) {
            for (int j=1; j < (int)moves[i].size(); j++) {
                Board old_board = update_board(moves[i][0], moves[i][j]);
                //do something
                /*
                if (check_draw()) {
                    eval = 0;
                } else {
                    eval = minimax_helper(depth-1);
                }*/
                eval = minimax_helper(depth-1);
                best = std::min(best, eval);

                //undo_hash_board(); //unhashes board that check_draw hashed
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
        best = -(int)INFINITY;
        for (int i=0; i < (int)moves.size(); i++) {
            for (int j=1; j < (int)moves[i].size(); j++) {
                Board old_board = update_board(moves[i][0], moves[i][j]);
                //do something
                /*
                if (check_draw()) {
                    eval = 0;
                } else {
                    eval = minimax_helper(depth-1);
                }*/
                eval = minimax_helper(depth-1);
                best = std::max(best, eval);

                //undo_hash_board();
                undo_update_board(old_board);
            }
        }
    } else {
        if (moves.empty()) { //reached a stalemate
            if (!board.b_check) {
                return 0;
            }
        }
        best = (int)INFINITY;
        for (int i=0; i < (int)moves.size(); i++) {
            for (int j=1; j < (int)moves[i].size(); j++) {
                Board old_board = update_board(moves[i][0], moves[i][j]);
                //do something
                /*
                if (check_draw()) {
                    eval = 0;
                } else {
                    eval = minimax_helper(depth-1);
                }*/
                eval = minimax_helper(depth-1);
                best = std::min(best, eval);

                //undo_hash_board();
                undo_update_board(old_board);
            }
        }
    }

    return best;
}