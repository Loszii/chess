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
    std::array<int, 2> alg_moves = minimax();
    update_board(alg_moves[0], alg_moves[1]);
    check_draw();
    check_game_over();
}

//evaluation and minimiax algorithm here

std::array<int, 2> Game::minimax() {
    //returns an array [start_pos, end_pos] of best move to make for current players turn
    
    //for now random
    std::vector<std::vector<int>> all_moves = get_all_legal_moves();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr1(0, (int)all_moves.size() - 1);
    int index1 = distr1(gen);
    std::uniform_int_distribution<> distr2(1, (int)all_moves[index1].size() - 1);
    int index2 = distr2(gen);
    int start_pos = all_moves[index1][0];
    int end_pos = all_moves[index1][index2];

    std::array<int, 2> result = {start_pos, end_pos};
    return result;
}