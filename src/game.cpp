#include <iostream>
#include <unordered_map>
#include <tuple>
#include <vector>
#include <array>
#include "game.h"
#include "raylib.h"

Game::Game() {
    board_texture = LoadTexture("../res/board.png");
    select_texture = LoadTexture("../res/selector.png");

    //skins
    skins[1] = LoadTexture("../res/w-pawn.png");
    skins[2] = LoadTexture("../res/w-bishop.png");
    skins[3] = LoadTexture("../res/w-knight.png");
    skins[4] = LoadTexture("../res/w-rook.png");
    skins[5] = LoadTexture("../res/w-queen.png");
    skins[6] = LoadTexture("../res/w-king.png");
    skins[-1] = LoadTexture("../res/b-pawn.png");
    skins[-2] = LoadTexture("../res/b-bishop.png");
    skins[-3] = LoadTexture("../res/b-knight.png");
    skins[-4] = LoadTexture("../res/b-rook.png");
    skins[-5] = LoadTexture("../res/b-queen.png");
    skins[-6] = LoadTexture("../res/b-king.png");

    //coord
    int x;
    int y;
    for (int i=0; i < 80; i += 10) {
        x = BEVEL;
        y = BEVEL + ((i/10) * SQUARE_WIDTH);
        for (int j=0; j < 8; j++) {
            coord[i+j] = std::make_tuple(x, y);
            x += SQUARE_WIDTH;
        }
    }
}

void Game::draw_game() {
    DrawTexture(board_texture, 0, 0, WHITE); //board
    int x;
    int y;
    for (int i=0; i < 80; i += 10) {
        for (int j=0; j < 8; j++) {
            if (board.data[i/10][j] != 0) {
                int ind = i + j;
                x = std::get<0>(coord[ind]);
                y = std::get<1>(coord[ind]);
                DrawTextureEx(skins[board.data[i/10][j]], (Vector2){(float)x, (float)y}, 0, SCALE, WHITE); //pieces
            }
        }
    }

    //shades in selected squares
    if (select) {
        draw_select(select_pos, Color{0, 0, 0, 150});
        for (int i=0; i < (int)moves.size(); i++) {
            draw_select(moves[i], Color{0, 0, 0, 150});
        }
    }

    if (board.w_check) {
        draw_select(board.w_king_pos, Color{255, 0, 0, 150});
    } else if (board.b_check) {
        draw_select(board.b_king_pos, Color{255, 0, 0, 150});
    }
}

void Game::draw_select(int pos, Color color) {
    //shades in a square at the given pos of form i*10 + j
    int x = std::get<0>(coord[pos]);
    int y = std::get<1>(coord[pos]);
    DrawTextureEx(select_texture, (Vector2){(float)x, (float)y}, 0, 1, color);
}

void Game::select_move(int pos) {
    //takes in a position and selects either a piece or a move
    if (select) {
        //check if mouse was clicked on a available move
        for (int i=0; i < (int)moves.size(); i++) {
            if (pos == moves[i]) {
                move_piece(select_pos, pos, board);
                swap_turn();
                select = false;
                break;
            }
        }
        if (select) { //if still selecting and didnt make viable move
            check_for_selection(pos); //checks to find selection and sets moves to all moves
        } else { //no longer selecting so we moved a piece
            update_board();
        }
    } else {
        check_for_selection(pos);
    }
}

void Game::check_for_selection(int pos) {
    //checks if there is a piece to select at given pos, if so sets select to true, select_pos to pos, sets moves to all moves of that piece
    int i = pos/10;
    int j = pos % 10;
    if ((board.w_turn && board.data[i][j] > 0) || (!board.w_turn && board.data[i][j] < 0)) { //pos num 0-100 representing 2d array indices
        //below code runs if there is a move to select
        select = true;
        select_pos = pos;
        moves = get_legal_moves(pos, board); //gets possible moves
    } else {
        select = false;
    }
}

int Game::get_index(int x, int y) {
    //takes in mouse x and y pos and returns i*10 + j position
    int i = x - BEVEL;
    i = i / SQUARE_WIDTH;
    int j = y - BEVEL;
    j = j / SQUARE_WIDTH;
    if (i >= 0 && i <= 7 && j >= 0 && j <= 7) {
        return 10*j + i;
    } else {
        return -1; //cannot find index
    }
}

void Game::swap_turn() {
    if (board.w_turn) {
        board.w_turn = false;
    } else {
        board.w_turn = true;
    }
}

