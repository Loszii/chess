#include <gtest/gtest.h>
#include <unordered_map>
#include <tuple>
#include <vector>
#include "movement.h"

//helper functions
void SAME_BOARD(int board1[8][8], int board2[8][8]) {
    for (int i=0; i < 8; i++) {
        for (int j=0; j < 8; j++) {
            ASSERT_EQ(board1[i][j], board2[i][j]);
        }
    }
}

//ASSERT_EQ

//tests
TEST(Movement, Pawn) {
    EXPECT_TRUE(true);
}
