#include "raylib.h"
#include <iostream>

const int screen_width = 784;
const int screen_height = 784;

const int square_width = 96;
const int bevel = 8;

class obj {
    public:
        bool alive = true;
        Texture2D texture;
        int x;
        int y;
        double scale;
        Color color = WHITE;
        obj(double scale, Texture2D texture) {
            this->scale = scale;
            this->texture = texture;
        }
        obj() = default;
        //implement deconstructor
        void draw(){
            if (alive) {
                DrawTextureEx(texture, (Vector2){x, y}, 0, scale, color);
            }
        }
};


void draw_pieces(obj* all_pieces) { //take in array of all pieces
    for (int i=0; i < 32; i++) {
        all_pieces[i].draw();
    }
}

bool is_piece(int x, int y, obj* all_pieces) {
    //takes in x and y val of cursor and returns true if piece there
    int left;
    int right;
    int top;
    int bottom;
    for (int i=0; i < 32; i++) {
        left = all_pieces[i].x;
        right = left + square_width;
        top = all_pieces[i].y;
        bottom = top + square_width;
        if (left < x && x < right && top < y && y < bottom) {
            return true;
        }
    }
    return false;
}

obj* get_piece(int x, int y, obj* all_pieces) {
    //takes in x and y val of cursor and gets the piece
    int left;
    int right;
    int top;
    int bottom;
    for (int i=0; i < 32; i++) {
        left = all_pieces[i].x;
        right = left + square_width;
        top = all_pieces[i].y;
        bottom = top + square_width;
        if (left < x && x < right && top < y && y < bottom) {
            return &all_pieces[i];
        }
    }
}

int main() {

    InitWindow(screen_width, screen_height, "Chess");
    SetTargetFPS(60);

    obj board = obj(1, LoadTexture("../res/board.png"));
    board.x = board.y = 0;

    obj all_pieces[32];
    int count = 0;

    //black pieces
    obj b_rook1 = obj(0.75, LoadTexture("../res/b-rook.png"));
    obj b_knight1 = obj(0.75, LoadTexture("../res/b-knight.png"));
    obj b_bishop1 = obj(0.75, LoadTexture("../res/b-bishop.png"));
    obj b_queen = obj(0.75, LoadTexture("../res/b-queen.png"));
    obj b_king = obj(0.75, LoadTexture("../res/b-king.png"));
    obj b_bishop2 = obj(0.75, LoadTexture("../res/b-bishop.png"));
    obj b_knight2 = obj(0.75, LoadTexture("../res/b-knight.png"));
    obj b_rook2 = obj(0.75, LoadTexture("../res/b-rook.png"));

    obj black_row1[8] = {b_rook1, b_knight1, b_bishop1, b_queen, b_king, b_bishop2, b_knight2, b_rook2};

    int x = bevel;
    int y = bevel;
    for (int i=0; i < 8; i++) {
        black_row1[i].x = x;
        black_row1[i].y = y;
        x += square_width;
        all_pieces[count] = black_row1[i];
        count += 1;
    }

    obj b_pawn1 = obj(0.75, LoadTexture("../res/b-pawn.png"));
    obj b_pawn2 = obj(0.75, LoadTexture("../res/b-pawn.png"));
    obj b_pawn3 = obj(0.75, LoadTexture("../res/b-pawn.png"));
    obj b_pawn4 = obj(0.75, LoadTexture("../res/b-pawn.png"));
    obj b_pawn5 = obj(0.75, LoadTexture("../res/b-pawn.png"));
    obj b_pawn6 = obj(0.75, LoadTexture("../res/b-pawn.png"));
    obj b_pawn7 = obj(0.75, LoadTexture("../res/b-pawn.png"));
    obj b_pawn8 = obj(0.75, LoadTexture("../res/b-pawn.png"));

    obj black_row2[8] = {b_pawn1, b_pawn2, b_pawn3, b_pawn4, b_pawn5, b_pawn6, b_pawn7, b_pawn8};

    x = bevel;
    y = square_width + bevel;
    for (int i=0; i < 8; i++) {
        black_row2[i].x = x;
        black_row2[i].y = y;
        x += square_width;
        all_pieces[count] = black_row2[i];
        count += 1;
    }

    //white pieces
    obj w_rook1 = obj(0.75, LoadTexture("../res/w-rook.png"));
    obj w_knight1 = obj(0.75, LoadTexture("../res/w-knight.png"));
    obj w_bishop1 = obj(0.75, LoadTexture("../res/w-bishop.png"));
    obj w_queen = obj(0.75, LoadTexture("../res/w-queen.png"));
    obj w_king = obj(0.75, LoadTexture("../res/w-king.png"));
    obj w_bishop2 = obj(0.75, LoadTexture("../res/w-bishop.png"));
    obj w_knight2 = obj(0.75, LoadTexture("../res/w-knight.png"));
    obj w_rook2 = obj(0.75, LoadTexture("../res/w-rook.png"));

    obj white_row1[8] = {w_rook1, w_knight1, w_bishop1, w_queen, w_king, w_bishop2, w_knight2, w_rook2};

    x = bevel;
    y = 7 * square_width + bevel;
    for (int i=0; i < 8; i++) {
        white_row1[i].x = x;
        white_row1[i].y = y;
        x += square_width;
        all_pieces[count] = white_row1[i];
        count += 1;
    }

    obj w_pawn1 = obj(0.75, LoadTexture("../res/w-pawn.png"));
    obj w_pawn2 = obj(0.75, LoadTexture("../res/w-pawn.png"));
    obj w_pawn3 = obj(0.75, LoadTexture("../res/w-pawn.png"));
    obj w_pawn4 = obj(0.75, LoadTexture("../res/w-pawn.png"));
    obj w_pawn5 = obj(0.75, LoadTexture("../res/w-pawn.png"));
    obj w_pawn6 = obj(0.75, LoadTexture("../res/w-pawn.png"));
    obj w_pawn7 = obj(0.75, LoadTexture("../res/w-pawn.png"));
    obj w_pawn8 = obj(0.75, LoadTexture("../res/w-pawn.png"));

    obj white_row2[8] = {w_pawn1, w_pawn2, w_pawn3, w_pawn4, w_pawn5, w_pawn6, w_pawn7, w_pawn8};

    x = bevel;
    y = 6 * square_width + bevel;
    for (int i=0; i < 8; i++) {
        white_row2[i].x = x;
        white_row2[i].y = y;
        x += square_width;
        all_pieces[count] = white_row2[i];
        count += 1;
    }


    //Game loop
    while (!WindowShouldClose()) {
        //drawing objects
        BeginDrawing();
        ClearBackground(WHITE);
        board.draw();
        draw_pieces(all_pieces);
        EndDrawing();

        //game functionality
        if (IsMouseButtonPressed(0)) {
            int mouse_x = GetMouseX();
            int mouse_y = GetMouseY();
            if(is_piece(mouse_x, mouse_y, all_pieces)) {
                obj* piece = get_piece(mouse_x, mouse_y, all_pieces);
                piece->color = RED;
            }
            
        }
    }

    CloseWindow();
    return 0;
}