void Game::update_board() {
    std::vector<int> enemy_moves = get_enemy_moves(board.w_turn, board);
    if (board.w_turn) { //updating before white moves
        board.b_check = false;
        board.w_king_pos = get_piece_pos(6);
        board.w_check = under_attack(board.w_king_pos, enemy_moves);
    } else {
        board.w_check = false;
        board.b_king_pos = get_piece_pos(-6);
        board.b_check = under_attack(board.b_king_pos, enemy_moves);
    }
}

bool Game::under_attack(int pos, std::vector<int> enemy_moves) {
    //returns true if position is in enemy moves array
    for (int i=0; i < (int)enemy_moves.size(); i++) {
        if (enemy_moves[i] == pos) {
            return true;
        }
    }
    return false;
}

int Game::get_piece_pos(int piece) {
    //returns first occurance of piece within the current board, -1 if none
    for (int i=0; i < 8; i++) {
        for (int j=0; j < 8; j++) {
            if (board.data[i][j] == piece) {
                return i*10 + j;
            }
        }
    }
    return -1;
};

int Game::move_piece(int start_pos, int end_pos, Board& board) {
    //given a start pos and end pos, moves piece in current board from start to end. returns piece taken
    int piece = board.data[end_pos/10][end_pos%10];
    board.data[end_pos/10][end_pos%10] = board.data[start_pos/10][start_pos%10];
    board.data[start_pos/10][start_pos%10] = 0;
    return piece;
}

void Game::undo_move_piece(int start_pos, int end_pos, int captured_piece, Board& board) {
    board.data[start_pos/10][start_pos%10] = board.data[end_pos/10][end_pos%10];
    board.data[end_pos/10][end_pos%10] = captured_piece;
}

std::vector<int> Game::get_pawn_moves(int i, int j, Board& board) {
    std::vector<int> result;
    bool w_turn; //must have local turn since we can be checking enemy pawn moves
    int u_bound;
    int dir;
    int start_row;
    if (board.data[i][j] > 0) {
        w_turn = true;
        u_bound = 0;
        dir = -1;
        start_row = 6;
    } else {
        w_turn = false;
        u_bound = 7;
        dir = 1;
        start_row = 1;
    }

    if (i == start_row && board.data[i+dir][j] == 0 && board.data[i+(2*dir)][j] == 0) {//straight
        result.push_back(((i+dir)*10)+j);
        result.push_back(((i+(2*dir))*10)+j);
    } else if (i != u_bound && board.data[i+dir][j] == 0) {
        result.push_back(((i+dir)*10)+j);
    }
    if (i != u_bound) { //diags
        if (j > 0) {
            if (w_turn && board.data[i+dir][j-1] < 0) {
                result.push_back((i+dir)*10 + (j-1));
            } else if (!w_turn && board.data[i+dir][j-1] > 0) {
                result.push_back((i+dir)*10 + (j-1));
            }
        }
        if (j < 7) {
            if (w_turn && board.data[i+dir][j+1] < 0) {
                result.push_back((i+dir)*10 + (j+1));
            } else if (!w_turn && board.data[i+dir][j+1] > 0) {
                result.push_back((i+dir)*10 + (j+1));
            }
        }
    }
    return result;
}

std::vector<int> Game::get_bishop_moves(int i, int j, Board& board) {
    std::vector<int> result;
    bool w_turn;
    if (board.data[i][j] > 0) {
        w_turn = true;
    } else {
        w_turn = false;
    }
    for (int k=1; k < j+1; k++) { //upper left
        if (i-k < 0) {
            break;
        }
        if (board.data[i-k][j-k] == 0) {
            result.push_back((i-k)*10 + (j-k));
        } else if ((w_turn && board.data[i-k][j-k] < 0) || (!w_turn && board.data[i-k][j-k] > 0)) { //piece there so break after
            result.push_back((i-k)*10 + (j-k));
            break;
        } else {
            break; //hit own piece so break
        }
    }
    for (int k=1; k < j+1; k++) { //lower left
        if (i+k > 7) {
            break;
        }
        if (board.data[i+k][j-k] == 0) {
            result.push_back((i+k)*10 + (j-k));
        } else if ((w_turn && board.data[i+k][j-k] < 0) || (!w_turn && board.data[i+k][j-k] > 0)) { 
            result.push_back((i+k)*10 + (j-k));
            break;
        } else {
            break;
        }
    }
    for (int k=1; k < (7-j)+1; k++) { //upper right
        if (i-k < 0) { //off screen
            break;
        }
        if (board.data[i-k][j+k] == 0) {
            result.push_back((i-k)*10 + (j+k));
        } else if ((w_turn && board.data[i-k][j+k] < 0) || (!w_turn && board.data[i-k][j+k] > 0)) { 
            result.push_back((i-k)*10 + (j+k));
            break;
        } else {
            break;
        }
    }
    for (int k=1; k < (7-j)+1; k++) { //lower right
        if (i+k > 7) { //off screen
            break;
        }
        if (board.data[i+k][j+k] == 0) {
            result.push_back((i+k)*10 + (j+k));
        } else if ((w_turn && board.data[i+k][j+k] < 0) || (!w_turn && board.data[i+k][j+k] > 0)) { 
            result.push_back((i+k)*10 + (j+k));
            break;
        } else {
            break;
        }
    }
    return result;
}

