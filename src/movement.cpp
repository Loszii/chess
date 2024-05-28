#include <unordered_map>
#include <tuple>
#include <vector>
#include "movement.h"
#include <iostream>

void move_piece(int start_pos, int end_pos, int board[8][8]) {
    //given a start pos and end pos, moves piece at start to end
    int i = start_pos/10;
    int j = start_pos%10;
    //for castling:
    int row;
    int side;
    bool check_castle = false;
    if (board[i][j] == 6) { //is king
        row = 70;
        side = 1;
        check_castle = true;
    } else if (board[i][j] == -6) {
        row = 0;
        side = -1;
        check_castle = true;
    }
    if (check_castle) {
        if (start_pos == row + 4 && end_pos == row + 6) {
            //right castle so move rook
            board[row/10][5] = side * 4;
            board[row/10][7] = 0;
        } else if (start_pos == row + 4 && end_pos == row + 2) {
            //left castle so move rook
            board[row/10][3] = side * 4;
            board[row/10][0] = 0;
        }
    }

    //en passant
    //if pawn moving diagonal and square is empty then make en_passant move
    int dir;
    int u_bound;
    bool check_en_passant = false;
    if (board[i][j] == 1) { //white pawn
        dir = -1;
        u_bound = 0;
        check_en_passant = true;
    } else if (board[i][j] == -1) {
        dir = 1;
        u_bound = 7;
        check_en_passant = true;
    }
    if (check_en_passant) {
        if (i != u_bound) { //diags
            if (j > 0) {
                if (end_pos == (i+dir)*10 + (j-1) && board[i+dir][j-1] == 0) {
                    board[end_pos/10 - dir][end_pos%10] = 0;
                }
            }
            if (j < 7) {
                if (end_pos == (i+dir)*10 + j+1 && board[i+dir][j+1] == 0) {
                    board[end_pos/10 - dir][end_pos%10] = 0;
                }
            }
        }
    }

    //actually move piece
    board[end_pos/10][end_pos%10] = board[start_pos/10][start_pos%10];
    board[start_pos/10][start_pos%10] = 0;
}

void get_pawn_moves(int board[8][8], int i, int j, bool w_turn, std::vector<int>& moves, int en_passant) {
    int u_bound;
    int dir;
    int start_row;
    if (w_turn) {
        u_bound = 0;
        dir = -1;
        start_row = 6;
    } else {
        u_bound = 7;
        dir = 1;
        start_row = 1;
    }
    if (i == start_row && board[i+dir][j] == 0 && board[i+(2*dir)][j] == 0) {//straight
        moves.push_back(((i+dir)*10)+j);
        moves.push_back(((i+(2*dir))*10)+j);
    } else if (i != u_bound && board[i+dir][j] == 0) {
        moves.push_back(((i+dir)*10)+j);
    }
    if (i != u_bound) { //diags
        if (j > 0) {
            if (w_turn && (board[i+dir][j-1] < 0 || ((i+dir)*10) + (j-1) == en_passant)) {
                moves.push_back((i+dir)*10 + (j-1));
            } else if (!w_turn && (board[i+dir][j-1] > 0 || ((i+dir)*10) + (j-1) == en_passant)) {
                moves.push_back((i+dir)*10 + (j-1));
            }
        }
        if (j < 7) {
            if (w_turn && (board[i+dir][j+1] < 0 || ((i+dir)*10) + (j+1) == en_passant)) {
                moves.push_back((i+dir)*10 + (j+1));
            } else if (!w_turn && (board[i+dir][j+1] > 0 || ((i+dir)*10) + (j+1) == en_passant)) {
                moves.push_back((i+dir)*10 + (j+1));
            }
        }
    }
}

