#include <iostream>
#include <vector>
#include "game.h"

void Game::move_piece(int start_pos, int end_pos) {
    //given a start pos and end pos, moves piece at start to end
    //returns the piece that was captured if any
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
    captured_piece = board[end_pos/10][end_pos%10];
    board[end_pos/10][end_pos%10] = board[start_pos/10][start_pos%10];
    board[start_pos/10][start_pos%10] = 0;
}

void Game::undo_move(int start_pos, int end_pos) {
    //if pawn moved diagonally and piece taken is 0 then we must add back a pawn
    //if king moved two squares must un-castle
    //if pawn promoted must de-promote 

    if (promotion_pos == end_pos) {
        if (board[end_pos/10][end_pos%10] > 0) {
            board[start_pos/10][start_pos%10] = 1;
            board[end_pos/10][end_pos%10] = captured_piece;
            return;
        } else {
            board[start_pos/10][start_pos%10] = -1;
            board[end_pos/10][end_pos%10] = captured_piece;
            return;
        }
    }

    //en passant or castle
    if (captured_piece == 0) {
        if (board[end_pos/10][end_pos%10] == 1) { //is pawn that moved
            if (end_pos == start_pos - 9 || end_pos == start_pos - 11) { //diagonal
                int pawn_pos = end_pos + 10;
                board[pawn_pos/10][pawn_pos%10] = -1;
            }
        } else if (board[end_pos/10][end_pos%10] == -1) {
            if (end_pos == start_pos + 11 || end_pos == start_pos + 9) { //diagonal
                int pawn_pos = end_pos - 10;
                board[pawn_pos/10][pawn_pos%10] = 1;
            }
        }
        //castle
        if (board[end_pos/10][end_pos%10] == 6 || board[end_pos/10][end_pos%10] == -6) {
            if (end_pos - start_pos == 2) { //castled right
                int rook_pos = end_pos - 1;
                int rook_dest = end_pos + 1;
                board[rook_dest/10][rook_dest%10] = board[rook_pos/10][rook_pos%10];
                board[rook_pos/10][rook_pos%10] = 0;
            } else if (end_pos - start_pos == -2) { //castled left
                int rook_pos = end_pos + 1;
                int rook_dest = end_pos - 2;
                board[rook_dest/10][rook_dest%10] = board[rook_pos/10][rook_pos%10];
                board[rook_pos/10][rook_pos%10] = 0;
            }
        }
    }

    board[start_pos/10][start_pos%10] = board[end_pos/10][end_pos%10];
    board[end_pos/10][end_pos%10] = captured_piece;
}

void Game::get_pawn_moves(int i, int j, std::vector<int>& temp) {
    bool w_turn; //must have local turn since we can be checking enemy pawn moves
    if (board[i][j] > 0) {
        w_turn = true;
    } else {
        w_turn = false;
    }
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
        temp.push_back(((i+dir)*10)+j);
        temp.push_back(((i+(2*dir))*10)+j);
    } else if (i != u_bound && board[i+dir][j] == 0) {
        temp.push_back(((i+dir)*10)+j);
    }
    if (i != u_bound) { //diags
        if (j > 0) {
            if (w_turn && board[i+dir][j-1] < 0) {
                temp.push_back((i+dir)*10 + (j-1));
            } else if (!w_turn && board[i+dir][j-1] > 0) {
                temp.push_back((i+dir)*10 + (j-1));
            }

            //en passant
            if (w_turn && ((i+dir)*10 + (j-1) == en_passant) && en_passant/10 == 2) {
                temp.push_back((i+dir)*10 + (j-1));
            } else if (!w_turn && ((i+dir)*10 + (j-1) == en_passant) && en_passant/10 == 5) {
                temp.push_back((i+dir)*10 + (j-1));
            }
        }
        if (j < 7) {
            if (w_turn && board[i+dir][j+1] < 0) {
                temp.push_back((i+dir)*10 + (j+1));
            } else if (!w_turn && board[i+dir][j+1] > 0) {
                temp.push_back((i+dir)*10 + (j+1));
            }

            //en passant
            if (w_turn && ((i+dir)*10 + (j+1) == en_passant) && en_passant/10 == 2) {
                temp.push_back((i+dir)*10 + (j+1));
            } else if (!w_turn && ((i+dir)*10 + (j+1) == en_passant) && en_passant/10 == 5) {
                temp.push_back((i+dir)*10 + (j+1));
            }
        }
    }
}

