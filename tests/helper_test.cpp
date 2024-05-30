#include <gtest/gtest.h>
#include <unordered_map>
#include <tuple>
#include <vector>
#include "helper.h"

//ASSERT_EQ

//tests
TEST(Helper, Coord) {
    std::unordered_map<int, std::tuple<int, int>> coord = get_coord();
    int x_pos;
    int y_pos;
    int x_coord;
    int y_coord;
    for (int i=0; i < 80; i+=10) {
        int x_pos=BEVEL;
        int y_pos=BEVEL + (i/10)*SQUARE_WIDTH;
        for (int j=0; j < 8; j++) {
            x_coord = std::get<0>(coord[i+j]);
            y_coord = std::get<1>(coord[i+j]);
            ASSERT_EQ(x_pos, x_coord);
            ASSERT_EQ(y_pos, y_coord);
            x_pos += SQUARE_WIDTH;
        }
    }
}

TEST(Helper, INDEX) {
    std::unordered_map<int, std::tuple<int, int>> coord = get_coord();
    ASSERT_EQ(get_index(1, 1, coord), -1); //in bevel
    ASSERT_EQ(get_index(8, 8, coord), 0); //on line between
    ASSERT_EQ(get_index(210, 210, coord), 22); //in square
    ASSERT_EQ(get_index(500, 300, coord), 35);
}

TEST(Helper, Selection) {
    int board[8][8] = {
        {-4, -3, -2, -5, -6, -2, -3, -4},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {4, 3, 2, 5, 6, 2, 3, 4},
    };
    bool select = false;
    int select_pos;
    std::vector<int> moves;
    bool w_turn = true;
    int pos = 60; //first white pawn
    bool w_castle[4] = {false, true, false, true}; 
    bool b_castle[4] = {false, true, false, true};
    int en_passant = -1;

    check_for_selection(board, select, select_pos, moves, w_turn, pos, w_castle, b_castle, en_passant);
    //should set select to true since whites turn and give back two moves
    //should also set select_pos to 60
    ASSERT_TRUE(select);
    std::vector<int> correct;
    correct.push_back(50);
    correct.push_back(40);
    ASSERT_EQ(moves, correct);
    ASSERT_EQ(select_pos, 60);

    w_turn = false; //now checking white pawn on black turn
    check_for_selection(board, select, select_pos, moves, w_turn, pos, w_castle, b_castle, en_passant);
    ASSERT_FALSE(select);

    pos = 1; //black knight
    correct.pop_back();
    correct.pop_back();
    correct.push_back(20);
    correct.push_back(22);
    check_for_selection(board, select, select_pos, moves, w_turn, pos, w_castle, b_castle, en_passant);
    ASSERT_TRUE(select);
    ASSERT_EQ(moves, correct);
    ASSERT_EQ(select_pos, 1);
}