void get_bishop_moves(int board[8][8], int i, int j, bool w_turn, std::vector<int>& moves) {
    for (int k=1; k < j+1; k++) { //upper left
        if (i-k < 0) {
            break;
        }
        if (board[i-k][j-k] == 0) {
            moves.push_back((i-k)*10 + (j-k));
        } else if ((w_turn && board[i-k][j-k] < 0) || (!w_turn && board[i-k][j-k] > 0)) { //piece there so break after
            moves.push_back((i-k)*10 + (j-k));
            break;
        } else {
            break; //hit own piece so break
        }
    }
    for (int k=1; k < j+1; k++) { //lower left
        if (i+k > 7) {
            break;
        }
        if (board[i+k][j-k] == 0) {
            moves.push_back((i+k)*10 + (j-k));
        } else if ((w_turn && board[i+k][j-k] < 0) || (!w_turn && board[i+k][j-k] > 0)) { 
            moves.push_back((i+k)*10 + (j-k));
            break;
        } else {
            break;
        }
    }
    for (int k=1; k < (7-j)+1; k++) { //upper right
        if (i-k < 0) { //off screen
            break;
        }
        if (board[i-k][j+k] == 0) {
            moves.push_back((i-k)*10 + (j+k));
        } else if ((w_turn && board[i-k][j+k] < 0) || (!w_turn && board[i-k][j+k] > 0)) { 
            moves.push_back((i-k)*10 + (j+k));
            break;
        } else {
            break;
        }
    }

    for (int k=1; k < (7-j)+1; k++) { //lower right
        if (i+k > 7) { //off screen
            break;
        }
        if (board[i+k][j+k] == 0) {
            moves.push_back((i+k)*10 + (j+k));
        } else if ((w_turn && board[i+k][j+k] < 0) || (!w_turn && board[i+k][j+k] > 0)) { 
            moves.push_back((i+k)*10 + (j+k));
            break;
        } else {
            break;
        }
    }
}

void get_knight_moves(int board[8][8], int i, int j, bool w_turn, std::vector<int>& moves) {
    if (i > 1) {//top 2
        if (j > 0 && ((w_turn && board[i-2][j-1] <= 0) || (!w_turn && board[i-2][j-1] >= 0))) {
            moves.push_back((i-2)*10 + (j-1));
        }
        if (j < 7 && ((w_turn && board[i-2][j+1] <= 0) || (!w_turn && board[i-2][j+1] >= 0))) {
            moves.push_back((i-2)*10 + (j+1));
        }
    }
    if (j < 6) {//right 2
        if (i > 0 && ((w_turn && board[i-1][j+2] <= 0) || (!w_turn && board[i-1][j+2] >= 0))) {
            moves.push_back((i-1)*10 + (j+2));
        }
        if (i < 7 && ((w_turn && board[i+1][j+2] <= 0) || (!w_turn &&  board[i+1][j+2] >= 0))) {
            moves.push_back((i+1)*10 + (j+2));
        }
    }
    if (i < 6) {//bottom 2
        if (j > 0 && ((w_turn && board[i+2][j-1] <= 0) || (!w_turn &&  board[i+2][j-1] >= 0))) {
            moves.push_back((i+2)*10 + (j-1));
        }
        if (j < 7 && ((w_turn && board[i+2][j+1] <= 0) || (!w_turn &&  board[i+2][j+1] >= 0))) {
            moves.push_back((i+2)*10 + (j+1));
        }
    }
    if (j > 1) {//left 2
        if (i > 0 && ((w_turn && board[i-1][j-2] <= 0) || (!w_turn &&  board[i-1][j-2] >= 0))) {
            moves.push_back((i-1)*10 + (j-2));
        }
        if (i < 7 && ((w_turn && board[i+1][j-2] <= 0) || (!w_turn &&  board[i+1][j-2] >= 0))) {
            moves.push_back((i+1)*10 + (j-2));
        }
    }
}

