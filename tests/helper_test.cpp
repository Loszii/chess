#include <gtest/gtest.h>
#include <unordered_map>
#include <tuple>
#include <vector>
#include "helper.h"
#include "movement.h"

//helper functions
extern void SAME_BOARD(int board1[8][8], int board2[8][8]);

extern void SAME_CASTLE(bool castle1[4], bool castle2[4]);

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

TEST(Helper, Index) {
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
    correct.clear();
    correct.push_back(20);
    correct.push_back(22);
    check_for_selection(board, select, select_pos, moves, w_turn, pos, w_castle, b_castle, en_passant);
    ASSERT_TRUE(select);
    ASSERT_EQ(moves, correct);
    ASSERT_EQ(select_pos, 1);
}

TEST(Helper, KingCoord) {
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

TEST(Helper, InCheck) {
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

TEST(Helper, CastleConditions) {
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

TEST(Helper, GameState) {
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

TEST(Helper, EnPassant) {
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

TEST(Helper, PawnPromotion) {
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

TEST(Helper, Updater) {
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
    int w_king_pos = 74;
    int b_king_pos = 4;
    bool w_check = false;
    bool b_check = false;
    int past_moves[2] = {-1, -1};
    bool w_castle[4] = {false, true, false, true};
    bool b_castle[4] = {false, true, false, true};
    int game_over = 0;
    int en_passant = -1;
    int promotion_pos = -1;

    //en passant updating
    past_moves[0] = 64;
    past_moves[1] = 44;
    move_piece(64, 44, board);
    w_turn = false;
    updater(board, w_turn, w_king_pos, b_king_pos, w_check, b_check, past_moves, w_castle, b_castle, game_over, en_passant, promotion_pos);
    ASSERT_EQ(en_passant, 54);

    //check/kingpos/castle updating
    move_piece(14, 24, board); //move black pawn up
    past_moves[0] = 14;
    past_moves[1] = 24;
    w_turn = true;
    updater(board, w_turn, w_king_pos, b_king_pos, w_check, b_check, past_moves, w_castle, b_castle, game_over, en_passant, promotion_pos);
    //nothing should change
    ASSERT_EQ(en_passant, -1);
    ASSERT_EQ(promotion_pos, -1);
    ASSERT_EQ(game_over, 0);
    ASSERT_EQ(w_king_pos, 74);
    ASSERT_EQ(b_king_pos, 4);
    ASSERT_FALSE(w_check);
    ASSERT_FALSE(b_check);
    //move white king
    move_piece(74, 64, board);
    w_turn = false;
    //set w_turn to true below to avoid needing to make another move (same functionality)
    updater(board, true, w_king_pos, b_king_pos, w_check, b_check, past_moves, w_castle, b_castle, game_over, en_passant, promotion_pos);
    ASSERT_EQ(w_king_pos, 64);
    ASSERT_FALSE(w_castle[1]);
    //move black king
    move_piece(4, 14, board);
    w_turn = true;
    updater(board, false, w_king_pos, b_king_pos, w_check, b_check, past_moves, w_castle, b_castle, game_over, en_passant, promotion_pos);
    ASSERT_EQ(b_king_pos, 14);
    ASSERT_FALSE(b_castle[1]);
    //in check updating
    int board2[8][8] = {
        {0, 0, 0, 0, -6, 0, 0, 0},
        {4, 0, 0, 0, 0, 0, 0, -4}, //defend with this and check white
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 6, 0, 0, 0},
    };
    w_turn = true;
    w_king_pos = 74;
    b_king_pos = 4;
    w_check = false;
    b_check = false;
    move_piece(10, 14, board2);
    w_turn = false;
    updater(board2, w_turn, w_king_pos, b_king_pos, w_check, b_check, past_moves, w_castle, b_castle, game_over, en_passant, promotion_pos);
    ASSERT_TRUE(b_check);
    ASSERT_FALSE(w_check);
    move_piece(17, 14, board2);
    w_turn = true;
    updater(board2, w_turn, w_king_pos, b_king_pos, w_check, b_check, past_moves, w_castle, b_castle, game_over, en_passant, promotion_pos);
    ASSERT_TRUE(w_check);
    ASSERT_FALSE(b_check);

    //game state updating and promotion
    int board3[8][8] = {
        {0, 0, 0, 0, -6, 0, 0, 0},
        {1, 5, 0, 0, 0, 0, 0, 0}, //promote and checkmate
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 6, 0, 0, 0},
    };
    move_piece(10, 0, board3);
    w_turn = false;
    updater(board3, w_turn, w_king_pos, b_king_pos, w_check, b_check, past_moves, w_castle, b_castle, game_over, en_passant, promotion_pos);
    ASSERT_EQ(promotion_pos, 0);
    ASSERT_EQ(game_over, 2);

    int board4[8][8] = {
        {0, 0, 0, 0, -6, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {-1, -5, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 6, 0, 0, 0},
    };
    move_piece(60, 70, board4);
    w_turn = true;
    updater(board4, w_turn, w_king_pos, b_king_pos, w_check, b_check, past_moves, w_castle, b_castle, game_over, en_passant, promotion_pos);
    ASSERT_EQ(promotion_pos, 70);
    ASSERT_EQ(game_over, -2);
}