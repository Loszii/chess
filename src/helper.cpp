#include <iostream>
#include <unordered_map>
#include <tuple>
#include <vector>
#include "helper.h"
#include "movement.h"
#include "raylib.h"

static const int SCREEN_WIDTH = 784;
static const int SCREEN_HEIGHT = 784;
static const int SQUARE_WIDTH = 96;
static const int BEVEL = 8;
static const double SCALE = 0.75;
static const int FONT_SIZE = 100;

std::vector<int> get_king_coord(int board[8][8]) {
    //returns array of two integer, first one being whites king second one will be blacks, ints are pos of form i*10 + j
    int white = 0;
    int black = 0;
    for (int i=0; i < 80; i+=10) { //iterate over entire board
        for (int j=0; j < 8; j++) {
            if (board[i/10][j] == 6) {
                white = i+j;
            } else if (board[i/10][j] == -6) {
                black = i+j;
            }
        }
    }
    std::vector<int> kings = {white, black};
    return kings;
}

bool in_check(int board[8][8], bool w_turn) {
    //given a board and a turn, if w_turn is true check is white is in check
    std::vector<int> king_positions = get_king_coord(board);
    bool disable_castle[4] = {false}; //a king can never take another king with check
    int disable_en_passant = -1; //an en passant can never capture a king
    std::vector<int> all_trajectories = get_all_trajectories(board, disable_castle, disable_castle, w_turn, disable_en_passant);
    int king_pos;
    if (w_turn) {
        king_pos = king_positions[0];
    } else {
        king_pos = king_positions[1];
    }
    for (int j=0; j < (int)all_trajectories.size(); j++) {
        if (all_trajectories[j] == king_pos) { //in check
            return true;
        }
    }
    return false;
}