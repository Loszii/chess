#include <array>
#include "board.h"

Board::Board() {
    data = {{
        {-4, -3, -2, -5, -6, -2, -3, -4},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {4, 3, 2, 5, 6, 2, 3, 4},
    }};
    w_turn = true;
    w_king_pos = 74;
    b_king_pos = 4;
    w_check = false;
    b_check = false;
}

Board::Board(std::array<std::array<int, 8>, 8> data, bool w_turn, int w_king_pos, int b_king_pos, bool w_check, bool b_check) {
    this->data = data;
    this->w_turn = w_turn;
    this->w_king_pos = w_king_pos;
    this->b_king_pos = b_king_pos;
    this->w_check = w_check;
    this->b_check = b_check;
}