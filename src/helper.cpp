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

void drawSelect(std::unordered_map<int, std::tuple<int, int>> coord, int pos, Texture2D select_texture, Color color) {
    //shades in a square at the given pos of form i*10 + j
    int x = std::get<0>(coord[pos]);
    int y = std::get<1>(coord[pos]);
    DrawTextureEx(select_texture, (Vector2){(float)x, (float)y}, 0, 1, color);
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

void check_castle_conditions(int board[8][8], bool castle[4], bool w_turn) {
    //given a board and array of castling conditions, changes them dependant upon all_trajectories, here all_trajectories is the enemy pieces moves
    //for castling: have 4 variables, in array [r_temp, r_perm, l_temp, l_perm] for each side, here we are given the side whose turn it is

    bool disable_castle[4] = {false}; //a castle of opponent cannot impact other castle opportunity
    int disable_en_passant = -1; //an en passant cannot interfere with castling
    std::vector<int> all_trajectories = get_all_trajectories(board, disable_castle, disable_castle, w_turn, disable_en_passant);

    //white
    //perms
    int row;
    int side;
    if (w_turn) {
        side = 1;
        row = 7;
    } else {
        side = -1;
        row = 0;
    }
    if (board[row][4] != side * 6) { //king moved, all perm become false, dont undo these
        castle[1] = false;
        castle[3] = false;
    }
    if (board[row][7] != side * 4) { //right rook
        castle[1] = false;
    }
    if (board[row][0] != side * 4) { //left rook
        castle[3] = false;
    }

    //temps
    castle[0] = true;
    castle[2] = true;

    if (board[row][5] != 0 || board[row][6] != 0) { //piece in the way
        castle[0] = false;
    }
    if (board[row][1] != 0 || board[row][2] != 0 || board[row][3] != 0) {
        castle[2] = false;
    }

    if (castle[0] || castle[2]) { //if both are not already both false
        for (int i=0; i < (int)all_trajectories.size(); i++) { //iterate through all moves to see if a piece can attack square
                if (all_trajectories[i] == (10*row) + 4) {
                    castle[0] = false;
                    castle[2] = false;
                    break;
                }
                if (all_trajectories[i] == (10*row) + 5 || all_trajectories[i] == (10*row) + 6) {
                    castle[0] = false;
                } else if (all_trajectories[i] == (10*row) + 2 || all_trajectories[i] == (10*row) + 3) {
                    castle[2] = false;
                }
        }
    }

}

void check_game_state(int board[8][8], int& game_over, bool w_check, bool b_check, bool w_turn, int en_passant) {
    //checks if there is a checkmate/stalemate for whoevers turn it is
    bool disable_castle[4] = {false};
    int scale;
    if (w_turn) {
        scale = -1;
    } else {
        scale = 1;
    }
    //cannot disable en_passant as it can be the only move left
    //dont need to know if can castle since if can castle then game_over can't be true (space to move and not in checks)
    std::vector<std::vector<int>> all_legal_moves = get_all_legal_moves(board, disable_castle, disable_castle, w_turn, en_passant);
    if ((w_turn && ((int)all_legal_moves.size() == 0 && w_check)) || (!w_turn && ((int)all_legal_moves.size() == 0 && b_check))) {
        game_over = scale * 2; //checkmate
    } else if ((int)all_legal_moves.size() == 0) {
        game_over = scale * 1; //stalemate
    }
}

int check_en_passant(int board[8][8], int start_pos, int end_pos) {
    //return -1 if no en passant, if there is this function returns the position of square enemy pawn can take
    //of form i*10 + j

    //check if start is a pawn, then check if it is moving two sqaures. if so then return the first one in its direction
    int diff;
    if (board[end_pos/10][end_pos%10] == 1) {
        //white pawn
        diff = end_pos - start_pos;
        if (diff == -20) {
            return start_pos - 10;
        }
    } else if (board[end_pos/10][end_pos%10] == -1) {
        diff = end_pos - start_pos;
        if (diff == 20) {
            return start_pos + 10;
        }
    } else {
        return -1;
    }
    return -1;
}

int check_pawn_promotion(int board[8][8]) {
    //given a board, promotes a pawn to a queen automatically if it make it to the end of board
    //return pos of promotion or -1 if none
    for (int i=0; i < 8; i++) {
        if (board[0][i] == 1) {
            board[0][i] = 5;
            return i;
        }
        if (board[7][i] == -1) {
            board[7][i] = -5;
            return 70+i;
        }
    }
    return -1;
}

void updater(int board[8][8], bool w_turn, int& w_king_pos, int& b_king_pos, bool& w_check, bool& b_check, int past_moves[2], bool w_castle[4], bool b_castle[4], int& game_over, int& en_passant, int& promotion_pos) {
    //code to run after moving piece, updates king pos, checks, castling, en passant, and promotion
    //if called with w_turn = true then will check these conditions for white before they can move
    
    //check for pawn promotion before below since can affect check/castle
    en_passant = check_en_passant(board, past_moves[0], past_moves[1]);
    promotion_pos = check_pawn_promotion(board);
    //checks
    std::vector<int> king_coord = get_king_coord(board);
    w_king_pos = king_coord[0];
    b_king_pos = king_coord[1];
    if (w_turn) {
        w_check = in_check(board, true);
        b_check = false; //black just moved so must be false
        check_castle_conditions(board, w_castle, true);
    } else {
        w_check = false;
        b_check = in_check(board, false);
        check_castle_conditions(board, b_castle, false);
    }
    //checkmate or stalemate
    check_game_state(board, game_over, w_check, b_check, w_turn, en_passant);
}