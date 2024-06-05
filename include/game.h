#include <iostream>
#include <unordered_map>
#include <tuple>
#include <vector>
#include "raylib.h"

#pragma once

struct Game {

    const int SQUARE_WIDTH = 96;
    const int BEVEL = 8;
    const double SCALE = 0.75;

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
    bool w_turn = true;
    bool w_castle[4] = {false, true, false, true}; //[right temp, right perm, left temp, left perm]
    bool b_castle[4] = {false, true, false, true};
    int en_passant = -1; //-1 if no squares available, if there is then this will be the pos
    int promotion_pos = -1; //-1 if nothing promoted, else will be square of promotion
    int w_king_pos = 74;
    int b_king_pos = 4;
    bool w_check = false;
    bool b_check = false;
    int past_moves[2] = {-1, -1}; //first is start position second is last position of the previous move made on board
    std::vector<int> moves; //stores all moves for selected piece

    std::unordered_map<int, Texture2D> skins; //skin textures maps num to Texture2D
    std::unordered_map<int, std::tuple<int, int>> coord; //mapping of indices to x-y coord in window

    Texture2D board_texture;
    Texture2D select_texture;
    bool select = false;
    int select_pos = -1; //position of select in form i*10 + j

    Game();

    void draw_game();
    void draw_select(int pos, Color color);

    void check_for_selection(int pos);
    int get_index(int x, int y);
    std::vector<int> get_king_coord();
    bool in_check();
    void check_castle_conditions();
    void check_game_state();
    int check_en_passant();
    int check_pawn_promotion();
    void updater();
};