std::vector<int> Game::get_knight_moves(int i, int j, Board& board) {
    std::vector<int> result;
    bool w_turn;
    if (board.data[i][j] > 0) {
        w_turn = true;
    } else {
        w_turn = false;
    }
    if (i > 1) {//top 2
        if (j > 0 && ((w_turn && board.data[i-2][j-1] <= 0) || (!w_turn && board.data[i-2][j-1] >= 0))) {
            result.push_back((i-2)*10 + (j-1));
        }
        if (j < 7 && ((w_turn && board.data[i-2][j+1] <= 0) || (!w_turn && board.data[i-2][j+1] >= 0))) {
            result.push_back((i-2)*10 + (j+1));
        }
    }
    if (j < 6) {//right 2
        if (i > 0 && ((w_turn && board.data[i-1][j+2] <= 0) || (!w_turn && board.data[i-1][j+2] >= 0))) {
            result.push_back((i-1)*10 + (j+2));
        }
        if (i < 7 && ((w_turn && board.data[i+1][j+2] <= 0) || (!w_turn &&  board.data[i+1][j+2] >= 0))) {
            result.push_back((i+1)*10 + (j+2));
        }
    }
    if (i < 6) {//bottom 2
        if (j > 0 && ((w_turn && board.data[i+2][j-1] <= 0) || (!w_turn &&  board.data[i+2][j-1] >= 0))) {
            result.push_back((i+2)*10 + (j-1));
        }
        if (j < 7 && ((w_turn && board.data[i+2][j+1] <= 0) || (!w_turn &&  board.data[i+2][j+1] >= 0))) {
            result.push_back((i+2)*10 + (j+1));
        }
    }
    if (j > 1) {//left 2
        if (i > 0 && ((w_turn && board.data[i-1][j-2] <= 0) || (!w_turn &&  board.data[i-1][j-2] >= 0))) {
            result.push_back((i-1)*10 + (j-2));
        }
        if (i < 7 && ((w_turn && board.data[i+1][j-2] <= 0) || (!w_turn &&  board.data[i+1][j-2] >= 0))) {
            result.push_back((i+1)*10 + (j-2));
        }
    }
    return result;
}

std::vector<int> Game::get_rook_moves(int i, int j, Board& board) {
    std::vector<int> result;
    bool w_turn;
    if (board.data[i][j] > 0) {
        w_turn = true;
    } else {
        w_turn = false;
    }
    for (int k=1; k < i+1; k++) { //top col
        if (board.data[i-k][j] == 0) {
            result.push_back((i-k)*10 + j);
        } else if ((w_turn && board.data[i-k][j] < 0) || (!w_turn && board.data[i-k][j] > 0)) {
            result.push_back((i-k)*10 + j);
            break;
        } else {
            break;
        }
    }
    for (int k=1; k < (7-j)+1; k++) { //right col
        if (board.data[i][j+k] == 0) {
            result.push_back(i*10 + (j+k));
        } else if ((w_turn && board.data[i][j+k] < 0) || (!w_turn && board.data[i][j+k] > 0)) {
            result.push_back(i*10 + (j+k));
            break;
        } else {
            break;
        }
    }
    for (int k=1; k < (7-i)+1; k++) { //bottom col
        if (board.data[i+k][j] == 0) {
            result.push_back((i+k)*10 + j);
        } else if ((w_turn && board.data[i+k][j] < 0) || (!w_turn && board.data[i+k][j] > 0)) {
            result.push_back((i+k)*10 + j);
            break;
        } else {
            break;
        }
    }
    for (int k=1; k < j+1; k++) { //left col
        if (board.data[i][j-k] == 0) {
            result.push_back(i*10 + (j-k));
        } else if ((w_turn && board.data[i][j-k] < 0) || (!w_turn && board.data[i][j-k] > 0)) {
            result.push_back(i*10 + (j-k));
            break;
        } else {
            break;
        }
    }
    return result;
}

