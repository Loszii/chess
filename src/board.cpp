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
    w_castle = {false, true, false, true};
    b_castle = {false, true, false, true};
    en_passant = -1;
}

Board::Board(std::array<std::array<int, 8>, 8> data, bool w_turn, int w_king_pos, int b_king_pos, bool w_check, bool b_check, std::array<bool, 4> w_castle, std::array<bool, 4> b_castle,
 int en_passant) {
    this->data = data;
    this->w_turn = w_turn;
    this->w_king_pos = w_king_pos;
    this->b_king_pos = b_king_pos;
    this->w_check = w_check;
    this->b_check = b_check;
    this->w_castle = w_castle;
    this->b_castle = b_castle;
    this->en_passant = en_passant;
}

bool Board::operator==(const Board& other) const {
    //improve this
    if (data == other.data && w_turn == other.w_turn && w_castle == other.w_castle && b_castle == other.b_castle && en_passant == other.en_passant) {
        return true;
    } else {
        return false;
    }
}