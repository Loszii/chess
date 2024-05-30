#include <unordered_map>
#include <tuple>
#include <vector>
#include "raylib.h"

#pragma once

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int SQUARE_WIDTH;
extern const int BEVEL;
extern const double SCALE;
extern const int FONT_SIZE;

std::unordered_map<int, Texture2D> get_skins();

std::unordered_map<int, std::tuple<int, int>> get_coord();

void drawGame(int board[8][8], std::unordered_map<int, std::tuple<int, int>> coord, std::unordered_map<int, Texture2D> skins, Texture2D board_texture);

void drawSelect(std::unordered_map<int, std::tuple<int, int>> coord, int pos, Texture2D select_texture);

int get_index(int x, int y, std::unordered_map<int, std::tuple<int, int>> coord);

void check_for_selection(int board[8][8], bool& select, int& select_pos, std::vector<int>& moves, bool w_turn, int pos, bool w_castle[4], bool b_castle[4], int en_passant);