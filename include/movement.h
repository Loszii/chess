#include <unordered_map>
#include <tuple>
#include <vector>

#pragma once
std::vector<int> get_moves(int board[8][8], std::unordered_map<int, std::tuple<int, int>> coord, int pos);