std::vector<int> Game::get_queen_moves(int i, int j, Board& board) {
    std::vector<int> result;
    std::vector<int> temp = get_bishop_moves(i, j, board);
    for (int k=0; k < (int)temp.size(); k++) {
        result.push_back(temp[k]);
    }
    temp = get_rook_moves(i, j, board);
    for (int k=0; k < (int)temp.size(); k++) {
        result.push_back(temp[k]);
    }
    return result;
}

std::vector<int> Game::get_king_moves(int i, int j, Board& board) {
    std::vector<int> result;
    bool w_turn;
    if (board.data[i][j] > 0) {
        w_turn = true;
    } else {
        w_turn = false;
    }
    if (i != 0) {//top row
        for (int k=-1; k < 2; k++) { //move left to right
            if (j+k >= 0 && j+k <= 7 && ((w_turn && board.data[i-1][j+k] <= 0) || (!w_turn && board.data[i-1][j+k] >= 0))) {
                result.push_back((i-1)*10 + (j+k));
            }
        }
    }
    if (i != 7) {//bottom row
        for (int k=-1; k < 2; k++) { 
            if (j+k >= 0 && j+k <= 7 && ((w_turn && board.data[i+1][j+k] <= 0) || (!w_turn && board.data[i+1][j+k] >= 0))) {
                result.push_back((i+1)*10 + (j+k));
            }
        }
    }
    if (j < 7 && ((w_turn && board.data[i][j+1] <= 0) || (!w_turn && board.data[i][j+1] >= 0))) {//right square
        result.push_back(i*10 + (j+1));
    }
    if (j > 0 && ((w_turn && board.data[i][j-1] <= 0) || (!w_turn && board.data[i][j-1] >= 0))) {//left square
        result.push_back(i*10 + (j-1));
    }
    return result;
}

std::vector<int> Game::get_trajectory(int pos, Board& board) {
    //given a board and square return all possible indices to move, legal or not
    //pos is a index of form i*10 + j
    //indices will be returned in same form

    std::vector<int> result;
    int i = pos / 10;
    int j = pos % 10;
    int piece = board.data[i][j];

    if (piece == 1 || piece == -1) {
        result = get_pawn_moves(i, j, board);
    } else if (piece == 2 || piece == -2) {
        result = get_bishop_moves(i, j, board);
    } else if (piece == 3 || piece == -3) {
        result = get_knight_moves(i, j, board);
    } else if (piece == 4 || piece == -4) {
        result = get_rook_moves(i, j, board);
    } else if (piece == 5 || piece == -5) {
        result = get_queen_moves(i, j, board);
    } else if (piece == 6 || piece == -6) {
        result = get_king_moves(i, j, board);
    }

    return result;
}

std::vector<int> Game::get_enemy_moves(bool w_turn, Board& board) {
    //returns all trajectories of the opposite color
    std::vector<int> result;
    for (int i=0; i < 80; i+=10) { //iterate over entire board
        for (int j=0; j < 8; j++) {
            if ((w_turn && board.data[i/10][j] < 0) || (!w_turn && board.data[i/10][j] > 0)) { //determines black or white side
                std::vector<int> trajectory = get_trajectory(i+j, board); //get all piece
                for (int k=0; k < (int)trajectory.size(); k++) {
                    result.push_back(trajectory[k]); //add to final array
                }
            }
        }
    }
    return result;
}

std::vector<int> Game::get_legal_moves(int pos, Board& board) {
    //uses board in parameter
    std::vector<int> result;
    std::vector<int> all_moves = get_trajectory(pos, board);
    int temp;
    int king;
    if (board.w_turn) {
        king = 6;
    } else {
        king = -6;
    }
    for (int i=0; i < (int)all_moves.size(); i++) {
        temp = move_piece(pos, all_moves[i], board);
        std::vector<int> enemy_moves = get_enemy_moves(board.w_turn, board);
        if (!under_attack(get_piece_pos(king), enemy_moves)) {
            result.push_back(all_moves[i]);
        }
        undo_move_piece(pos, all_moves[i], temp, board);
    }
    return result;
}