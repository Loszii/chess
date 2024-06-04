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

void drawSelect(std::unordered_map<int, std::tuple<int, int>> coord, int pos, Texture2D select_texture, Color color);

int get_index(int x, int y, std::unordered_map<int, std::tuple<int, int>> coord);

void check_for_selection(int board[8][8], bool& select, int& select_pos, std::vector<int>& moves, bool w_turn, int pos, bool w_castle[4], bool b_castle[4], int en_passant);

std::vector<int> get_king_coord(int board[8][8]);

bool in_check(int board[8][8], bool w_turn);

void check_castle_conditions(int board[8][8], bool castle[4], bool w_turn);

void check_game_state(int board[8][8], int& game_over, bool w_check, bool b_check, bool w_turn, int en_passant);

int check_en_passant(int board[8][8], int start_pos, int end_pos);

int check_pawn_promotion(int board[8][8]);

void updater(int board[8][8], bool w_turn, int& w_king_pos, int& b_king_pos, bool& w_check, bool& b_check, int past_moves[2], bool w_castle[4], bool b_castle[4], int& game_over, int& en_passant, int& promotion_pos);

