#include <gtest/gtest.h>
#include <vector>
#include <array>
#include "game.h"
#include "board.h"

//https://www.chessprogramming.org/Perft_Results

//possibly split all positions into differnet TEST cases

typedef unsigned long long u64;

u64 perft(Game& game, int depth) {
    std::vector<std::vector<int>> moves = game.get_all_legal_moves();
    u64 nodes = 0;
    if (depth == 1) {
        for (int i=0; i < (int)moves.size(); i++) {
            for (int j= 1; j < (int)moves[i].size(); j++) {
                nodes += 1;
            }
        }
        return (u64)nodes;
    }
    for (int i=0; i < (int)moves.size(); i++) {
        for (int j=1; j < (int)moves[i].size(); j++) {
            Board old_board = game.update_board(moves[i][0], moves[i][j]);
            nodes += perft(game, depth-1);
            game.undo_update_board(old_board);
        }
    }
    return nodes;
}


TEST(Game, Perft) {
    //a debug function counting all possible board states from a position, the positions here are from the chess debug wiki
    //initial position
    Game game = Game(false); //false to not load any textures
    ASSERT_EQ(perft(game, 4), (u64)197281);

    //position 2
    game.board = Board(); //reset the board
    game.board.data = {{{-4, 0, 0, 0, -6, 0, 0, -4},
                   {-1, 0, -1, -1, -5, -1, -2, 0},
                   {-2, -3, 0, 0, -1, -3, -1, 0},
                   {0, 0, 0, 1, 3, 0, 0, 0},
                   {0, -1, 0, 0, 1, 0, 0, 0},
                   {0, 0, 3, 0, 0, 5, 0, -1},
                   {1, 1, 1, 2, 2, 1, 1, 1},
                   {4, 0, 0, 0, 6, 0, 0, 4}}};
    game.board.w_castle = {true, true, true, true};
    game.board.b_castle = {true, true, true, true};
    ASSERT_EQ(perft(game, 4), (u64)4085603);

    //position 3
    game.board = Board();
    game.board.data = {{{0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, -1, 0, 0, 0, 0, 0},
                {0, 0, 0, -1, 0, 0, 0, 0},
                {6, 1, 0, 0, 0, 0, 0, -4},
                {0, 4, 0, 0, 0, -1, 0, -6},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 1, 0, 1, 0},
                {0, 0, 0, 0, 0, 0, 0, 0}}};
    game.board.w_castle = {false, false, false, false};
    game.board.b_castle = {false, false, false, false};
    ASSERT_EQ(perft(game, 4), (u64)43238);

    //position 4
    game.board = Board();
    game.board.data = {{{-4, 0, 0, 0, -6, 0, 0, -4},
                {1, -1, -1, -1, 0, -1, -1, -1},
                {0, -2, 0, 0, 0, -3, -2, 3},
                {-3, 1, 0, 0, 0, 0, 0, 0},
                {2, 2, 1, 0, 1, 0, 0, 0},
                {-5, 0, 0, 0, 0, 3, 0, 0},
                {1, -1, 0, 1, 0, 0, 1, 1},
                {4, 0, 0, 5, 0, 4, 6, 0}}};
    game.board.w_castle = {false, false, false, false};
    game.board.b_castle = {true, true, true, true};
    ASSERT_EQ(perft(game, 4), (u64)422333);

    //position 5
    game.board = Board();
    game.board.data = {{{-4, -3, -2, -5, 0, -6, -0, -4},
                        {-1, -1, 0, 1, -2, -1, -1, -1},
                        {0, 0, -1, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 2, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0},
                        {1, 1, 1, 0, 3, -3, 1, 1},
                        {4, 3, 2, 5, 6, 0, 0, 4},}};
    game.board.w_castle = {true, true, false, true};
    game.board.b_castle = {false, false, false, false};
    ASSERT_EQ(perft(game, 4), (u64)2103487);

    //position 6
    game.board = Board();
    game.board.data = {{{-4, 0, 0, 0, 0, -4, -6, 0},
                        {0, -1, -1, 0, -5, -1, -1, -1},
                        {-1, 0, -3, -1, 0, -3, 0, 0},
                        {0, 0, -2, 0, -1, 0, 2, 0},
                        {0, 0, 2, 0, 1, 0, -2, 0},
                        {1, 0, 3, 1, 0, 3, 0, 0},
                        {0, 1, 1, 0, 5, 1, 1, 1},
                        {4, 0, 0, 0, 0, 4, 6, 0},}};
    game.board.w_castle = {false, false, false, false};
    game.board.b_castle = {false, false, false, false};
    ASSERT_EQ(perft(game, 4), (u64)3894594);
}