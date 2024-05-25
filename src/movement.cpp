#include <unordered_map>
#include <tuple>
#include <vector>
#include "movement.h"
#include <iostream>

void move_piece(int start_pos, int end_pos, int board[8][8]) {
    //given a start pos and end pos, moves piece at start to end
    board[end_pos/10][end_pos%10] = board[start_pos/10][start_pos%10];
    board[start_pos/10][start_pos%10] = 0;
}

std::vector<int> get_moves(int board[8][8], int pos, bool legal_only) {
    //given a board, coord sys, and square return all possible indices to move
    //pos is a index of form i*10 + j
    //indices will be returned in same form
    std::vector<int> moves;
    int i = pos / 10;
    int j = pos % 10;
    int piece = board[i][j];
    bool white_to_move;
    if (piece > 0) {
        white_to_move = true;
    } else {
        white_to_move = false;
    }
    switch (piece) {

        case -1://black pawn
            if (i == 1 && board[i+1][j] == 0 && board[i+2][j] == 0) { //straight dir
                moves.push_back(((i+1)*10)+j);
                moves.push_back(((i+2)*10)+j);
            } else if (i < 7 && board[i+1][j] == 0) {
                moves.push_back(((i+1)*10)+j);
            }
            if (i < 7) { //diags
                if (j > 0 && board[i+1][j-1] > 0) { // < 0 means black piece
                    moves.push_back((i+1)*10 + (j-1));
                }
                if (j < 7 && board[i+1][j+1] > 0) {
                    moves.push_back((i+1)*10 + (j+1));
                }
            }
            break;
        
        case 1://white pawn
            if (i == 6 && board[i-1][j] == 0 && board[i-2][j] == 0) { //straight dir
                moves.push_back(((i-1)*10)+j);
                moves.push_back(((i-2)*10)+j);
            } else if (i > 0 && board[i-1][j] == 0) {
                moves.push_back(((i-1)*10)+j);
            }
            if (i > 0) { //diags
                if (j > 0 && board[i-1][j-1] < 0) { // < 0 means black piece
                    moves.push_back((i-1)*10 + (j-1));
                }
                if (j < 7 && board[i-1][j+1] < 0) {
                    moves.push_back((i-1)*10 + (j+1));
                }
            }
            break;

            case -2://black bishop
                for (int k=1; k < j+1; k++) { //upper left
                    if (i-k < 0) {
                        break;
                    }
                    if (board[i-k][j-k] == 0) {
                        moves.push_back((i-k)*10 + (j-k));
                    } else if (board[i-k][j-k] > 0) { //piece there so break after
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
                    } else if (board[i+k][j-k] > 0) { 
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
                    } else if (board[i-k][j+k] > 0) { 
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
                    } else if (board[i+k][j+k] > 0) { 
                        moves.push_back((i+k)*10 + (j+k));
                        break;
                    } else {
                        break;
                    }
                }
                break;

            case 2://white bishop
                for (int k=1; k < j+1; k++) { //upper left
                    if (i-k < 0) {
                        break;
                    }
                    if (board[i-k][j-k] == 0) {
                        moves.push_back((i-k)*10 + (j-k));
                    } else if (board[i-k][j-k] < 0) { //piece there so break after
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
                    } else if (board[i+k][j-k] < 0) { 
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
                    } else if (board[i-k][j+k] < 0) { 
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
                    } else if (board[i+k][j+k] < 0) { 
                        moves.push_back((i+k)*10 + (j+k));
                        break;
                    } else {
                        break;
                    }
                }
                break;

            case -3: //black knight
                if (i > 1) {//top 2
                    if (j > 0 && board[i-2][j-1] >= 0) {
                        moves.push_back((i-2)*10 + (j-1));
                    }
                    if (j < 7 && board[i-2][j+1] >= 0) {
                        moves.push_back((i-2)*10 + (j+1));
                    }
                }
                if (j < 6) {//right 2
                    if (i > 0 && board[i-1][j+2] >= 0) {
                        moves.push_back((i-1)*10 + (j+2));
                    }
                    if (i < 7 && board[i+1][j+2] >= 0) {
                        moves.push_back((i+1)*10 + (j+2));
                    }
                }
                if (i < 6) {//bottom 2
                    if (j > 0 && board[i+2][j-1] >= 0) {
                        moves.push_back((i+2)*10 + (j-1));
                    }
                    if (j < 7 && board[i+2][j+1] >= 0) {
                        moves.push_back((i+2)*10 + (j+1));
                    }
                }
                if (j > 1) {//left 2
                    if (i > 0 && board[i-1][j-2] >= 0) {
                        moves.push_back((i-1)*10 + (j-2));
                    }
                    if (i < 7 && board[i+1][j-2] >= 0) {
                        moves.push_back((i+1)*10 + (j-2));
                    }
                }
                break;
            
            case 3: //white knight
                if (i > 1) {//top 2
                    if (j > 0 && board[i-2][j-1] <= 0) {
                        moves.push_back((i-2)*10 + (j-1));
                    }
                    if (j < 7 && board[i-2][j+1] <= 0) {
                        moves.push_back((i-2)*10 + (j+1));
                    }
                }
                if (j < 6) {//right 2
                    if (i > 0 && board[i-1][j+2] <= 0) {
                        moves.push_back((i-1)*10 + (j+2));
                    }
                    if (i < 7 && board[i+1][j+2] <= 0) {
                        moves.push_back((i+1)*10 + (j+2));
                    }
                }
                if (i < 6) {//bottom 2
                    if (j > 0 && board[i+2][j-1] <= 0) {
                        moves.push_back((i+2)*10 + (j-1));
                    }
                    if (j < 7 && board[i+2][j+1] <= 0) {
                        moves.push_back((i+2)*10 + (j+1));
                    }
                }
                if (j > 1) {//left 2
                    if (i > 0 && board[i-1][j-2] <= 0) {
                        moves.push_back((i-1)*10 + (j-2));
                    }
                    if (i < 7 && board[i+1][j-2] <= 0) {
                        moves.push_back((i+1)*10 + (j-2));
                    }
                }
                break;

            case -4://black rook
                for (int k=1; k < i+1; k++) { //top col
                    if (board[i-k][j] == 0) {
                        moves.push_back((i-k)*10 + j);
                    } else if (board[i-k][j] > 0) {
                        moves.push_back((i-k)*10 + j);
                        break;
                    } else {
                        break;
                    }
                }
                for (int k=1; k < (7-j)+1; k++) { //right col
                    if (board[i][j+k] == 0) {
                        moves.push_back(i*10 + (j+k));
                    } else if (board[i][j+k] > 0) {
                        moves.push_back(i*10 + (j+k));
                        break;
                    } else {
                        break;
                    }
                }
                for (int k=1; k < (7-i)+1; k++) { //bottom col
                    if (board[i+k][j] == 0) {
                        moves.push_back((i+k)*10 + j);
                    } else if (board[i+k][j] > 0) {
                        moves.push_back((i+k)*10 + j);
                        break;
                    } else {
                        break;
                    }
                }
                for (int k=1; k < j+1; k++) { //left col
                    if (board[i][j-k] == 0) {
                        moves.push_back(i*10 + (j-k));
                    } else if (board[i][j-k] > 0) {
                        moves.push_back(i*10 + (j-k));
                        break;
                    } else {
                        break;
                    }
                }
                break;

            case 4://white rook
                for (int k=1; k < i+1; k++) { //top col
                    if (board[i-k][j] == 0) {
                        moves.push_back((i-k)*10 + j);
                    } else if (board[i-k][j] < 0) {
                        moves.push_back((i-k)*10 + j);
                        break;
                    } else {
                        break;
                    }
                }
                for (int k=1; k < (7-j)+1; k++) { //right col
                    if (board[i][j+k] == 0) {
                        moves.push_back(i*10 + (j+k));
                    } else if (board[i][j+k] < 0) {
                        moves.push_back(i*10 + (j+k));
                        break;
                    } else {
                        break;
                    }
                }
                for (int k=1; k < (7-i)+1; k++) { //bottom col
                    if (board[i+k][j] == 0) {
                        moves.push_back((i+k)*10 + j);
                    } else if (board[i+k][j] < 0) {
                        moves.push_back((i+k)*10 + j);
                        break;
                    } else {
                        break;
                    }
                }
                for (int k=1; k < j+1; k++) { //left col
                    if (board[i][j-k] == 0) {
                        moves.push_back(i*10 + (j-k));
                    } else if (board[i][j-k] < 0) {
                        moves.push_back(i*10 + (j-k));
                        break;
                    } else {
                        break;
                    }
                }
                break;

                case -5://black queen

                    //bishop code
                    for (int k=1; k < j+1; k++) { //upper left
                        if (i-k < 0) {
                            break;
                        }
                        if (board[i-k][j-k] == 0) {
                            moves.push_back((i-k)*10 + (j-k));
                        } else if (board[i-k][j-k] > 0) { //piece there so break after
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
                        } else if (board[i+k][j-k] > 0) { 
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
                        } else if (board[i-k][j+k] > 0) { 
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
                        } else if (board[i+k][j+k] > 0) { 
                            moves.push_back((i+k)*10 + (j+k));
                            break;
                        } else {
                            break;
                        }
                    }

                    //rook code
                    for (int k=1; k < i+1; k++) { //top col
                        if (board[i-k][j] == 0) {
                            moves.push_back((i-k)*10 + j);
                        } else if (board[i-k][j] > 0) {
                            moves.push_back((i-k)*10 + j);
                            break;
                        } else {
                            break;
                        }
                    }
                    for (int k=1; k < (7-j)+1; k++) { //right col
                        if (board[i][j+k] == 0) {
                            moves.push_back(i*10 + (j+k));
                        } else if (board[i][j+k] > 0) {
                            moves.push_back(i*10 + (j+k));
                            break;
                        } else {
                            break;
                        }
                    }
                    for (int k=1; k < (7-i)+1; k++) { //bottom col
                        if (board[i+k][j] == 0) {
                            moves.push_back((i+k)*10 + j);
                        } else if (board[i+k][j] > 0) {
                            moves.push_back((i+k)*10 + j);
                            break;
                        } else {
                            break;
                        }
                    }
                    for (int k=1; k < j+1; k++) { //left col
                        if (board[i][j-k] == 0) {
                            moves.push_back(i*10 + (j-k));
                        } else if (board[i][j-k] > 0) {
                            moves.push_back(i*10 + (j-k));
                            break;
                        } else {
                            break;
                        }
                    }
                    break;

                case 5://white queen

                    //bishop code
                    for (int k=1; k < j+1; k++) { //upper left
                        if (i-k < 0) {
                            break;
                        }
                        if (board[i-k][j-k] == 0) {
                            moves.push_back((i-k)*10 + (j-k));
                        } else if (board[i-k][j-k] < 0) { //piece there so break after
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
                        } else if (board[i+k][j-k] < 0) { 
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
                        } else if (board[i-k][j+k] < 0) { 
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
                        } else if (board[i+k][j+k] < 0) { 
                            moves.push_back((i+k)*10 + (j+k));
                            break;
                        } else {
                            break;
                        }
                    }

                    //rook code
                    for (int k=1; k < i+1; k++) { //top col
                        if (board[i-k][j] == 0) {
                            moves.push_back((i-k)*10 + j);
                        } else if (board[i-k][j] < 0) {
                            moves.push_back((i-k)*10 + j);
                            break;
                        } else {
                            break;
                        }
                    }
                    for (int k=1; k < (7-j)+1; k++) { //right col
                        if (board[i][j+k] == 0) {
                            moves.push_back(i*10 + (j+k));
                        } else if (board[i][j+k] < 0) {
                            moves.push_back(i*10 + (j+k));
                            break;
                        } else {
                            break;
                        }
                    }
                    for (int k=1; k < (7-i)+1; k++) { //bottom col
                        if (board[i+k][j] == 0) {
                            moves.push_back((i+k)*10 + j);
                        } else if (board[i+k][j] < 0) {
                            moves.push_back((i+k)*10 + j);
                            break;
                        } else {
                            break;
                        }
                    }
                    for (int k=1; k < j+1; k++) { //left col
                        if (board[i][j-k] == 0) {
                            moves.push_back(i*10 + (j-k));
                        } else if (board[i][j-k] < 0) {
                            moves.push_back(i*10 + (j-k));
                            break;
                        } else {
                            break;
                        }
                    }
                    break;

                    case -6://black king
                        if (i > 0) {//top row
                            for (int k=-1; k < 2; k++) { //move left to right
                                if (j+k >= 0 && j+k <= 7 && board[i-1][j+k] >= 0) {
                                    moves.push_back((i-1)*10 + (j+k));
                                }
                            }
                        }
                        if (i < 7) {//bottom row
                            for (int k=-1; k < 2; k++) { 
                                if (j+k >= 0 && j+k <= 7 && board[i+1][j+k] >= 0) {
                                    moves.push_back((i+1)*10 + (j+k));
                                }
                            }
                        }
                        if (j < 7 && board[i][j+1] >= 0) {//right square
                            moves.push_back(i*10 + (j+1));
                        }
                        if (j > 0 && board[i][j-1] >= 0) {//left square
                            moves.push_back(i*10 + (j-1));
                        }
                        break;

                    case 6://white king
                        if (i > 0) {//top row
                            for (int k=-1; k < 2; k++) { //move left to right
                                if (j+k >= 0 && j+k <= 7 && board[i-1][j+k] <= 0) {
                                    moves.push_back((i-1)*10 + (j+k));
                                }
                            }
                        }
                        if (i < 7) {//bottom row
                            for (int k=-1; k < 2; k++) { 
                                if (j+k >= 0 && j+k <= 7 && board[i+1][j+k] <= 0) {
                                    moves.push_back((i+1)*10 + (j+k));
                                }
                            }
                        }
                        if (j < 7 && board[i][j+1] <= 0) {//right square
                            moves.push_back(i*10 + (j+1));
                        }
                        if (j > 0 && board[i][j-1] <= 0) {//left square
                            moves.push_back(i*10 + (j-1));
                        }
                        break;
    }
    

    //need to do this whether they are in check or not since a player can move a piece and reveal a check
    if (legal_only) {
            if (white_to_move) {
                std::vector<int> final_moves;
                //for each move in moves make a new copy of board, make the move, and check if still in check
                for (int k=0; k < (int)moves.size(); k++) {
                    bool to_remove = false;
                    int board_copy[8][8]; //copying board
                    for (int l=0; l < 8; l++) {
                        for (int m=0; m < 8; m++) {
                            board_copy[l][m] = board[l][m];
                        }
                    }
                    move_piece(pos, moves[k], board_copy); //makes piece move on our copy

                    //check if board_copy has a check now
            
                    //first recalculate king pos
                    std::vector<int> king_positions = get_king_coord(board_copy);
                    int white_king_pos = king_positions[0];

                    std::vector<std::vector<int>> all_moves = get_all_moves(board_copy, false); //set both checks to false to prevent recursive call (only care if still in check)

                    //check all moves and see if kings in check, disgregarding first index signifying piece pos
                    for (int l=0; l < (int)all_moves.size(); l++) {
                        for (int m=1; m < (int)all_moves[l].size(); m++) {
                            if (all_moves[l][m] == white_king_pos) { //still in check
                                to_remove = true;
                                break;
                            }
                        }
                    }
                    if (!to_remove) {
                        final_moves.push_back(moves[k]); //passed all tests
                    }
                }

                return final_moves;

        } else { //same but for black
            std::vector<int> final_moves;
            for (int k=0; k < (int)moves.size(); k++) {
                bool to_remove = false;
                int board_copy[8][8];
                for (int l=0; l < 8; l++) {
                    for (int m=0; m < 8; m++) {
                        board_copy[l][m] = board[l][m];
                    }
                }
                move_piece(pos, moves[k], board_copy);
                std::vector<int> king_positions = get_king_coord(board_copy);
                int black_king_pos = king_positions[1];

                std::vector<std::vector<int>> all_moves = get_all_moves(board_copy, false);

                for (int l=0; l < (int)all_moves.size(); l++) {
                    for (int m=1; m < (int)all_moves[l].size(); m++) {
                        if (all_moves[l][m] == black_king_pos) { //still in check
                            to_remove = true;
                            break;
                        }
                    }
                }
                if (!to_remove) {
                    final_moves.push_back(moves[k]); //passed all tests
                }
            }
            return final_moves;

        }
    } else {
        return moves;
    }
}

//prob remove white_check param
std::vector<std::vector<int>> get_all_moves(int board[8][8], bool legal_only) {
    //returns all possible moves given a board and turn, of form array of arrays were each array's first element is the piece moving (index) and rest are possible moves
    //returns all moves even illegal ones
    std::vector<std::vector<int>> all_moves;
    for (int i=0; i < 80; i+=10) { //iterate over entire board
        for (int j=0; j < 8; j++) {
            if (board[i/10][j] != 0) { //if piece
                std::vector<int> moves;
                moves.push_back(i+j); //adding position of peice to first
                std::vector<int> temp = get_moves(board, i+j, legal_only); //getting moves for the rest of arrray
                for (int k=0; k < (int)temp.size(); k++) {
                    moves.push_back(temp[k]); //transfer to new array
                }
                all_moves.push_back(moves);
            }
        }
    }

    return all_moves;
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