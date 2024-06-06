#include <array>
#pragma once

struct Board {
    std::array<std::array<int, 8>, 8> data;
    bool w_turn = true;
    int w_king_pos;
    int b_king_pos;
    bool w_check;
    bool b_check;

    Board();
    Board(std::array<std::array<int, 8>, 8> data, bool w_turn, int w_king_pos, int b_king_pos, bool w_check, bool b_check);
};