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

//tests
TEST(Movement, MovePiece) {
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
    int result1[8][8] = {
        {-4, -3, -2, -5, -6, -2, -3, -4},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 1},
        {4, 3, 2, 5, 6, 2, 3, 4},
    };

    move_piece(60, 40, board); //pawn
    SAME_BOARD(board, result1);

    move_piece(10, 20, board); //pawn
    int result2[8][8] = {
        {-4, -3, -2, -5, -6, -2, -3, -4},
        {0, -1, -1, -1, -1, -1, -1, -1},
        {-1, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 1},
        {4, 3, 2, 5, 6, 2, 3, 4},
    };
    SAME_BOARD(board, result2);

    move_piece(70, 50, board); //rook
    int result3[8][8] = {
        {-4, -3, -2, -5, -6, -2, -3, -4},
        {0, -1, -1, -1, -1, -1, -1, -1},
        {-1, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0},
        {4, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 1},
        {0, 3, 2, 5, 6, 2, 3, 4},
    };
    SAME_BOARD(board, result3);

    //castling
    int board2[8][8] = {
        {-4, 0, 0, 0, -6, -2, -3, -4},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {4, 3, 2, 5, 6, 0, 0, 4},
    };
    move_piece(4, 2, board2); //black left castle
    int result4[8][8] = {
        {0, 0, -6, -4, 0, -2, -3, -4},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {4, 3, 2, 5, 6, 0, 0, 4},
    };
    SAME_BOARD(board2, result4);

    move_piece(74, 76, board2); //white right castle
    int result5[8][8] = {
        {0, 0, -6, -4, 0, -2, -3, -4},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {4, 3, 2, 5, 0, 4, 6, 0},
    };
    SAME_BOARD(board2, result5);

    //en passant
    int board3[8][8] = {
        {-4, -3, -2, -5, -6, -2, -3, -4},
        {-1, -1, -1, 0, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0}, //en passant here
        {0, 0, 0, 1, -1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 0, 1, 1, 1, 1},
        {4, 3, 2, 5, 6, 2, 3, 4},
    };
    move_piece(33, 24, board3);
    int result6[8][8] = {
        {-4, -3, -2, -5, -6, -2, -3, -4},
        {-1, -1, -1, 0, -1, -1, -1, -1},
        {0, 0, 0, 0, 1, 0, 0, 0}, //en passant here
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 0, 1, 1, 1, 1},
        {4, 3, 2, 5, 6, 2, 3, 4},
    };
    SAME_BOARD(board3, result6);

    int board4[8][8] = {
        {-4, 0, -2, -5, -6, -2, -3, -4},
        {-1, -1, -1, 0, -1, -1, -1, -1},
        {0, 0, 0, 0, -1, 0, 0, 0}, //no en passant
        {0, 0, 0, 1, -3, 0, 0, 0}, //-3 should stay
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 0, 1, 1, 1, 1},
        {4, 3, 2, 5, 6, 2, 3, 4},
    };
    move_piece(33, 24, board4);
    int result7[8][8] = {
        {-4, 0, -2, -5, -6, -2, -3, -4},
        {-1, -1, -1, 0, -1, -1, -1, -1},
        {0, 0, 0, 0, 1, 0, 0, 0}, //no en passant
        {0, 0, 0, 0, -3, 0, 0, 0}, //-3 should stay
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 0, 1, 1, 1, 1},
        {4, 3, 2, 5, 6, 2, 3, 4},
    };
    SAME_BOARD(board4, result7);
}

TEST(Movement, Pawn) {
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
    std::vector<int> moves;
    std::vector<int> correct;
    bool w_turn = true;
    int en_passant = -1;
    int i = 6;
    int j = 0;
    get_pawn_moves(board, i, j, w_turn, moves, en_passant); //white pawn
    correct.push_back(50);
    correct.push_back(40);
    ASSERT_EQ(moves, correct);
    correct.clear();
    moves.clear();

    w_turn = false;
    i = 1;
    get_pawn_moves(board, i, j, w_turn, moves, en_passant); //black
    correct.push_back(20);
    correct.push_back(30);
    ASSERT_EQ(moves, correct);
    correct.clear();
    moves.clear();
    //only one square now
    int board2[8][8] = {
        {-4, -3, -2, -5, -6, -2, -3, -4},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 1},
        {4, 3, 2, 5, 6, 2, 3, 4},
    };
    i = 5;
    w_turn = true;
    get_pawn_moves(board2, i, j, w_turn, moves, en_passant);
    correct.push_back(40);
    ASSERT_EQ(moves, correct);
    correct.clear();
    moves.clear();

    //capute
    int board3[8][8] = {
        {-4, 0, -2, -5, -6, -2, -3, -4},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, -3, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 1},
        {4, 3, 2, 5, 6, 2, 3, 4},
    };
    get_pawn_moves(board3, i, j, w_turn, moves, en_passant);
    correct.push_back(40);
    correct.push_back(41);
    ASSERT_EQ(moves, correct);
    correct.clear();
    moves.clear();
    //en passante
    int board4[8][8] = {
        {-4, -3, -2, -5, -6, -2, -3, -4},
        {-1, -1, -1, 0, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, -1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 0, 1, 1, 1, 1},
        {4, 3, 2, 5, 6, 2, 3, 4},
    };
    en_passant = 24;
    i = 3;
    j = 3;
    get_pawn_moves(board4, i, j, w_turn, moves, en_passant);
    correct.push_back(23);
    correct.push_back(24);
    ASSERT_EQ(moves, correct);
}

TEST(Movement, Bishop) {
    //test with and bishop, add enemy stopping line of sight and ally blocking completely
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
    std::vector<int> moves;
    std::vector<int> correct;
    bool w_turn = true;
    int i = 7;
    int j = 2;
    get_bishop_moves(board, i, j, w_turn, moves);
    ASSERT_TRUE(moves.empty()); 

    int board1[8][8] = {
        {-4, -3, 0, -5, -6, -2, -3, -4},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, -2, 0, 0, 0},
        {0, 0, 0, 2, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {4, 3, 0, 5, 6, 2, 3, 4},
    };
    i = 4;
    j = 3; //white bishop
    correct.push_back(32);
    correct.push_back(21);
    correct.push_back(10);
    correct.push_back(52);
    correct.push_back(34);
    correct.push_back(54);
    get_bishop_moves(board1, i, j, w_turn, moves);
    ASSERT_EQ(moves, correct);
    correct.clear();
    moves.clear();

    int board2[8][8] = {
        {-4, -3, 0, -5, -6, -2, -3, -4},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, -2, 0, 0, 0},
        {0, 0, 0, 2, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {4, 3, 0, 5, 6, 2, 3, 4},
    };
    i = 3;
    j = 4; //black bishop
    w_turn = false;
    correct.push_back(23);
    correct.push_back(43);
    correct.push_back(25);
    correct.push_back(45);
    correct.push_back(56);
    correct.push_back(67);
    get_bishop_moves(board2, i, j, w_turn, moves);
    ASSERT_EQ(moves, correct);
}

/*Test(Movement, Knight) {

}*/