#include <unordered_map>
#include <tuple>
#include <vector>

#pragma once

void move_piece(int start_pos, int end_pos, int board[8][8]);

void get_pawn_moves(int board[8][8], int i, int j, bool w_turn, std::vector<int>& moves, int en_passant);

void get_bishop_moves(int board[8][8], int i, int j, bool w_turn, std::vector<int>& moves);

void get_knight_moves(int board[8][8], int i, int j, bool w_turn, std::vector<int>& moves);

void get_rook_moves(int board[8][8], int i, int j, bool w_turn, std::vector<int>& moves);

void get_king_moves(int board[8][8], int i, int j, bool w_turn, std::vector<int>& moves, bool castle[4]);

std::vector<int> get_trajectory(int board[8][8], int pos, bool w_castle[4], bool b_castle[4], int en_passant);

std::vector<int> get_all_trajectories(int board[8][8], bool w_castle[4], bool b_castle[4], bool w_turn, int en_passant);

std::vector<int> get_legal_moves(int board[8][8], int pos, bool w_castle[4], bool b_castle[4], bool w_turn, int en_passant);

std::vector<int> get_all_legal_moves(int board[8][8], bool w_castle[4], bool b_castle[4], bool w_turn, int en_passant);

std::vector<int> get_king_coord(int board[8][8]);

bool in_check(int board[8][8], bool w_turn);

void check_castle_conditions(int board[8][8], bool castle[4], bool w_turn);

void check_game_state(int board[8][8], int& game_over, bool w_check, bool b_check, bool w_turn, int en_passant);

int check_en_passant(int board[8][8], int start_pos, int end_pos);