void get_rook_moves(int board[8][8], int i, int j, bool w_turn, std::vector<int>& moves) {
    for (int k=1; k < i+1; k++) { //top col
        if (board[i-k][j] == 0) {
            moves.push_back((i-k)*10 + j);
        } else if ((w_turn && board[i-k][j] < 0) || (!w_turn && board[i-k][j] > 0)) {
            moves.push_back((i-k)*10 + j);
            break;
        } else {
            break;
        }
    }
    for (int k=1; k < (7-j)+1; k++) { //right col
        if (board[i][j+k] == 0) {
            moves.push_back(i*10 + (j+k));
        } else if ((w_turn && board[i][j+k] < 0) || (!w_turn && board[i][j+k] > 0)) {
            moves.push_back(i*10 + (j+k));
            break;
        } else {
            break;
        }
    }
    for (int k=1; k < (7-i)+1; k++) { //bottom col
        if (board[i+k][j] == 0) {
            moves.push_back((i+k)*10 + j);
        } else if ((w_turn && board[i+k][j] < 0) || (!w_turn && board[i+k][j] > 0)) {
            moves.push_back((i+k)*10 + j);
            break;
        } else {
            break;
        }
    }
    for (int k=1; k < j+1; k++) { //left col
        if (board[i][j-k] == 0) {
            moves.push_back(i*10 + (j-k));
        } else if ((w_turn && board[i][j-k] < 0) || (!w_turn && board[i][j-k] > 0)) {
            moves.push_back(i*10 + (j-k));
            break;
        } else {
            break;
        }
    }
}

void get_king_moves(int board[8][8], int i, int j, bool w_turn, std::vector<int>& moves, bool castle[4]) {
    if (i != 0) {//top row
        for (int k=-1; k < 2; k++) { //move left to right
            if (j+k >= 0 && j+k <= 7 && ((w_turn && board[i-1][j+k] <= 0) || (!w_turn && board[i-1][j+k] >= 0))) {
                moves.push_back((i-1)*10 + (j+k));
            }
        }
    }
    if (i != 7) {//bottom row
        for (int k=-1; k < 2; k++) { 
            if (j+k >= 0 && j+k <= 7 && ((w_turn && board[i+1][j+k] <= 0) || (!w_turn && board[i+1][j+k] >= 0))) {
                moves.push_back((i+1)*10 + (j+k));
            }
        }
    }
    if (j < 7 && ((w_turn && board[i][j+1] <= 0) || (!w_turn && board[i][j+1] >= 0))) {//right square
        moves.push_back(i*10 + (j+1));
    }
    if (j > 0 && ((w_turn && board[i][j-1] <= 0) || (!w_turn && board[i][j-1] >= 0))) {//left square
        moves.push_back(i*10 + (j-1));
    }

    //castling
    int row;
    if (w_turn) {
        row = 70;
    } else {
        row = 0;
    }
    if (castle[0] && castle[1]) { //right
        moves.push_back(row + 6);
    }
    if (castle[2] && castle[3]) { //left
        moves.push_back(row + 2);
    }
}

std::vector<int> get_trajectory(int board[8][8], int pos, bool w_castle[4], bool b_castle[4], int en_passant) {
    //given a board, coord sys, and square return all possible indices to move, legal or not
    //pos is a index of form i*10 + j
    //indices will be returned in same form

    std::vector<int> moves;
    int i = pos / 10;
    int j = pos % 10;
    int piece = board[i][j];

    switch (piece) {

        case -1://black pawn
            get_pawn_moves(board, i, j, false, moves, en_passant);
            break;
        
        case 1://white pawn
            get_pawn_moves(board, i, j, true, moves, en_passant);
            break;

        case -2://black bishop
            get_bishop_moves(board, i, j, false, moves);
            break;

        case 2://white bishop
            get_bishop_moves(board, i, j, true, moves);
            break;

        case -3: //black knight
            get_knight_moves(board, i, j, false, moves);
            break;
        
        case 3: //white knight
            get_knight_moves(board, i, j, true, moves);
            break;

        case -4://black rook
            get_rook_moves(board, i, j, false, moves);
            break;

        case 4://white rook
            get_rook_moves(board, i, j, true, moves);
            break;

        case -5://black queen
            get_bishop_moves(board, i, j, false, moves);
            get_rook_moves(board, i, j, false, moves);
            break;

        case 5://white queen
            get_bishop_moves(board, i, j, true, moves);
            get_rook_moves(board, i, j, true, moves);
            break;

        case -6://black king
            get_king_moves(board, i, j, false, moves, b_castle);
            break;

        case 6://white king
            get_king_moves(board, i, j, true, moves, w_castle);
            break;
    }
    return moves;
}

