#include <vector>

#pragma once

int eval_board(int board[8][8]);

void make_best_move(int board[8][8], bool w_castle[4], bool b_castle[4], bool w_turn, int en_passant);