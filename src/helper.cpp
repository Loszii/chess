#include <iostream>
#include <unordered_map>
#include <tuple>
#include <vector>
#include "helper.h"
#include "movement.h"
#include "raylib.h"

const int SCREEN_WIDTH = 784;
const int SCREEN_HEIGHT = 784;
const int SQUARE_WIDTH = 96;
const int BEVEL = 8;
const double SCALE = 0.75;
const int FONT_SIZE = 100;

std::unordered_map<int, Texture2D> get_skins() {
    //returns a map that takes in integers representing chess pieces and returns their Texture
    /*key:
    + = white, - = black
    0: empty
    1: pawn
    2: bishop
    3: knight
    4: rook
    5: queen
    6: king*/
    std::unordered_map<int, Texture2D> skins;
    skins[1] = LoadTexture("../res/w-pawn.png");
    skins[2] = LoadTexture("../res/w-bishop.png");
    skins[3] = LoadTexture("../res/w-knight.png");
    skins[4] = LoadTexture("../res/w-rook.png");
    skins[5] = LoadTexture("../res/w-queen.png");
    skins[6] = LoadTexture("../res/w-king.png");
    skins[-1] = LoadTexture("../res/b-pawn.png");
    skins[-2] = LoadTexture("../res/b-bishop.png");
    skins[-3] = LoadTexture("../res/b-knight.png");
    skins[-4] = LoadTexture("../res/b-rook.png");
    skins[-5] = LoadTexture("../res/b-queen.png");
    skins[-6] = LoadTexture("../res/b-king.png");
    return skins;
}

std::unordered_map<int, std::tuple<int, int>> get_coord() {
    //makes the coord, a map of 2d indices of form i*10 + j to a tuple of form (x,y)
    //this tuple represents the 2d-array squares physical position on the window.
    std::unordered_map<int, std::tuple<int, int>> coord;
    int x;
    int y;
    for (int i=0; i < 80; i += 10) {
        x = BEVEL;
        y = BEVEL + ((i/10) * SQUARE_WIDTH);
        for (int j=0; j < 8; j++) {
            coord[i+j] = std::make_tuple(x, y);
            x += SQUARE_WIDTH;
        }
    }
    return coord;
}

void drawGame(int board[8][8], std::unordered_map<int, std::tuple<int, int>> coord, std::unordered_map<int, Texture2D> skins, Texture2D board_texture) {
    //draws the board and then all of the pieces on it
    DrawTexture(board_texture, 0, 0, WHITE);
    int x;
    int y;
    for (int i=0; i < 80; i += 10) {
        for (int j=0; j < 8; j++) {
            if (board[i/10][j] != 0) {
                int ind = i + j;
                x = std::get<0>(coord[ind]);
                y = std::get<1>(coord[ind]);
                DrawTextureEx(skins[board[i/10][j]], (Vector2){(float)x, (float)y}, 0, SCALE, WHITE);
            }
        }
    }
}

void drawSelect(std::unordered_map<int, std::tuple<int, int>> coord, int pos, Texture2D select_texture) {
    //shades in a square at the given pos of form i*10 + j
    int x = std::get<0>(coord[pos]);
    int y = std::get<1>(coord[pos]);
    DrawTextureEx(select_texture, (Vector2){(float)x, (float)y}, 0, 1, (Color){50, 0, 0, 100});
}

int get_index(int x, int y, std::unordered_map<int, std::tuple<int, int>> coord) {
    //takes in mouse x and y pos and returns i*10 + j position
    int top;
    int bottom;
    int left;
    int right;
    for (int i=0; i < 80; i += 10) {
        for (int j=0; j < 8; j++) {
            top = std::get<1>(coord[i+j]);
            bottom = top + SQUARE_WIDTH;
            left = std::get<0>(coord[i+j]);
            right = left + SQUARE_WIDTH;
            if (left <= x && x <= right && top <= y && y <= bottom) {
                return i+j;
            }
        }
    }
    return -1; //cannot find index
}

void check_for_selection(int board[8][8], bool& select, int& select_pos, std::vector<int>& moves, bool w_turn, int pos, bool w_castle[4], bool b_castle[4], int en_passant) {
    //checks if there is a move to select at given pos, if so sets select to true, coord to pos, sets moves to all moves of that piece, all of these changes happen to original var
    int i = pos/10;
    int j = pos % 10;
    if ((w_turn && board[i][j] > 0) || (!w_turn && board[i][j] < 0)) { //pos num 0-100 representing 2d array indices
        //below code runs if there is a move to select
        select = true;
        select_pos = pos;
        moves = get_legal_moves(board, select_pos, w_castle, b_castle, w_turn, en_passant); //gets possible moves
    } else {
        select = false;
    }
}
