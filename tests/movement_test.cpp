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
    move_piece(60, 40, board); //moving single piece
    SAME_BOARD(board, result1);
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
    int board10[8][8] = {
        {-4, 0, 0, 0, -6, 0, 0, -4},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {4, 0, 0, 0, 6, 0, 0, 4},
    };
    move_piece(4, 6, board10); //black  right castle
    int result10[8][8] = {
        {-4, 0, 0, 0, 0, -4, -6, 0},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {4, 0, 0, 0, 6, 0, 0, 4},
    };
    SAME_BOARD(board10, result10);
    move_piece(74, 72, board10); //white left castle
    int result11[8][8] = {
        {-4, 0, 0, 0, 0, -4, -6, 0},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 6, 4, 0, 0, 0, 4},
    };
    SAME_BOARD(board10, result11);
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
    //en passant black
    int board20[8][8] = {
        {-4, -3, -2, -5, -6, -2, -3, -4},
        {-1, -1, -1, 0, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, -1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 0, 1, 1, 1, 1},
        {4, 3, 2, 5, 6, 2, 3, 4},
    };
    move_piece(44, 53, board20);
    int result20[8][8] = {
        {-4, -3, -2, -5, -6, -2, -3, -4},
        {-1, -1, -1, 0, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, -1, 0, 0, 0, 0},
        {1, 1, 1, 0, 1, 1, 1, 1},
        {4, 3, 2, 5, 6, 2, 3, 4},
    };
    SAME_BOARD(board20, result20);
    int board21[8][8] = {
        {-4, -3, -2, -5, -6, -2, -3, -4},
        {-1, -1, -1, 0, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, -1, 0, 0, 0},
        {0, 0, 0, 2, 0, 0, 0, 0},
        {1, 1, 1, 0, 1, 1, 1, 1},
        {4, 3, 0, 5, 6, 2, 3, 4},
    };
    move_piece(44, 53, board21);
    int result21[8][8] = {
        {-4, -3, -2, -5, -6, -2, -3, -4},
        {-1, -1, -1, 0, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, -1, 0, 0, 0, 0},
        {1, 1, 1, 0, 1, 1, 1, 1},
        {4, 3, 0, 5, 6, 2, 3, 4},
    };
    SAME_BOARD(board21, result21);
}

//for movement test going off board, capturing enemy, and being blocked by ally (white and black)
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
        {0, -1, -1, -1, -1, -1, -1, -1},
        {-1, 0, 0, 0, 0, 0, 0, 0},
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
    w_turn = false; //black
    i = 2;
    get_pawn_moves(board2, i, j, w_turn, moves, en_passant);
    correct.push_back(30);
    ASSERT_EQ(moves, correct);
    correct.clear();
    moves.clear();

    //ally piece
    int board10[8][8] = {
        {-4, -3, -2, -5, -6, -2, -3, -4},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 2, 0, -1, 0, 0},
        {0, 0, 0, 1, 0, -2, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {4, 3, 2, 5, 6, 2, 3, 4},
    };
    w_turn = true;
    i = 4;
    j = 3;
    get_pawn_moves(board10, i, j, w_turn, moves, en_passant);
    ASSERT_TRUE(moves.empty());
    w_turn = false;
    i = 3;
    j = 5;
    get_pawn_moves(board10, i, j, w_turn, moves, en_passant);
    ASSERT_TRUE(moves.empty());

    //capture
    int board3[8][8] = {
        {-4, 0, -2, -5, -6, -2, -3, -4},
        {-1, -1, -1, -1, -1, -1, -1, 0},
        {0, 0, 0, 0, 0, 0, 0, -1},
        {0, 0, 0, 0, 0, 0, 3, 0},
        {0, -3, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 1},
        {4, 3, 2, 5, 6, 2, 0, 4},
    };
    w_turn = true;
    i = 5;
    j = 0;
    get_pawn_moves(board3, i, j, w_turn, moves, en_passant);
    correct.push_back(40);
    correct.push_back(41);
    ASSERT_EQ(moves, correct);
    correct.clear();
    moves.clear();
    w_turn = false;
    i = 2;
    j = 7;
    get_pawn_moves(board3, i, j, w_turn, moves, en_passant);
    correct.push_back(37);
    correct.push_back(36);
    ASSERT_EQ(moves, correct);
    correct.clear();
    moves.clear();

    //en passante
    int board4[8][8] = {
        {-4, -3, -2, -5, -6, -2, -3, -4},
        {-1, 0, -1, 0, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, -1, 0, 0, 0},
        {0, -1, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 0, 0, 1, 1, 1, 1},
        {4, 3, 2, 5, 6, 2, 3, 4},
    };
    w_turn = true;
    en_passant = 24;
    i = 3;
    j = 3;
    get_pawn_moves(board4, i, j, w_turn, moves, en_passant);
    correct.push_back(23);
    correct.push_back(24);
    ASSERT_EQ(moves, correct);
    moves.clear();
    correct.clear();

    w_turn = false;
    en_passant = 52;
    i = 4;
    j = 1;
    get_pawn_moves(board4, i, j, w_turn, moves, en_passant);
    correct.push_back(51);
    correct.push_back(52);
    ASSERT_EQ(moves, correct);
}

