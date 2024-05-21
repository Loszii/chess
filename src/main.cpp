#include "raylib.h"
#include <iostream>

void draw_board(Texture2D board) {
    DrawTextureEx(board, (Vector2){0, 0}, 0, 1, WHITE);
}

void draw_pieces(Texture2D black_row1[], Texture2D b_pawn, Texture2D white_row1[], Texture2D w_pawn) {
    int square_width = 96;
    int bevel = 8;
    double scale = 0.75;
    int x = bevel;
    int y = bevel;

    for (int i=0; i < 8; i++) {
        DrawTextureEx(black_row1[i], (Vector2){x, y}, 0, scale, WHITE);
        x += square_width;
    }
    x = bevel;
    y = square_width + bevel;
    for (int i=0; i < 8; i++) {
        DrawTextureEx(b_pawn, (Vector2){x, y}, 0, scale, WHITE);
        x += square_width;
    }
    x = bevel;
    y = 6*square_width + bevel;
    for (int i=0; i < 8; i++) {
        DrawTextureEx(white_row1[i], (Vector2){x, y}, 0, scale, WHITE);
        x += square_width;
    }
    x = bevel;
    y = 7*square_width + bevel;
    for (int i=0; i < 8; i++) {
        DrawTextureEx(w_pawn, (Vector2){x, y}, 0, scale, WHITE);
        x += square_width;
    }
}

int main() {
    const int screen_width = 784;
    const int screen_height = 784;

    InitWindow(screen_width, screen_height, "Chess");
    SetTargetFPS(60);

    //board
    Texture2D board = LoadTexture("../res/board.png");

    //black pieces
    Texture2D b_king = LoadTexture("../res/b-king.png");
    Texture2D b_queen = LoadTexture("../res/b-queen.png");
    Texture2D b_rook = LoadTexture("../res/b-rook.png");
    Texture2D b_bishop = LoadTexture("../res/b-bishop.png");
    Texture2D b_knight = LoadTexture("../res/b-knight.png");
    Texture2D b_pawn = LoadTexture("../res/b-pawn.png");

    //white pieces
    Texture2D w_king = LoadTexture("../res/w-king.png");
    Texture2D w_queen = LoadTexture("../res/w-queen.png");
    Texture2D w_rook = LoadTexture("../res/w-rook.png");
    Texture2D w_bishop = LoadTexture("../res/w-bishop.png");
    Texture2D w_knight = LoadTexture("../res/w-knight.png");
    Texture2D w_pawn = LoadTexture("../res/w-pawn.png");

    Texture2D black_row1[8] = {b_rook, b_knight, b_bishop, b_queen, b_king, b_bishop, b_knight, b_rook};
    Texture2D white_row1[8] = {w_rook, w_knight, w_bishop, w_queen, w_king, w_bishop, w_knight, w_rook};

    //Game loop
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE);

        draw_board(board);
        draw_pieces(black_row1, b_pawn, white_row1, w_pawn);
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}