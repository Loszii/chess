#include "raylib.h"
#include <iostream>
#include <unordered_map>
#include <tuple>
#include <vector>
#include "movement.h"

const int SCREEN_WIDTH = 784;
const int SCREEN_HEIGHT = 784;

const int SQUARE_WIDTH = 96;
const int BEVEL = 8;
const double SCALE = 0.75;

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
        x = BEVEL;
        y = BEVEL + ((i/10) * SQUARE_WIDTH);
        for (int j=0; j < 8; j++) {
            coord[i+j] = std::make_tuple(x, y);
            x += SQUARE_WIDTH;
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
                DrawTextureEx(skins[board[i/10][j]], (Vector2){(float)x, (float)y}, 0, SCALE, WHITE);
            }
        }
    }
}

void drawSelect(std::unordered_map<int, std::tuple<int, int>> coord, int pos, Texture2D select_texture) {
    //shades in a square at the given pos of form i*10 + j
    int x = std::get<0>(coord[pos]);
    int y = std::get<1>(coord[pos]);
    DrawTextureEx(select_texture, (Vector2){(float)x, (float)y}, 0, 1, (Color){255, 0, 0, 50});
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
            bottom = top + SQUARE_WIDTH;
            left = std::get<0>(coord[i+j]);
            right = left + SQUARE_WIDTH;
            if (left <= x && x <= right && top <= y && y <= bottom) {
                return i+j;
            }
        }
    }
    return -1; //cannot find index
}

void move_piece(int start_pos, int end_pos, int board[8][8]) {
    //given a start pos and end pos, moves piece at start to end
    board[end_pos/10][end_pos%10] = board[start_pos/10][start_pos%10];
    board[start_pos/10][start_pos%10] = 0;
}

int main() {
    //main game loop

    //window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Chess");
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

    /*bool white_right_castle = true;
    bool white_left_castle = true;
    bool black_right_castle = true;
    bool black_left_castle = true;*/
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
    bool select = false;
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
            for (int i=0; i < move_cap; i++) {
                drawSelect(coord, moves[i], select_texture);
            }
        }

        EndDrawing();

        //game functionality
        if (IsMouseButtonPressed(0)) {
            int pos = get_index(GetMouseX(), GetMouseY(), coord);
            if (pos != -1) { //-1 means not a square
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