TEST(Movement, Bishop) {
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

    w_turn = false;
    i = 0;
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
    w_turn = true;
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

    i = 3;
    j = 4; //black bishop
    w_turn = false;
    correct.push_back(23);
    correct.push_back(43);
    correct.push_back(25);
    correct.push_back(45);
    correct.push_back(56);
    correct.push_back(67);
    get_bishop_moves(board1, i, j, w_turn, moves);
    ASSERT_EQ(moves, correct);
}

TEST(Movement, Knight) {
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
    int j = 1;
    get_knight_moves(board, i, j, w_turn, moves);
    correct.push_back(50);
    correct.push_back(52);
    ASSERT_EQ(moves, correct); 
    correct.clear();
    moves.clear();

    w_turn = false;
    i = 0;
    get_knight_moves(board, i, j, w_turn, moves);
    correct.push_back(20);
    correct.push_back(22);
    ASSERT_EQ(moves, correct); 
    correct.clear();
    moves.clear();


    int board2[8][8] = {
        {-4, -3, -2, -5, -6, -2, -3, -4},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 3, 0, 0, 0, 0},
        {0, 0, 0, 0, -3, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {4, 0, 2, 5, 6, 2, 3, 4},
    };
    w_turn = true;
    i = 3;
    j = 3;
    get_knight_moves(board, i, j, w_turn, moves);
    correct.push_back(12);
    correct.push_back(14);
    correct.push_back(25);
    correct.push_back(45);
    correct.push_back(52);
    correct.push_back(54);
    correct.push_back(21);
    correct.push_back(41);
    ASSERT_EQ(moves, correct); 
    correct.clear();
    moves.clear();
    
    w_turn = false;
    i = 4;
    j = 4;
    get_knight_moves(board, i, j, w_turn, moves);
    correct.push_back(23);
    correct.push_back(25);
    correct.push_back(36);
    correct.push_back(56);
    correct.push_back(63);
    correct.push_back(65);
    correct.push_back(32);
    correct.push_back(52);
    ASSERT_EQ(moves, correct); 
}

TEST(Movement, Rook) {
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
    int j = 0;
    get_rook_moves(board, i, j, w_turn, moves);
    ASSERT_TRUE(moves.empty()); 

    w_turn = false;
    i = 0;
    get_rook_moves(board, i, j, w_turn, moves);
    ASSERT_TRUE(moves.empty()); 

    int board2[8][8] = {
        {0, -3, -2, -5, -6, -2, -3, -4},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, -4, 0, 0, 0},
        {0, 0, 0, 4, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {0, 3, 2, 5, 6, 2, 3, 4},
    };

    w_turn = true;
    i = 4;
    j = 3;
    get_rook_moves(board, i, j, w_turn, moves);
    correct.push_back(33);
    correct.push_back(23);
    correct.push_back(13);
    correct.push_back(44);
    correct.push_back(45);
    correct.push_back(46);
    correct.push_back(47);
    correct.push_back(53);
    correct.push_back(42);
    correct.push_back(41);
    correct.push_back(40);
    ASSERT_EQ(moves, correct);
    moves.clear();
    correct.clear();

    w_turn = false;
    i = 3;
    j = 4;
    get_rook_moves(board, i, j, w_turn, moves);
    correct.push_back(24);
    correct.push_back(35);
    correct.push_back(36);
    correct.push_back(37);
    correct.push_back(44);
    correct.push_back(54);
    correct.push_back(64);
    correct.push_back(33);
    correct.push_back(32);
    correct.push_back(31);
    correct.push_back(30);
    ASSERT_EQ(moves, correct);
}

