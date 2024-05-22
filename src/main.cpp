#include "raylib.h"
#include <iostream>
#include <unordered_map>
#include <tuple>

const int screen_width = 784;
const int screen_height = 784;

const int square_width = 96;
const int bevel = 8;
const double scale = 0.75; // scale for pieces to fit into sqayre

std::unordered_map<int, Texture2D> get_skins() {
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

void drawGame(int board[8][8], std::unordered_map<int, std::tuple<int, int>> coord, std::unordered_map<int, Texture2D> skins, Texture2D board_texture, Texture2D selector_texture) {
    DrawTexture(board_texture, 0, 0, WHITE);
    int x;
    int y;
    for (int i=0; i < 8; i++) {
        for (int j=0; j < 8; j++) {
            if (board[i][j] != 0) {
                int ind = (i*10) + j;
                x = std::get<0>(coord[ind]);
                y = std::get<1>(coord[ind]);
                DrawTextureEx(skins[board[i][j]], (Vector2){x, y}, 0, scale, WHITE);
            }
        }
    }
}

int main() {

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
    std::unordered_map<int, std::tuple<int, int>> coord = get_coord();
    //mapping of indices to x-y coord
    //00, 01, 02, 03, 04, 05, 06, 07,
    //10, 11, 12, 13, 14, 15, 16, 17...


    Texture2D board_texture = LoadTexture("../res/board.png");
    Texture2D select_texture = LoadTexture("../res/selector.png");
    bool selector = false; //make false if click on 0 square


    //Game loop
    while (!WindowShouldClose()) {
        //drawing objects
        BeginDrawing();
        ClearBackground(WHITE);
        
        drawGame(board, coord, skins, board_texture, select_texture);

        EndDrawing();

        /*
        //game functionality
        if (IsMouseButtonPressed(0)) {
            int mouse_x = GetMouseX();
            int mouse_y = GetMouseY();
            if(is_piece(mouse_x, mouse_y, all_pieces)) {
                obj* piece = get_piece(mouse_x, mouse_y, all_pieces);
                selector = true;
                selector_x = piece->x;
                selector_y = piece->y;
                std::cout << piece->name << '\n';
            } else {
                selector = false;
            }
            
        }*/
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