void Game::get_bishop_moves(int i, int j, std::vector<int>& temp) {
    bool w_turn; //must have local turn since we can be checking enemy pawn moves
    if (board[i][j] > 0) {
        w_turn = true;
    } else {
        w_turn = false;
    }
    for (int k=1; k < j+1; k++) { //upper left
        if (i-k < 0) {
            break;
        }
        if (board[i-k][j-k] == 0) {
            temp.push_back((i-k)*10 + (j-k));
        } else if ((w_turn && board[i-k][j-k] < 0) || (!w_turn && board[i-k][j-k] > 0)) { //piece there so break after
            temp.push_back((i-k)*10 + (j-k));
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
            temp.push_back((i+k)*10 + (j-k));
        } else if ((w_turn && board[i+k][j-k] < 0) || (!w_turn && board[i+k][j-k] > 0)) { 
            temp.push_back((i+k)*10 + (j-k));
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
            temp.push_back((i-k)*10 + (j+k));
        } else if ((w_turn && board[i-k][j+k] < 0) || (!w_turn && board[i-k][j+k] > 0)) { 
            temp.push_back((i-k)*10 + (j+k));
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
            temp.push_back((i+k)*10 + (j+k));
        } else if ((w_turn && board[i+k][j+k] < 0) || (!w_turn && board[i+k][j+k] > 0)) { 
            temp.push_back((i+k)*10 + (j+k));
            break;
        } else {
            break;
        }
    }
}

void Game::get_knight_moves(int i, int j, std::vector<int>& temp) {
    bool w_turn; //must have local turn since we can be checking enemy pawn moves
    if (board[i][j] > 0) {
        w_turn = true;
    } else {
        w_turn = false;
    }
    if (i > 1) {//top 2
        if (j > 0 && ((w_turn && board[i-2][j-1] <= 0) || (!w_turn && board[i-2][j-1] >= 0))) {
            temp.push_back((i-2)*10 + (j-1));
        }
        if (j < 7 && ((w_turn && board[i-2][j+1] <= 0) || (!w_turn && board[i-2][j+1] >= 0))) {
            temp.push_back((i-2)*10 + (j+1));
        }
    }
    if (j < 6) {//right 2
        if (i > 0 && ((w_turn && board[i-1][j+2] <= 0) || (!w_turn && board[i-1][j+2] >= 0))) {
            temp.push_back((i-1)*10 + (j+2));
        }
        if (i < 7 && ((w_turn && board[i+1][j+2] <= 0) || (!w_turn &&  board[i+1][j+2] >= 0))) {
            temp.push_back((i+1)*10 + (j+2));
        }
    }
    if (i < 6) {//bottom 2
        if (j > 0 && ((w_turn && board[i+2][j-1] <= 0) || (!w_turn &&  board[i+2][j-1] >= 0))) {
            temp.push_back((i+2)*10 + (j-1));
        }
        if (j < 7 && ((w_turn && board[i+2][j+1] <= 0) || (!w_turn &&  board[i+2][j+1] >= 0))) {
            temp.push_back((i+2)*10 + (j+1));
        }
    }
    if (j > 1) {//left 2
        if (i > 0 && ((w_turn && board[i-1][j-2] <= 0) || (!w_turn &&  board[i-1][j-2] >= 0))) {
            temp.push_back((i-1)*10 + (j-2));
        }
        if (i < 7 && ((w_turn && board[i+1][j-2] <= 0) || (!w_turn &&  board[i+1][j-2] >= 0))) {
            temp.push_back((i+1)*10 + (j-2));
        }
    }
}

void Game::get_rook_moves(int i, int j, std::vector<int>& temp) {
    bool w_turn; //must have local turn since we can be checking enemy pawn moves
    if (board[i][j] > 0) {
        w_turn = true;
    } else {
        w_turn = false;
    }
    for (int k=1; k < i+1; k++) { //top col
        if (board[i-k][j] == 0) {
            temp.push_back((i-k)*10 + j);
        } else if ((w_turn && board[i-k][j] < 0) || (!w_turn && board[i-k][j] > 0)) {
            temp.push_back((i-k)*10 + j);
            break;
        } else {
            break;
        }
    }
    for (int k=1; k < (7-j)+1; k++) { //right col
        if (board[i][j+k] == 0) {
            temp.push_back(i*10 + (j+k));
        } else if ((w_turn && board[i][j+k] < 0) || (!w_turn && board[i][j+k] > 0)) {
            temp.push_back(i*10 + (j+k));
            break;
        } else {
            break;
        }
    }
    for (int k=1; k < (7-i)+1; k++) { //bottom col
        if (board[i+k][j] == 0) {
            temp.push_back((i+k)*10 + j);
        } else if ((w_turn && board[i+k][j] < 0) || (!w_turn && board[i+k][j] > 0)) {
            temp.push_back((i+k)*10 + j);
            break;
        } else {
            break;
        }
    }
    for (int k=1; k < j+1; k++) { //left col
        if (board[i][j-k] == 0) {
            temp.push_back(i*10 + (j-k));
        } else if ((w_turn && board[i][j-k] < 0) || (!w_turn && board[i][j-k] > 0)) {
            temp.push_back(i*10 + (j-k));
            break;
        } else {
            break;
        }
    }
}

