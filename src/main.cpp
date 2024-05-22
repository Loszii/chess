#include "raylib.h"
#include <iostream>
#include <unordered_map>
#include <tuple>
#include <vector>

const int screen_width = 784;
const int screen_height = 784;

const int square_width = 96;
const int bevel = 8;
const double scale = 0.75; // scale for pieces to fit into sqayre

std::unordered_map<int, Texture2D> get_skins() {
    //returns a map that takes in integers representing chess pieces and returns their Texture
    /*key:
    + = white, - = black
    0: empty
    1: pawn
    2: bishop
    3: knight
    4: rook
    5: queen
    6: king*/
    
    std::unordered_map<int, Texture2D> skins;
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

    return skins;
}

std::unordered_map<int, std::tuple<int, int>> get_coord() {
    //makes the coord, a map of 2d indices of form i*10 + j to a tuple of form (x,y)
    //this tuple represents the 2d-array squares physical position on the window.
    std::unordered_map<int, std::tuple<int, int>> coord;
    int x;
    int y;
    for (int i=0; i < 80; i += 10) {
        x = bevel;
        y = bevel + ((i/10) * square_width);
        for (int j=0; j < 8; j++) {
            coord[i+j] = std::make_tuple(x, y);
            x += square_width;
        }
    }
    
    return coord;
}

void drawGame(int board[8][8], std::unordered_map<int, std::tuple<int, int>> coord, std::unordered_map<int, Texture2D> skins, Texture2D board_texture) {
    //draws the board and then all of the pieces on it
    DrawTexture(board_texture, 0, 0, WHITE);
    int x;
    int y;
    for (int i=0; i < 80; i += 10) {
        for (int j=0; j < 8; j++) {
            if (board[i/10][j] != 0) {
                int ind = i + j;
                x = std::get<0>(coord[ind]);
                y = std::get<1>(coord[ind]);
                DrawTextureEx(skins[board[i/10][j]], (Vector2){x, y}, 0, scale, WHITE);
            }
        }
    }
}

void drawSelect(std::unordered_map<int, std::tuple<int, int>> coord, int pos, Texture2D select_texture) {
    //shades in a square at the given pos of form i*10 + j
    int x = std::get<0>(coord[pos]);
    int y = std::get<1>(coord[pos]);
    DrawTextureEx(select_texture, (Vector2){x, y}, 0, 1, (Color){255, 0, 0, 50});
}

int get_index(int x, int y, std::unordered_map<int, std::tuple<int, int>> coord) {
    //takes in mouse x and y pos and returns i*10 + j
    int top;
    int bottom;
    int left;
    int right;
    for (int i=0; i < 80; i += 10) {
        for (int j=0; j < 8; j++) {
            top = std::get<1>(coord[i+j]);
            bottom = top + square_width;
            left = std::get<0>(coord[i+j]);
            right = left + square_width;
            if (left < x && x < right && top < y && y < bottom) {
                return i+j;
            }
        }
    }
}

std::vector<int> get_moves(int board[8][8], std::unordered_map<int, std::tuple<int, int>> coord, int pos) {
    //given a board, coord sys, and square return all possible indices to move
    //pos is a index of form i*10 + j
    //indices will be returned in same form
    std::vector<int> moves;
    int i = pos / 10;
    int j = pos % 10;
    int piece = board[i][j];
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

    }
    return moves;
}

void move_piece(int start_pos, int end_pos, int board[8][8]) {
    board[end_pos/10][end_pos%10] = board[start_pos/10][start_pos%10];
    board[start_pos/10][start_pos%10] = 0;
}

int main() {
    //main game loop

    //window
    InitWindow(screen_width, screen_height, "Chess");
    SetTargetFPS(60);

    int board[8][8] = {
        {-4, -3, -2, -5, -6, -2, -3, -4},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {4, 3, 2, 5, 6, 2, 3, 4},
    };

    bool white_turn = true;
    std::vector<int> moves;
    int move_cap;
    
    std::unordered_map<int, Texture2D> skins = get_skins(); //skin textures maps num to Texture2D
    std::unordered_map<int, std::tuple<int, int>> coord = get_coord(); //mapping of indices to x-y coord
    //2d indices of form vvvvv
    //00, 01, 02, 03, 04, 05, 06, 07,
    //10, 11, 12, 13, 14, 15, 16, 17...


    Texture2D board_texture = LoadTexture("../res/board.png");
    Texture2D select_texture = LoadTexture("../res/selector.png");
    bool select = false; //make false if click on 0 square
    int select_coord;

    //Game loop
    while (!WindowShouldClose()) {

        //TO DO: ONLY GET MOVES WHEN SELECT IS CHANGED TO TRUE NOT EVERY FRAME!

        //drawing objects
        BeginDrawing();
        ClearBackground(WHITE);
        
        //draws pieces and board
        drawGame(board, coord, skins, board_texture);

        //shades in selected squares
        if (select) {
            drawSelect(coord, select_coord, select_texture);
            for (int i=0; i < move_cap; i++) {
                drawSelect(coord, moves[i], select_texture);
            }

            //if mouse is clicked again, use getindex and check if it within moves array
            //if it is then we place a 0 at the select_coord pos in board and move the int to new spot
        }

        EndDrawing();

        //game functionality
        if (IsMouseButtonPressed(0)) {
            int pos = get_index(GetMouseX(), GetMouseY(), coord);
            int i = pos/10;
            int j = pos % 10;

            if (select) {
                //check if mouse was clicked on a available move
                for (int i=0; i < move_cap; i++) {
                    if (pos == moves[i]) {
                        move_piece(select_coord, moves[i], board);
                        if (white_turn) {
                            white_turn = false;
                        } else {
                            white_turn = true;
                        }
                        select = false;
                        break;
                    }
                }
                if (select) { //if still selecting and didnt make viable move
                    if ((white_turn && board[i][j] > 0) || (!white_turn && board[i][j] < 0)) { //pos num 0-100 representing 2d array indices
                        select = true;
                        select_coord = pos;
                        moves = get_moves(board, coord, select_coord); //gets possible moves
                        move_cap = moves.size();
                    } else {
                        select = false;
                    }
                }
            } else { //get pos of selection if piece
                if ((white_turn && board[i][j] > 0) || (!white_turn && board[i][j] < 0)) { //pos num 0-100 representing 2d array indices
                    select = true;
                    select_coord = pos;
                    moves = get_moves(board, coord, select_coord); //gets possible moves
                    move_cap = moves.size();
                } else {
                    select = false;
                }
            }
            
        }
    }
    //unloading textures
    UnloadTexture(board_texture);
    UnloadTexture(select_texture);
    for (auto& skin : skins) {
        UnloadTexture(skin.second);
    }

    CloseWindow();
    return 0;
}