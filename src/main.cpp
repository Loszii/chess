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
    DrawTextureEx(select_texture, (Vector2){x, y}, 0, 1, (Color){255, 0, 0, 100});
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

int is_piece(int x, int y, int board[8][8], std::unordered_map<int, std::tuple<int, int>> coord) {
    //returns i*10+j of piece if there is one at x,y. else -1
    int index = get_index(x, y, coord);
    int i = index / 10;
    int j = index % 10;
    if (board[i][j] != 0) {
        return index;
    } else {
        return -1;
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
        case 1://white pawn
            if (i > 0 && board[i-1][j] == 0) {
                moves.push_back(((i-1)*10)+j);
            }
    }
    return moves;
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
        //drawing objects
        BeginDrawing();
        ClearBackground(WHITE);
        
        //draws pieces and board
        drawGame(board, coord, skins, board_texture);

        //shades in selected squares
        if (select) {
            drawSelect(coord, select_coord, select_texture);
            std::vector<int> moves = get_moves(board, coord, select_coord);
            int size = moves.size();
            for (int i=0; i < size; i++) {
                drawSelect(coord, moves[i], select_texture);
            }

            //if mouse is clicked again, use getindex and check if it within moves array
            //if it is then we place a 0 at the select_coord pos in board and move the int to new spot
        }

        EndDrawing();

        //game functionality
        if (IsMouseButtonPressed(0)) {
            int mouse_x = GetMouseX();
            int mouse_y = GetMouseY();
            int pos = is_piece(mouse_x, mouse_y, board, coord); //returns -1 if no piece and the position if there is
            if (pos != -1) { //pos num 0-100 representing 2d array indices
                select = true;
                select_coord = pos;
            } else {
                select = false;
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