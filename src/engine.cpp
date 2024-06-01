#include <iostream>
#include <vector>
#include <random>
#include "engine.h"
#include "movement.h"

const int evaluator[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
};

int eval_board(int board[8][8]) {
    //returns a number evaluating the board, lower better for black, bigger better for white
    return 0;
}

std::vector<int> make_best_move(int board[8][8], bool w_castle[4], bool b_castle[4], bool w_turn, int en_passant) {
    //returns an array of the start pos / end pos of best move, for now w_turn is always false since engine is always black

    //for now lets implement random moves
    std::vector<std::vector<int>> all_moves = get_all_legal_moves(board, w_castle, b_castle, w_turn, en_passant);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr1(0, (int)all_moves.size() - 1);
    int index1 = distr1(gen);
    std::uniform_int_distribution<> distr2(1, (int)all_moves[index1].size() - 1);
    int index2 = distr2(gen);
    int start_pos = all_moves[index1][0];
    int end_pos = all_moves[index1][index2];
    
    std::vector<int> result;
    result.push_back(start_pos);
    result.push_back(end_pos);

    return result;
}