std::vector<int> get_all_trajectories(int board[8][8], bool w_castle[4], bool b_castle[4], bool w_turn, int en_passant) {
    //returns all possible trajectores of white or black, if w_turn is true then returns all of blacks trajectories, since 
    //this function is mainly used in calculating enemy moves and preventing psuedo legal ones
    //the return is an array of all possible moves for opponent
    std::vector<int> all_trajectories;
    for (int i=0; i < 80; i+=10) { //iterate over entire board
        for (int j=0; j < 8; j++) {
            if ((w_turn && board[i/10][j] < 0) || (!w_turn && board[i/10][j] > 0)) { //determines black or white side
                std::vector<int> trajectory = get_trajectory(board, i+j, w_castle, b_castle, en_passant); //get all piece
                for (int k=0; k < (int)trajectory.size(); k++) {
                    all_trajectories.push_back(trajectory[k]); //add to final array
                }
            }
        }
    }
    return all_trajectories;
}

std::vector<int> get_legal_moves(int board[8][8], int pos, bool w_castle[4], bool b_castle[4], bool w_turn, int en_passant) {
    //given a position in form i*10 + j of a piece, returns all legal moves of piece at pos by taking into consideration enemy trajectories
    //this function uses get_trajectory() to get current piece moves, then makes the move on a board copy and evaluates if this leaves the 
    //player in check, if so it removes it, if w_turn is true then the piece at pos must be white
    std::vector<int> legal_moves;
    std::vector<int> current_moves = get_trajectory(board, pos, w_castle, b_castle, en_passant);
    for (int i=0; i < (int)current_moves.size(); i++) {
        int board_copy[8][8];
        //copy the board
        for (int j=0; j < 8; j++) {
            for (int k=0; k < 8; k++) {
                board_copy[j][k] = board[j][k];
            }
        }
        //make move then check if enemy has vision of king, only concerned with static board
        move_piece(pos, current_moves[i], board_copy);
        if (!in_check(board_copy, w_turn)) {
            legal_moves.push_back(current_moves[i]);
        }
    }
    return legal_moves;
}

std::vector<int> get_all_legal_moves(int board[8][8], bool w_castle[4], bool b_castle[4], bool w_turn, int en_passant) {
    //returns all possible legal moves of white or black, if w_turn is true then returns all of whites legal moves
    //the return is an array of all possible moves
    std::vector<int> all_legal_moves;
    for (int i=0; i < 80; i+=10) { //iterate over entire board
        for (int j=0; j < 8; j++) {
            if ((w_turn && board[i/10][j] > 0) || (!w_turn && board[i/10][j] < 0)) { //determines black or white side
                std::vector<int> legal_moves = get_legal_moves(board, i+j, w_castle, b_castle, w_turn, en_passant); //get all piece
                for (int k=0; k < (int)legal_moves.size(); k++) {
                    all_legal_moves.push_back(legal_moves[k]); //add to final array
                }
            }
        }
    }
    return all_legal_moves;
}

std::vector<int> get_king_coord(int board[8][8]) {
    //returns array of two integer, first one being whites king second one will be blacks, ints are pos of form i*10 + j
    int white = 0;
    int black = 0;
    for (int i=0; i < 80; i+=10) { //iterate over entire board
        for (int j=0; j < 8; j++) {
            if (board[i/10][j] == 6) {
                white = i+j;
            } else if (board[i/10][j] == -6) {
                black = i+j;
            }
        }
    }
    std::vector<int> kings = {white, black};
    return kings;
}