void Game::get_king_moves(int i, int j, std::vector<int>& temp, bool castle[4]) {
    bool w_turn; //must have local turn since we can be checking enemy pawn moves
    if (board[i][j] > 0) {
        w_turn = true;
    } else {
        w_turn = false;
    }
    if (i != 0) {//top row
        for (int k=-1; k < 2; k++) { //move left to right
            if (j+k >= 0 && j+k <= 7 && ((w_turn && board[i-1][j+k] <= 0) || (!w_turn && board[i-1][j+k] >= 0))) {
                temp.push_back((i-1)*10 + (j+k));
            }
        }
    }
    if (i != 7) {//bottom row
        for (int k=-1; k < 2; k++) { 
            if (j+k >= 0 && j+k <= 7 && ((w_turn && board[i+1][j+k] <= 0) || (!w_turn && board[i+1][j+k] >= 0))) {
                temp.push_back((i+1)*10 + (j+k));
            }
        }
    }
    if (j < 7 && ((w_turn && board[i][j+1] <= 0) || (!w_turn && board[i][j+1] >= 0))) {//right square
        temp.push_back(i*10 + (j+1));
    }
    if (j > 0 && ((w_turn && board[i][j-1] <= 0) || (!w_turn && board[i][j-1] >= 0))) {//left square
        temp.push_back(i*10 + (j-1));
    }

    //castling
    int row;
    if (w_turn) {
        row = 70;
    } else {
        row = 0;
    }
    if (castle[0] && castle[1]) { //right
        temp.push_back(row + 6);
    }
    if (castle[2] && castle[3]) { //left
        temp.push_back(row + 2);
    }
}

std::vector<int> Game::get_trajectory(int pos) {
    //given a board and square return all possible indices to move, legal or not
    //pos is a index of form i*10 + j
    //indices will be returned in same form

    std::vector<int> temp;
    int i = pos / 10;
    int j = pos % 10;
    int piece = board[i][j];

    switch (piece) {

        case -1://black pawn
            get_pawn_moves(i, j, temp);
            break;
        
        case 1://white pawn
            get_pawn_moves(i, j, temp);
            break;

        case -2://black bishop
            get_bishop_moves(i, j, temp);
            break;

        case 2://white bishop
            get_bishop_moves(i, j, temp);
            break;

        case -3: //black knight
            get_knight_moves(i, j, temp);
            break;
        
        case 3: //white knight
            get_knight_moves(i, j, temp);
            break;

        case -4://black rook
            get_rook_moves(i, j, temp);
            break;

        case 4://white rook
            get_rook_moves(i, j, temp);
            break;

        case -5://black queen
            get_bishop_moves(i, j, temp);
            get_rook_moves(i, j, temp);
            break;

        case 5://white queen
            get_bishop_moves(i, j, temp);
            get_rook_moves(i, j, temp);
            break;

        case -6://black king
            get_king_moves(i, j, temp, b_castle);
            break;

        case 6://white king
            get_king_moves(i, j, temp, w_castle);
            break;
    }
    return temp;
}

std::vector<int> Game::get_all_trajectories() {
    //returns all possible trajectores of white or black, if w_turn is true then returns all of blacks trajectories, since 
    //this function is mainly used in calculating enemy moves and preventing psuedo legal ones
    //the return is an array of all possible moves for opponent
    std::vector<int> all_trajectories;
    for (int i=0; i < 80; i+=10) { //iterate over entire board
        for (int j=0; j < 8; j++) {
            if ((w_turn && board[i/10][j] < 0) || (!w_turn && board[i/10][j] > 0)) { //determines black or white side
                std::vector<int> trajectory = get_trajectory(i+j); //get all piece
                for (int k=0; k < (int)trajectory.size(); k++) {
                    all_trajectories.push_back(trajectory[k]); //add to final array
                }
            }
        }
    }
    return all_trajectories;
}

std::vector<int> Game::get_legal_moves(int pos) {
    //given a position in form i*10 + j of a piece, returns all legal moves of piece at pos by taking into consideration enemy trajectories
    //this function uses get_trajectory() to get current piece moves, then makes the move on a board copy and evaluates if this leaves the 
    //player in check, if so it removes it, if w_turn is true then the piece at pos must be white
    std::vector<int> legal_moves;
    std::vector<int> current_moves = get_trajectory(pos);
    for (int i=0; i < (int)current_moves.size(); i++) {
        //make move then check if enemy has vision of king, only concerned with static board
        move_piece(pos, current_moves[i]);
        if (!in_check()) {
            legal_moves.push_back(current_moves[i]);
        }
        undo_move(pos, current_moves[i]);
    }
    return legal_moves;
}

std::vector<std::vector<int>> Game::get_all_legal_moves() {
    //returns all possible legal moves of white or black, if w_turn is true then returns all of whites legal moves
    //the return is a 2d vector of all possible moves with the first element in each array being positiong of original piece
    std::vector<std::vector<int>> all_legal_moves;
    for (int i=0; i < 80; i+=10) { //iterate over entire board
        for (int j=0; j < 8; j++) {
            if ((w_turn && board[i/10][j] > 0) || (!w_turn && board[i/10][j] < 0)) { //determines black or white side
                std::vector<int> legal_moves = get_legal_moves(i+j); //get all piece
                std::vector<int> temp; //temp to prepend position to
                if (!legal_moves.empty()) {
                    temp.push_back(i+j);
                    for (int k=0; k < (int)legal_moves.size(); k++) {
                        temp.push_back(legal_moves[k]); //add to final array
                    }
                    all_legal_moves.push_back(temp);
                }
            }
        }
    }
    return all_legal_moves;
}