TEST(Movement, King) {
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
    int j = 4;
    bool castle[4] = {false};
    get_king_moves(board, i, j, w_turn, moves, castle);
    ASSERT_TRUE(moves.empty()); 

    w_turn = false;
    i = 0;
    get_king_moves(board, i, j, w_turn, moves, castle);
    ASSERT_TRUE(moves.empty()); 

    //capture
    int board2[8][8] = {
        {-4, -3, -2, -5, -6, -2, -3, -4},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, -6, 0, 0, 0},
        {0, 0, 0, 6, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {4, 3, 2, 5, 6, 2, 3, 4},
    };
    w_turn = true;
    i = 4;
    j = 3;
    get_king_moves(board2, i, j, w_turn, moves, castle);
    correct.push_back(32);
    correct.push_back(33);
    correct.push_back(34);
    correct.push_back(52);
    correct.push_back(53);
    correct.push_back(54);
    correct.push_back(44);
    correct.push_back(42);
    ASSERT_EQ(moves, correct);
    moves.clear();
    correct.clear();

    w_turn = false;
    i = 3;
    j = 4;
    get_king_moves(board2, i, j, w_turn, moves, castle);
    correct.push_back(23);
    correct.push_back(24);
    correct.push_back(25);
    correct.push_back(43);
    correct.push_back(44);
    correct.push_back(45);
    correct.push_back(35);
    correct.push_back(33);
    ASSERT_EQ(moves, correct);
    moves.clear();
    correct.clear();

    //castle right
    int board3[8][8] = {
        {-4, 0, 0, 0, -6, 0, 0, -4},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, -6, 0, 0, 0},
        {0, 0, 0, 6, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {4, 0, 0, 0, 6, 0, 0, 4},
    };
    castle[0] = true;
    w_turn = true;
    i = 7;
    j = 4;
    get_king_moves(board3, i, j, w_turn, moves, castle);
    correct.push_back(75);
    correct.push_back(73);
    ASSERT_EQ(moves, correct);
    moves.clear();
    castle[1] = true;
    get_king_moves(board3, i, j, w_turn, moves, castle);
    correct.push_back(76);
    ASSERT_EQ(moves, correct);
    moves.clear();
    correct.clear();

    //black
    castle[1] = false;
    w_turn = false;
    i = 0;
    get_king_moves(board3, i, j, w_turn, moves, castle);
    correct.push_back(5);
    correct.push_back(3);
    ASSERT_EQ(moves, correct);
    moves.clear();
    castle[1] = true;
    get_king_moves(board3, i, j, w_turn, moves, castle);
    correct.push_back(6);
    ASSERT_EQ(moves, correct);
    moves.clear();
    correct.clear();
    
    //rest
    castle[0] = false;
    castle[1] = false;
    //only set one right and make sure doesnt work
    castle[2] = true;
    w_turn = true;
    i = 7;
    get_king_moves(board3, i, j, w_turn, moves, castle);
    correct.push_back(75);
    correct.push_back(73);
    ASSERT_EQ(moves, correct);
    moves.clear();
    castle[3] = true;
    get_king_moves(board3, i, j, w_turn, moves, castle);
    correct.push_back(72);
    ASSERT_EQ(moves, correct);
    moves.clear();
    correct.clear();
    castle[3] = false;

    //black left
    w_turn = false;
    i = 0;
    get_king_moves(board3, i, j, w_turn, moves, castle);
    correct.push_back(5);
    correct.push_back(3);
    ASSERT_EQ(moves, correct);
    moves.clear();
    castle[3] = true;
    get_king_moves(board3, i, j, w_turn, moves, castle);
    correct.push_back(2);
    ASSERT_EQ(moves, correct);
    moves.clear();
    correct.clear();
}

