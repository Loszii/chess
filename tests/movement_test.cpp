#include <gtest/gtest.h>
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

void SAME_CASTLE(bool castle1[4], bool castle2[4]) {
    for (int i=0; i < 4; i++) {
        ASSERT_EQ(castle1[i], castle2[i]);
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
    int cap_piece;
    cap_piece = move_piece(60, 40, board); //moving single piece
    SAME_BOARD(board, result1);
    ASSERT_EQ(cap_piece, 0);

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
    cap_piece = move_piece(4, 2, board2); //black left castle
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
    ASSERT_EQ(cap_piece, 0);
    cap_piece = move_piece(74, 76, board2); //white right castle
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
    ASSERT_EQ(cap_piece, 0);
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
    cap_piece =  move_piece(4, 6, board10); //black  right castle
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
    ASSERT_EQ(cap_piece, 0);
    cap_piece = move_piece(74, 72, board10); //white left castle
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
    ASSERT_EQ(cap_piece, 0);
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
    cap_piece = move_piece(33, 24, board3);
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
    ASSERT_EQ(cap_piece, 0);
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
    cap_piece = move_piece(33, 24, board4);
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
    ASSERT_EQ(cap_piece, -1);
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
    cap_piece = move_piece(44, 53, board20);
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
    ASSERT_EQ(cap_piece, 0);
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
    cap_piece = move_piece(44, 53, board21);
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
    ASSERT_EQ(cap_piece, 2);
}

TEST(Movement, UndoMove) {
    int cap_piece;
    int promotion_pos = -1;
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
    int result[8][8] = {
        {-4, -3, -2, -5, -6, -2, -3, -4},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {4, 3, 2, 5, 6, 2, 3, 4},
    };
    cap_piece = move_piece(60, 40, board);
    promotion_pos = check_pawn_promotion(board);
    undo_move(60, 40, board, cap_piece, promotion_pos);
    cap_piece = move_piece(10, 30, board);
    promotion_pos = check_pawn_promotion(board);
    undo_move(10, 30, board, cap_piece, promotion_pos);
    SAME_BOARD(board, result);

    //en passant
    int board2[8][8] = {
        {-4, -3, -2, -5, -6, -2, -3, -4},
        {-1, -1, 0, -1, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, -1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 1, 1, 1, 1, 1, 1},
        {4, 3, 2, 5, 6, 2, 3, 4},
    };
    int result2[8][8] = {
        {-4, -3, -2, -5, -6, -2, -3, -4},
        {-1, -1, 0, -1, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, -1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 1, 1, 1, 1, 1, 1},
        {4, 3, 2, 5, 6, 2, 3, 4},
    };
    cap_piece = move_piece(31, 22, board2);
    undo_move(31, 22, board2, cap_piece, promotion_pos);
    SAME_BOARD(board2, result2);

    int board3[8][8] = {
        {-4, -3, -2, -5, -6, -2, -3, -4},
        {0, -1, -1, -1, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {-1, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 1, 1, 1, 1, 1, 1},
        {4, 3, 2, 5, 6, 2, 3, 4},
    };
    int result3[8][8] = {
        {-4, -3, -2, -5, -6, -2, -3, -4},
        {0, -1, -1, -1, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {-1, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 1, 1, 1, 1, 1, 1},
        {4, 3, 2, 5, 6, 2, 3, 4},
    };
    cap_piece = move_piece(31, 20, board3);
    undo_move(31, 20, board3, cap_piece, promotion_pos);
    SAME_BOARD(board3, result3);

    int board4[8][8] = {
        {-4, -3, -2, -5, -6, -2, -3, -4},
        {-1, 0, -1, -1, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, -1, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 1, 1, 1, 1, 1},
        {4, 3, 2, 5, 6, 2, 3, 4},
    };
    int result4[8][8] = {
        {-4, -3, -2, -5, -6, -2, -3, -4},
        {-1, 0, -1, -1, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, -1, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 1, 1, 1, 1, 1},
        {4, 3, 2, 5, 6, 2, 3, 4},
    };
    cap_piece = move_piece(41, 52, board4);
    undo_move(41, 52, board4, cap_piece, promotion_pos);
    cap_piece = move_piece(41, 50, board4);
    undo_move(41, 50, board4, cap_piece, promotion_pos);
    SAME_BOARD(board4, result4);

    //capturing regularly no en passant
    int board5[8][8] = {
        {-4, -3, -2, -5, -6, 0, -3, -4},
        {-1, 0, -1, -1, -1, -1, 0, -1},
        {0, 0, 0, 0, 0, 0, -2, 0},
        {0, 0, 0, 0, 0, 1, -1, 0},
        {1, -1, 0, 0, 0, 0, 0, 0},
        {2, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 0, 1, 1},
        {4, 3, 0, 5, 6, 2, 3, 4},
    };
    int result5[8][8] = {
        {-4, -3, -2, -5, -6, 0, -3, -4},
        {-1, 0, -1, -1, -1, -1, 0, -1},
        {0, 0, 0, 0, 0, 0, -2, 0},
        {0, 0, 0, 0, 0, 1, -1, 0},
        {1, -1, 0, 0, 0, 0, 0, 0},
        {2, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 0, 1, 1},
        {4, 3, 0, 5, 6, 2, 3, 4},
    };
    cap_piece = move_piece(41, 50, board5);
    undo_move(41, 50, board5, cap_piece, promotion_pos);
    cap_piece = move_piece(35, 26, board5);
    undo_move(35, 26, board5, cap_piece, promotion_pos);
    SAME_BOARD(board5, result5);

    //castle
    int board6[8][8] = {
        {-4, 0, 0, 0, -6, 0, 0, -4},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {4, 0, 0, 0, 6, 0, 0, 4},
    };
    int result6[8][8] = {
        {-4, 0, 0, 0, -6, 0, 0, -4},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {4, 0, 0, 0, 6, 0, 0, 4},
    };
    //white castles
    cap_piece = move_piece(74, 76, board5);
    undo_move(74, 76, board5, cap_piece, promotion_pos);
    cap_piece = move_piece(74, 72, board5);
    undo_move(74, 72, board5, cap_piece, promotion_pos);
    //black castles
    cap_piece = move_piece(4, 6, board5);
    undo_move(4, 6, board5, cap_piece, promotion_pos);
    cap_piece = move_piece(4, 2, board5);
    undo_move(4, 2, board5, cap_piece, promotion_pos);
    SAME_BOARD(board6, result6);

    //promotion
    int board7[8][8] = {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
    };
    int result7[8][8] = {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
    };
    cap_piece = move_piece(14, 4, board7);
    promotion_pos = check_pawn_promotion(board7);
    undo_move(14, 4, board7, cap_piece, promotion_pos);
    SAME_BOARD(board7, result7);

    int board8[8][8] = {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, -1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
    };
    int result8[8][8] = {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, -1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
    };
    cap_piece = move_piece(64, 74, board8);
    promotion_pos = check_pawn_promotion(board8);
    undo_move(64, 74, board8, cap_piece, promotion_pos);
    SAME_BOARD(board8, result8);

    //taking with promotion
    int board9[8][8] = {
        {0, 0, 0, -2, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
    };
    int result9[8][8] = {
        {0, 0, 0, -2, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
    };
    cap_piece = move_piece(14, 3, board9);
    promotion_pos = check_pawn_promotion(board9);
    undo_move(14, 3, board9, cap_piece, promotion_pos);
    SAME_BOARD(board9, result9);
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

    //en passant
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
    correct.clear();
    moves.clear();

    //en passant wrong side
    int board5[8][8] = {
        {-4, -3, -2, -5, -6, -2, -3, -4},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 0, 1, 1, 1, 1},
        {4, 3, 2, 5, 6, 2, 3, 4},
    };
    w_turn = true;
    en_passant = 53;
    i = 6;
    j = 2;
    get_pawn_moves(board5, i, j, w_turn, moves, en_passant);
    correct.push_back(52);
    correct.push_back(42);
    ASSERT_EQ(moves, correct);
    moves.clear();
    correct.clear();

    int board6[8][8] = {
        {-4, -3, -2, -5, -6, -2, -3, -4},
        {-1, 0, -1, -1, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, -1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {4, 3, 2, 5, 6, 2, 3, 4},
    };
    w_turn = false;
    en_passant = 21;
    i = 1;
    j = 0;
    get_pawn_moves(board6, i, j, w_turn, moves, en_passant);
    correct.push_back(20);
    correct.push_back(30);
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
    1. pieces pinned to king | DONE
    2. king moving into check | DONE
    3. moving pieces while in check that do not block | DONE
    4. moving pieces while in check that do block | DONE
    5. en passant to reveal king by eating pawn blocking check via enemy piece | DONE
    6. make sure it doesnt stop any moves that are actually legal (taking piece to block check) | DONE
    */
    int board[8][8] = {
        {0, 0, 0, 0, -6, 0, 0, 0},
        {0, 0, 0, -1, -1, -1, 0, 0},
        {0, 0, 2, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, -2, 0},
        {0, 0, 0, 1, 1, 1, 0, 0},
        {0, 0, 0, 0, 6, 0, 0, 0},
    };
    //pawn is pinned by bishop
    bool w_turn = true;
    int pos = 65;
    bool castle[4] = {false}; //legality of castling checked in other function to never leave in check
    int en_passant = -1;
    std::vector<int> legal;
    std::vector<int> correct;
    legal = get_legal_moves(board, pos, castle, castle, w_turn, en_passant);
    correct.push_back(56);
    ASSERT_EQ(legal, correct);
    correct.clear();
    w_turn = false;
    pos = 13;
    legal = get_legal_moves(board, pos, castle, castle, w_turn, en_passant);
    correct.push_back(22);
    ASSERT_EQ(legal, correct);
    correct.clear();

    int board2[8][8] = {
        {0, 0, 0, -1, -6, 0, 0, 0},
        {0, 0, 0, -4, -1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, -2, 0},
        {0, 0, 0, 0, 1, 4, 0, 0},
        {0, 0, 0, 0, 6, 1, 0, 0}, //both kings cant move into rook check
    };
    w_turn = true;
    pos = 74;
    legal = get_legal_moves(board2, pos, castle, castle, w_turn, en_passant);
    ASSERT_TRUE(legal.empty());
    w_turn = false;
    pos = 4;
    legal = get_legal_moves(board2, pos, castle, castle, w_turn, en_passant);
    ASSERT_TRUE(legal.empty());

    int board3[8][8] = {
        {0, 0, 0, 0, -6, 0, 0, 4}, //both in check, shouldnt be able to move another piece
        {0, 0, 0, 0, 0, 0, 0, 0},
        {-1, 0, 0, 0, 0, 0, 0, 0}, //pawns cant move
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 6, 0, 0, -4}, 
    };
    w_turn = true;
    pos = 50;
    legal = get_legal_moves(board3, pos, castle, castle, w_turn, en_passant);
    ASSERT_TRUE(legal.empty());
    w_turn = false;
    pos = 20;
    legal = get_legal_moves(board3, pos, castle, castle, w_turn, en_passant);
    ASSERT_TRUE(legal.empty());

    int board4[8][8] = {
        {0, 0, 0, 0, -6, 0, 0, 4}, //both in check, can block with rook
        {0, 0, 0, 0, 0, 0, -4, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 4, 0},
        {0, 0, 0, 0, 6, 0, 0, -4}, 
    };
    w_turn = true;
    pos = 66;
    correct.push_back(76);
    legal = get_legal_moves(board4, pos, castle, castle, w_turn, en_passant);
    ASSERT_EQ(correct, legal);
    correct.clear();

    w_turn = false;
    pos = 16;
    legal = get_legal_moves(board4, pos, castle, castle, w_turn, en_passant);
    correct.push_back(6);
    ASSERT_EQ(correct, legal);
    correct.clear();

    int board5[8][8] = {
        {0, 0, 0, 0, 0, 0, 0, 0}, //both in check, can block with rook
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}, //taking en passant reveals king
        {6, 1, -1, 0, 0, 0, 0, -4},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {4, 0, 0, 0, 1, -1, 0, -6}, //same for black
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}, 
    };
    en_passant = 22;
    w_turn = true;
    pos = 31;
    correct.push_back(21);
    legal = get_legal_moves(board5, pos, castle, castle, w_turn, en_passant);
    ASSERT_EQ(correct, legal);
    correct.clear();

    w_turn = false;
    en_passant = 65;
    pos = 55;
    correct.push_back(65);
    legal = get_legal_moves(board5, pos, castle, castle, w_turn, en_passant);
    ASSERT_EQ(correct, legal);
    correct.clear();
}

TEST(Movement, AllLegal) {
    //just test that first index is og piece and of correct color
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
    bool castle[4] = {false}; //legality of castling checked in other function to never leave in check
    int en_passant = -1;
    std::vector<std::vector<int>> all_legal;
    int pos;
    all_legal = get_all_legal_moves(board, castle, castle, w_turn, en_passant);
    for (int i=0; i < all_legal.size(); i++) {
        pos = all_legal[i][0];
        ASSERT_TRUE(board[pos/10][pos%10] > 0);
    }

    w_turn = false;
    all_legal = get_all_legal_moves(board, castle, castle, w_turn, en_passant);
    for (int i=0; i < all_legal.size(); i++) {
        pos = all_legal[i][0];
        ASSERT_TRUE(board[pos/10][pos%10] < 0);
    }
}

TEST(Movement, KingCoord) {
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
    std::vector<int> coords = get_king_coord(board);
    ASSERT_EQ(coords[0], 74);
    ASSERT_EQ(coords[1], 4);

    int board2[8][8] = {
        {-4, -3, -2, -5, 0, -2, -3, -4},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 6, 0, 0, 0, 0, -6, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {4, 3, 2, 5, 0, 2, 3, 4},
    };
    coords = get_king_coord(board2);
    ASSERT_EQ(coords[0], 41);
    ASSERT_EQ(coords[1], 46);
}

TEST(Movement, InCheck) {
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
    ASSERT_FALSE(in_check(board, w_turn));
    w_turn = false;
    ASSERT_FALSE(in_check(board, w_turn));

    int board2[8][8] = {
        {0, -3, -2, -5, -6, -2, -3, -4},
        {-1, -1, -1, -1, 0, -1, -1, -1},
        {0, 0, 0, 0, 4, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, -4, 0, 0, 0},
        {1, 1, 1, 1, 0, 1, 1, 1},
        {4, 3, 2, 5, 6, 2, 3, 0},
    };
    w_turn = true;
    ASSERT_TRUE(in_check(board2, w_turn));
    w_turn = false;
    ASSERT_TRUE(in_check(board2, w_turn));
}

TEST(Movement, CastleConditions) {
    //to test:
    /*1. both permanents becoming false (king move rook move) | DONE
    2. temps going false when one of squares under attack or piece in the way
    */
    //check_castle_conditions(board, castle, w_turn)
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
    bool w_castle[4] = {false, true, false, true}; //[right temp, right perm, left temp, left perm]
    bool b_castle[4] = {false, true, false, true};
    check_castle_conditions(board, w_castle, w_turn);
    bool correct[4] = {false, true, false, true};
    SAME_CASTLE(w_castle, correct);
    w_turn = false;
    check_castle_conditions(board, b_castle, w_turn);
    SAME_CASTLE(b_castle, correct);

    int board1[8][8] = { //right rooks moved
        {-4, -3, -2, -5, -6, -2, -3, 0},
        {-1, -1, -1, -1, -1, -1, -1, 0},
        {0, 0, 0, 0, 0, 0, 0, -4},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 4},
        {1, 1, 1, 1, 1, 1, 1, 0},
        {4, 3, 2, 5, 6, 2, 3, 0},
    };
    w_turn = true;
    bool correct1[4] = {false, false, false, true};
    check_castle_conditions(board1, w_castle, w_turn);
    SAME_CASTLE(w_castle, correct1);
    w_turn = false;
    check_castle_conditions(board1, b_castle, w_turn);
    SAME_CASTLE(b_castle, correct1);

    int board2[8][8] = { //left rooks moved
        {0, -3, -2, -5, -6, -2, -3, -4},
        {0, -1, -1, -1, -1, -1, -1, -1},
        {-4, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {4, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 1},
        {0, 3, 2, 5, 6, 2, 3, 4},
    };
    w_turn = true;
    bool w_castle2[4] = {false, true, false, true};
    bool b_castle2[4] = {false, true, false, true};
    bool correct2[4] = {false, true, false, false}; //left perm false
    check_castle_conditions(board2, w_castle2, w_turn);
    SAME_CASTLE(w_castle2, correct2);
    w_turn = false;
    check_castle_conditions(board2, b_castle2, w_turn);
    SAME_CASTLE(b_castle2, correct2);

    int board3[8][8] = { //kings moved
        {-4, -3, -2, -5, 0, -2, -3, -4},
        {-1, -1, -1, -1, -6, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 6, 1, 1, 1},
        {4, 3, 2, 5, 0, 2, 3, 4},
    };
    w_turn = true;
    bool w_castle3[4] = {false, true, false, true};
    bool b_castle3[4] = {false, true, false, true};
    bool correct3[4] = {false}; //left perm false
    check_castle_conditions(board3, w_castle3, w_turn);
    SAME_CASTLE(w_castle3, correct3);
    w_turn = false;
    check_castle_conditions(board3, b_castle3, w_turn);
    SAME_CASTLE(b_castle3, correct3);

    //temps
    int board4[8][8] = { //nothing in way
        {-4, 0, 0, 0, -6, 0, 0, -4},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {4, 0, 0, 0, 6, 0, 0, 4},
    };
    w_turn = true;
    bool w_castle4[4] = {false, true, false, true};
    bool b_castle4[4] = {false, true, false, true};
    bool correct4[4] = {true, true, true, true};
    check_castle_conditions(board4, w_castle4, w_turn);
    SAME_CASTLE(w_castle4, correct4);
    w_turn = false;
    check_castle_conditions(board4, b_castle4, w_turn);
    SAME_CASTLE(b_castle4, correct4);

    int board5[8][8] = { //right path blocked
        {-4, 0, 0, 0, -6, 0, 0, -4},
        {-1, -1, -1, -1, -1, 0, -1, -1},
        {0, 0, 0, 0, 0, 5, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, -5, 0, 0},
        {1, 1, 1, 1, 1, 0, 1, 1},
        {4, 0, 0, 0, 6, 0, 0, 4},
    };
    w_turn = true;
    bool w_castle5[4] = {false, true, false, true};
    bool b_castle5[4] = {false, true, false, true};
    bool correct5[4] = {false, true, true, true};
    check_castle_conditions(board5, w_castle5, w_turn);
    SAME_CASTLE(w_castle5, correct5);
    w_turn = false;
    check_castle_conditions(board5, b_castle5, w_turn);
    SAME_CASTLE(b_castle5, correct5);

    int board6[8][8] = { //left path blocked
        {-4, 0, 0, 0, -6, 0, 0, -4},
        {-1, -1, -1, 0, -1, -1, -1, -1},
        {0, 0, 0, 5, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, -5, 0, 0, 0, 0},
        {1, 1, 1, 0, 1, 1, 1, 1},
        {4, 0, 0, 0, 6, 0, 0, 4},
    };
    w_turn = true;
    bool w_castle6[4] = {false, true, false, true};
    bool b_castle6[4] = {false, true, false, true};
    bool correct6[4] = {true, true, false, true};
    check_castle_conditions(board6, w_castle6, w_turn);
    SAME_CASTLE(w_castle6, correct6);
    w_turn = false;
    check_castle_conditions(board6, b_castle6, w_turn);
    SAME_CASTLE(b_castle6, correct6);

    int board7[8][8] = { //both in check
        {-4, 0, 0, 0, -6, 0, 0, -4},
        {-1, -1, -1, -1, 0, -1, -1, -1},
        {0, 0, 0, 0, 5, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, -5, 0, 0, 0},
        {1, 1, 1, 1, 0, 1, 1, 1},
        {4, 0, 0, 0, 6, 0, 0, 4},
    };
    w_turn = true;
    bool w_castle7[4] = {false, true, false, true};
    bool b_castle7[4] = {false, true, false, true};
    bool correct7[4] = {false, true, false, true};
    check_castle_conditions(board7, w_castle7, w_turn);
    SAME_CASTLE(w_castle7, correct7);
    w_turn = false;
    check_castle_conditions(board7, b_castle7, w_turn);
    SAME_CASTLE(b_castle7, correct7);
}

TEST(Movement, GameState) {
    //check with edge cases like en passant last move to prevent state mate after pawn checks
    //check regular checkmate and stalemate of both sides
    //0 = regular , 2 = checkmate, 1 = stalemate (negative when black wins)

    //regular board
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
    int game_over = 0;
    bool w_check = false;
    bool b_check = false;
    bool w_turn = true;
    int en_passant = -1;
    check_game_state(board, game_over, w_check, b_check, w_turn, en_passant); //game over by ref
    ASSERT_EQ(game_over, 0);
    w_turn = false;
    check_game_state(board, game_over, w_check, b_check, w_turn, en_passant);
    ASSERT_EQ(game_over, 0);

    int board2[8][8] = { //black gets checkmate
        {0, 0, 0, 0, -6, 0, 0, 0},
        {0, 0, 0, 0, 5, 0, 0, 0},
        {0, 0, 0, 0, 6, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
    };
    w_turn = false;
    b_check = true;

    check_game_state(board2, game_over, w_check, b_check, w_turn, en_passant); 
    ASSERT_EQ(game_over, 2);
    game_over = 0;
    b_check = false;

    int board3[8][8] = { //white gets checkmate
        {0, 0, 0, 0, 6, 0, 0, 0},
        {0, 0, 0, 0, -5, 0, 0, 0},
        {0, 0, 0, 0, -6, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
    };
    w_turn = true;
    w_check = true;

    check_game_state(board3, game_over, w_check, b_check, w_turn, en_passant); 
    ASSERT_EQ(game_over, -2);
    game_over = 0;
    w_check = false;

    int board4[8][8] = { //black gets stalemate
        {0, 0, 0, 0, -6, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 6, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
    };
    w_turn = false;

    check_game_state(board4, game_over, w_check, b_check, w_turn, en_passant); 
    ASSERT_EQ(game_over, 1);
    game_over = 0;

    int board5[8][8] = { //white gets stalemate
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, -6, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, -6, 0, 0, 0},
        {0, 0, -1, 0, 0, 0, -1, 0},
        {0, 0, 0, 0, 6, 0, 0, 0},
    };
    w_turn = true;

    check_game_state(board5, game_over, w_check, b_check, w_turn, en_passant); 
    ASSERT_EQ(game_over, -1);
    game_over = 0;

    //en passant edge case:
    int board6[8][8] = { 
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, -1, -1, -1, 0, 0, 0},
        {0, 0, -1, -6, -1, 0, 0, 0},
        {0, -1, 1, -1, -1, 0, 0, 0},
        {0, 0, 0, 4, 1, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 2},
        {0, 0, 0, 0, 0, 0, 0, 0},
    }; //en passant only thing stopping stalemate
    en_passant = 52;
    w_turn = false;
    check_game_state(board6, game_over, w_check, b_check, w_turn, en_passant); 
    ASSERT_EQ(game_over, 0);
}

TEST(Movement, EnPassant) {
    int board[8][8] = {
        {-4, -3, -2, -5, -6, -2, -3, -4},
        {-1, -1, -1, -1, 0, -1, -1, -1},
        {0, 0, 0, 0, -1, 0, 0, 0}, //no en passant only one
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0},
        {1, 1, 1, 1, 0, 1, 1, 1},
        {4, 3, 2, 5, 6, 2, 3, 4},
    };
    int start_pos;
    int end_pos;
    start_pos = 64;
    end_pos = 64;
    ASSERT_EQ(check_en_passant(board, start_pos, end_pos), -1);
    start_pos = 14;
    end_pos = 24;
    ASSERT_EQ(check_en_passant(board, start_pos, end_pos), -1);

    int board2[8][8] = {
        {-4, -3, -2, -5, -6, -2, -3, -4},
        {-1, -1, -1, -1, 0, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0}, //both en passant
        {0, 0, 0, 0, -1, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 0, 1, 1, 1},
        {4, 3, 2, 5, 6, 2, 3, 4},
    };
    start_pos = 64;
    end_pos = 44;
    ASSERT_EQ(check_en_passant(board2, start_pos, end_pos), 54);

    start_pos = 14;
    end_pos = 34;
    ASSERT_EQ(check_en_passant(board2, start_pos, end_pos), 24);

    //check with rook instead of pawn 
    int board3[8][8] = {
        {-4, -3, -2, -5, -6, -2, -3, -4},
        {-1, -1, -1, -1, 0, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0}, //wrong piece
        {0, 0, 0, 0, -4, 0, 0, 0},
        {0, 0, 0, 0, 4, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 0, 1, 1, 1},
        {4, 3, 2, 5, 6, 2, 3, 4},
    };
    start_pos = 64;
    end_pos = 44;
    ASSERT_EQ(check_en_passant(board3, start_pos, end_pos), -1);

    start_pos = 14;
    end_pos = 34;
    ASSERT_EQ(check_en_passant(board3, start_pos, end_pos), -1);
}

TEST(Movement, PawnPromotion) {
    int board[8][8] = {
        {0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
    };
    int pos = check_pawn_promotion(board);
    int correct[8][8] = {
        {0, 0, 0, 5, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
    };
    SAME_BOARD(board, correct);
    ASSERT_EQ(pos, 3);

    int board2[8][8] = {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, -1, 0, 0, 0},
    };
    pos = check_pawn_promotion(board2);
    int correct2[8][8] = {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, -5, 0, 0, 0},
    };
    SAME_BOARD(board2, correct2);
    ASSERT_EQ(pos, 74);
}