bool in_check(int board[8][8], bool w_turn) {
    //given a board and a turn, if w_turn is true check is white is in check
    std::vector<int> king_positions = get_king_coord(board);
    bool disable_castle[4] = {false}; //a king can never take another king with check
    int disable_en_passant = -1; //an en passant can never capture a king
    std::vector<int> all_trajectories = get_all_trajectories(board, disable_castle, disable_castle, w_turn, disable_en_passant);
    int king_pos;
    if (w_turn) {
        king_pos = king_positions[0];
    } else {
        king_pos = king_positions[1];
    }
    for (int j=0; j < (int)all_trajectories.size(); j++) {
        if (all_trajectories[j] == king_pos) { //in check
            return true;
        }
    }
    return false;
}

void check_castle_conditions(int board[8][8], bool castle[4], bool w_turn) {
    //given a board and array of castling conditions, changes them dependant upon all_trajectories, here all_trajectories is the enemy pieces moves
    //for castling: have 4 variables, in array [r_temp, r_perm, l_temp, l_perm] for each side, here we are given the side whose turn it is

    bool disable_castle[4] = {false}; //a castle of opponent cannot impact other castle opportunity
    int disable_en_passant = -1; //an en passant cannot interfere with castling
    std::vector<int> all_trajectories = get_all_trajectories(board, disable_castle, disable_castle, w_turn, disable_en_passant);

    //white
    //perms
    int row;
    int side;
    if (w_turn) {
        side = 1;
        row = 7;
    } else {
        side = -1;
        row = 0;
    }
    if (board[row][4] != side * 6) { //king moved, all perm become false, dont undo these
        castle[1] = false;
        castle[3] = false;
    }
    if (board[row][7] != side * 4) { //right rook
        castle[1] = false;
    }
    if (board[row][0] != side * 4) { //left rook
        castle[3] = false;
    }

    //temps
    castle[0] = true;
    castle[2] = true;

    if (board[row][5] != 0 || board[row][6] != 0) { //piece in the way
        castle[0] = false;
    }
    if (board[row][1] != 0 || board[row][2] != 0 || board[row][3] != 0) {
        castle[2] = false;
    }

    if (castle[0] || castle[2]) { //if both are not already both false
        for (int i=0; i < (int)all_trajectories.size(); i++) { //iterate through all moves to see if a piece can attack square
                if (all_trajectories[i] == (10*row) + 4) {
                    castle[0] = false;
                    castle[2] = false;
                    break;
                }
                if (all_trajectories[i] == (10*row) + 5 || all_trajectories[i] == (10*row) + 6) {
                    castle[0] = false;
                } else if (all_trajectories[i] == (10*row) + 2 || all_trajectories[i] == (10*row) + 3) {
                    castle[2] = false;
                }
        }
    }

}

void check_game_state(int board[8][8], int& game_over, bool w_check, bool b_check, bool w_turn, int en_passant) {
    bool disable_castle[4] = {false};
    //cannot disable en_passant as it can be the only move left
    //dont need to know if can castle since if can castle then game_over can't be true (space to move and not in checks)
    std::vector<int> all_legal_moves = get_all_legal_moves(board, disable_castle, disable_castle, w_turn, en_passant);
    if ((w_turn && ((int)all_legal_moves.size() == 0 && w_check)) || (!w_turn && ((int)all_legal_moves.size() == 0 && b_check))) {
        game_over = 1; //checkmate
    } else if ((int)all_legal_moves.size() == 0) {
        game_over = 2; //stalemate
    }
}

int check_en_passant(int board[8][8], int start_pos, int end_pos) {
    //return -1 if no en passant, if there is this function returns the position of square enemy pawn can take
    //of form i*10 + j

    //check if start is a pawn, then check if it is moving two sqaures. if so then return the first one in its direction
    int diff;
    if (board[end_pos/10][end_pos%10] == 1) {
        //white pawn
        diff = end_pos - start_pos;
        if (diff == -20) {
            return start_pos - 10;
        }
    } else if (board[end_pos/10][end_pos%10] == -1) {
        diff = end_pos - start_pos;
        if (diff == 20) {
            return start_pos + 10;
        }
    } else {
        return -1;
    }
    return -1;
}