#include <unordered_map>
#include <tuple>
#include <vector>

#pragma once

void move_piece(int start_pos, int end_pos, int board[8][8]);

std::vector<int> get_moves(int board[8][8], int pos, bool legal_only);

std::vector<std::vector<int>> get_all_moves(int board[8][8], bool legal_only);

std::vector<int> get_king_coord(int board[8][8]);