#include <unordered_map>
#include <tuple>
#include <vector>
#include "raylib.h"

#pragma once

std::vector<int> get_king_coord(int board[8][8]);

bool in_check(int board[8][8], bool w_turn);