TEST(Movement, Trajectory) {
    //make sure get_trajectory == get_piece_moves at the right pos
    int board[8][8] = {
        {-4, -3, -2, -5, -6, -2, -3, -4},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {4, 3, 2, 5, 6, 2, 3, 4},
    };
    bool w_castle[4] = {true};
    bool b_castle[4] = {false};
    int en_passant = -1;
    std::vector<int> trajectory;
    std::vector<int> moves;
    bool w_turn = true;

    int pos = 50;
    trajectory =  get_trajectory(board, pos, w_castle, b_castle, en_passant);
    get_pawn_moves(board, pos/10, pos%10, w_turn, moves, en_passant);
    ASSERT_EQ(trajectory, moves);
    moves.clear();

    pos = 70;
    trajectory =  get_trajectory(board, pos, w_castle, b_castle, en_passant);
    get_rook_moves(board, pos/10, pos%10, w_turn, moves);
    ASSERT_EQ(trajectory, moves);
    moves.clear();

    pos = 71;
    trajectory =  get_trajectory(board, pos, w_castle, b_castle, en_passant);
    get_knight_moves(board, pos/10, pos%10, w_turn, moves);
    ASSERT_EQ(trajectory, moves);
    moves.clear();

    pos = 72;
    trajectory =  get_trajectory(board, pos, w_castle, b_castle, en_passant);
    get_bishop_moves(board, pos/10, pos%10, w_turn, moves);
    ASSERT_EQ(trajectory, moves);
    moves.clear();

    pos = 73;
    trajectory =  get_trajectory(board, pos, w_castle, b_castle, en_passant);
    get_bishop_moves(board, pos/10, pos%10, w_turn, moves);
    get_rook_moves(board, pos/10, pos%10, w_turn, moves);
    ASSERT_EQ(trajectory, moves);
    moves.clear();

    pos = 74;
    trajectory =  get_trajectory(board, pos, w_castle, b_castle, en_passant);
    get_king_moves(board, pos/10, pos%10, w_turn, moves, w_castle);
    ASSERT_EQ(trajectory, moves);
    moves.clear();

    w_turn = false;
    pos = 20;
    trajectory =  get_trajectory(board, pos, w_castle, b_castle, en_passant);
    get_pawn_moves(board, pos/10, pos%10, w_turn, moves, en_passant);
    ASSERT_EQ(trajectory, moves);
    moves.clear();

    pos = 0;
    trajectory =  get_trajectory(board, pos, w_castle, b_castle, en_passant);
    get_rook_moves(board, pos/10, pos%10, w_turn, moves);
    ASSERT_EQ(trajectory, moves);
    moves.clear();

    pos = 1;
    trajectory =  get_trajectory(board, pos, w_castle, b_castle, en_passant);
    get_knight_moves(board, pos/10, pos%10, w_turn, moves);
    ASSERT_EQ(trajectory, moves);
    moves.clear();

    pos = 2;
    trajectory =  get_trajectory(board, pos, w_castle, b_castle, en_passant);
    get_bishop_moves(board, pos/10, pos%10, w_turn, moves);
    ASSERT_EQ(trajectory, moves);
    moves.clear();

    pos = 3;
    trajectory =  get_trajectory(board, pos, w_castle, b_castle, en_passant);
    get_bishop_moves(board, pos/10, pos%10, w_turn, moves);
    get_rook_moves(board, pos/10, pos%10, w_turn, moves);
    ASSERT_EQ(trajectory, moves);
    moves.clear();

    pos = 4;
    trajectory =  get_trajectory(board, pos, w_castle, b_castle, en_passant);
    get_king_moves(board, pos/10, pos%10, w_turn, moves, w_castle);
    ASSERT_EQ(trajectory, moves);
    moves.clear();
}

TEST(Movement, AllTrajectory) {
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
    bool w_turn = true;
    bool castle[4] = {false};
    int en_passant = -1;
    std::vector<int> all_trajectories;
    all_trajectories = get_all_trajectories(board, castle, castle, w_turn, en_passant);
    std::vector<int> correct;
    correct.push_back(20);
    correct.push_back(22);
    correct.push_back(25);
    correct.push_back(27);
    correct.push_back(20);
    correct.push_back(30);
    correct.push_back(21);
    correct.push_back(31);
    correct.push_back(22);
    correct.push_back(32);
    correct.push_back(23);
    correct.push_back(33);
    correct.push_back(24);
    correct.push_back(34);
    correct.push_back(25);
    correct.push_back(35);
    correct.push_back(26);
    correct.push_back(36);
    correct.push_back(27);
    correct.push_back(37);
    ASSERT_EQ(all_trajectories, correct);
    correct.clear();

    w_turn = false;
    all_trajectories = get_all_trajectories(board, castle, castle, w_turn, en_passant);
    correct.push_back(50);
    correct.push_back(40);
    correct.push_back(51);
    correct.push_back(41);
    correct.push_back(52);
    correct.push_back(42);
    correct.push_back(53);
    correct.push_back(43);
    correct.push_back(54);
    correct.push_back(44);
    correct.push_back(55);
    correct.push_back(45);
    correct.push_back(56);
    correct.push_back(46);
    correct.push_back(57);
    correct.push_back(47);
    correct.push_back(50);
    correct.push_back(52);
    correct.push_back(55);
    correct.push_back(57);
    ASSERT_EQ(all_trajectories, correct);
}

TEST(Movement, Legal) {
    //needs to be most extensive test
    //some cases to consider:
    /*
    1. pieces pinned to king
    2. king moving into check
    3. moving pieces while in check that do not block
    4. moving pieces while in check that do block
    5. en passant to reveal king by eating pawn blocking check via enemy piece
    6. make sure it doesnt stop any moves that are actually legal (taking piece to block check) (or just moving normally)
    */

    //get_legal_moves(int board[8][8], int pos, bool w_castle[4], bool b_castle[4], bool w_turn, int en_passant)
}