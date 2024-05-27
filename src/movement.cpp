#include <unordered_map>
#include <tuple>
#include <vector>
#include "movement.h"
#include <iostream>

/*OPTIMIZE BELOW CODE, 
2. make get_all_moves only white or black's moves since we never need both at once
3. seperate this into many smaller functions, like get all trajectories and then get all moves which uses trajectories to calcualte stuff*/

void move_piece(int start_pos, int end_pos, int board[8][8]) {
    //given a start pos and end pos, moves piece at start to end

    //for castling: white first
    if (board[start_pos/10][start_pos%10] == 6) { //is king
        if (start_pos == 74 && end_pos == 76) {
            //right castle so move rook
            board[7][5] = 4;
            board[7][7] = 0;
        } else if (start_pos == 74 && end_pos == 72) {
            //left castle so move rook
            board[7][3] = 4;
            board[7][0] = 0;
        }
    }
    //black
    if (board[start_pos/10][start_pos%10] == -6) { //is king
        if (start_pos == 4 && end_pos == 6) {
            //right castle so move rook
            board[0][5] = -4;
            board[0][7] = 0;
        } else if (start_pos == 4 && end_pos == 2) {
            //left castle so move rook
            board[0][3] = -4;
            board[0][0] = 0;
        }
    }

    //actually move piece
    board[end_pos/10][end_pos%10] = board[start_pos/10][start_pos%10];
    board[start_pos/10][start_pos%10] = 0;
}

void get_pawn_moves(int board[8][8], int i, int j, bool w_turn, std::vector<int>& moves) {
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
            if (w_turn && board[i+dir][j-1] < 0) {
                moves.push_back((i+dir)*10 + (j-1));
            } else if (!w_turn && board[i+dir][j-1] > 0) {
                moves.push_back((i+dir)*10 + (j-1));
            }
        }
        if (j < 7) {
            if (w_turn && board[i+dir][j+1] < 0) {
                moves.push_back((i+dir)*10 + (j+1));
            } else if (!w_turn && board[i+dir][j+1] > 0) {
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

std::vector<int> get_trajectory(int board[8][8], int pos, bool w_castle[4], bool b_castle[4]) {
    //given a board, coord sys, and square return all possible indices to move, legal or not
    //pos is a index of form i*10 + j
    //indices will be returned in same form

    std::vector<int> moves;
    int i = pos / 10;
    int j = pos % 10;
    int piece = board[i][j];

    switch (piece) {

        case -1://black pawn
            get_pawn_moves(board, i, j, false, moves);
            break;
        
        case 1://white pawn
            get_pawn_moves(board, i, j, true, moves);
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
    
    //improve below VVVVV

    //need to do this whether they are in check or not since a player can move a piece and reveal a check
    /*
    if (legal_only) {
            std::vector<int> final_moves;
            //for each move in moves make a new copy of board, make the move, and check if still in check
            for (int k=0; k < (int)moves.size(); k++) {
                bool to_remove = false;
                int board_copy[8][8]; //copying board
                bool castle_copy[4];
                for (int l=0; l < 8; l++) {
                    for (int m=0; m < 8; m++) {
                        board_copy[l][m] = board[l][m];
                    }
                }
                for (int l=0; l < 4; l++) {
                    castle_copy[l] = castle[l];
                }

                move_piece(pos, moves[k], board_copy); //makes piece move on our copy

                //recalculate en passant and castling conditions
                //maybe make function like check_conditions() and call it here with params

                //check if board_copy has a check now
                std::vector<int> king_positions = get_king_coord(board_copy);
                int king_pos;
                if (w_turn) {
                    king_pos = king_positions[0];
                } else {
                    king_pos = king_positions[1];
                }

                //any check that is on white during blacks turn is legal since it would be game winning so we set to false to prevent further recursion
                std::vector<std::vector<int>> all_moves = get_all_moves(board_copy, false, castle_copy, !w_turn);

                //check all moves and see if kings in check, disgregarding first index signifying piece pos
                for (int l=0; l < (int)all_moves.size(); l++) {
                    for (int m=1; m < (int)all_moves[l].size(); m++) {
                        if (all_moves[l][m] == king_pos) { //in check
                            to_remove = true;
                            break;
                        }
                    }
                }
                if (!to_remove) {
                    final_moves.push_back(moves[k]); //legal move since does not result in check
                }

                return final_moves;

        }
    } else {
        return moves;
    }*/

    return moves;
}

/*
//prob remove white_check param
std::vector<std::vector<int>> get_all_moves(int board[8][8], bool legal_only, bool castle[4], bool w_turn) {
    //returns all possible moves given a board and turn, of form array of arrays were each array's first element is the piece moving (index) and rest are possible moves
    //returns all moves if legal_only false, else it returns just moves that prevent losing next turn
    std::vector<std::vector<int>> all_moves;
    for (int i=0; i < 80; i+=10) { //iterate over entire board
        for (int j=0; j < 8; j++) {
            if (board[i/10][j] != 0) { //if piece
                std::vector<int> moves;
                moves.push_back(i+j); //adding position of peice to first
                std::vector<int> temp = get_moves(board, i+j, legal_only, castle, w_turn); //getting moves for the rest of arrray
                for (int k=0; k < (int)temp.size(); k++) {
                    moves.push_back(temp[k]); //transfer to new array
                }
                all_moves.push_back(moves);
            }
        }
    }

    return all_moves;
}*/

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

//change all_moves to only be of opposing color since we only need to see their trajectories
void check_castle_conditions(int board[8][8], bool castle[4], std::vector<std::vector<int>> all_moves, bool w_turn) {
    //given a board and array of castling conditions, changes them dependant upon all_moves, here all_moves is the enemy pieces moves
    //for castling: have 4 variables, in array [r_temp, r_perm, l_temp, l_perm] for each side, here we are given the side whose turn it is

    //to do: make castling into two vectors, white and black. can split the code below in half then since each temp and perm got same indice

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
        for (int i=0; i < (int)all_moves.size(); i++) { //iterate through all moves to see if a piece can attack square
            for (int j=1; j < (int)all_moves[i].size(); j++) {
                if (all_moves[i][j] == (10*row) + 4) {
                    castle[0] = false;
                    castle[2] = false;
                    break;
                }
                if (all_moves[i][j] == (10*row) + 5 || all_moves[i][j] == (10*row) + 6) {
                    castle[0] = false;
                } else if (all_moves[i][j] == (10*row) + 2 || all_moves[i][j] == (10*row) + 3) {
                    castle[2] = false;
                }
            }
            if (!(castle[0] || castle[2])) { //both temp already evaluated to be false
                break;
            }
        }
    }

}
