#include <array>
#pragma once

struct Board {

    std::array<int, 64> data; //the board
    bool w_turn = true; //turn

    //king positions and checks
    int w_king_pos;
    int b_king_pos;
    bool w_check;
    bool b_check;

    //castling conditions stored into an array
    std::array<bool, 4> w_castle; //[right temp, right perm, left temp, left perm]
    std::array<bool, 4> b_castle;

    int en_passant; //below -1 normally and set to position of en passant

    //constructors
    Board();
    Board(std::array<int, 64> data, bool w_turn, int w_king_pos, int b_king_pos, bool w_check, bool b_check, std::array<bool, 4> w_castle, std::array<bool, 4> b_castle,
    int en_passant);

    //operator overload
    bool operator==(const Board& other) const;

};