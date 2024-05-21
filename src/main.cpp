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
        char name;
        obj(double scale, Texture2D texture, char name) {
            this->scale = scale;
            this->texture = texture;
            this->name = name;
        }
        obj() = default;
        //implement deconstructor
        void draw(){
            if (alive) {
                DrawTextureEx(texture, (Vector2){x, y}, 0, scale, WHITE);
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

    //to:do, make movable squares highlighted, have functions to recturn them given the object

    //window
    InitWindow(screen_width, screen_height, "Chess");
    SetTargetFPS(60);

    //board
    obj board = obj(1, LoadTexture("../res/board.png"), 'b');
    board.x = board.y = 0;

    //selector settings
    bool selector = false;
    int selector_x = 0;
    int selector_y = 0;
    Texture2D selector_texture = LoadTexture("../res/selector.png");

    //piece array
    obj all_pieces[32];
    int count = 0;

    //black pieces
    obj b_rook1 = obj(0.75, LoadTexture("../res/b-rook.png"), 'r');
    obj b_knight1 = obj(0.75, LoadTexture("../res/b-knight.png"), 'n');
    obj b_bishop1 = obj(0.75, LoadTexture("../res/b-bishop.png"), 'b');
    obj b_queen = obj(0.75, LoadTexture("../res/b-queen.png"), 'q');
    obj b_king = obj(0.75, LoadTexture("../res/b-king.png"), 'k');
    obj b_bishop2 = obj(0.75, LoadTexture("../res/b-bishop.png"), 'b');
    obj b_knight2 = obj(0.75, LoadTexture("../res/b-knight.png"), 'n');
    obj b_rook2 = obj(0.75, LoadTexture("../res/b-rook.png"), 'r');

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

    obj b_pawn1 = obj(0.75, LoadTexture("../res/b-pawn.png"), 'p');
    obj b_pawn2 = obj(0.75, LoadTexture("../res/b-pawn.png"), 'p');
    obj b_pawn3 = obj(0.75, LoadTexture("../res/b-pawn.png"), 'p');
    obj b_pawn4 = obj(0.75, LoadTexture("../res/b-pawn.png"), 'p');
    obj b_pawn5 = obj(0.75, LoadTexture("../res/b-pawn.png"), 'p');
    obj b_pawn6 = obj(0.75, LoadTexture("../res/b-pawn.png"), 'p');
    obj b_pawn7 = obj(0.75, LoadTexture("../res/b-pawn.png"), 'p');
    obj b_pawn8 = obj(0.75, LoadTexture("../res/b-pawn.png"), 'p');

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
    obj w_rook1 = obj(0.75, LoadTexture("../res/w-rook.png"), 'r');
    obj w_knight1 = obj(0.75, LoadTexture("../res/w-knight.png"), 'n');
    obj w_bishop1 = obj(0.75, LoadTexture("../res/w-bishop.png"), 'b');
    obj w_queen = obj(0.75, LoadTexture("../res/w-queen.png"), 'q');
    obj w_king = obj(0.75, LoadTexture("../res/w-king.png"), 'k');
    obj w_bishop2 = obj(0.75, LoadTexture("../res/w-bishop.png"), 'b');
    obj w_knight2 = obj(0.75, LoadTexture("../res/w-knight.png"), 'n');
    obj w_rook2 = obj(0.75, LoadTexture("../res/w-rook.png"), 'r');

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

    obj w_pawn1 = obj(0.75, LoadTexture("../res/w-pawn.png"), 'p');
    obj w_pawn2 = obj(0.75, LoadTexture("../res/w-pawn.png"), 'p');
    obj w_pawn3 = obj(0.75, LoadTexture("../res/w-pawn.png"), 'p');
    obj w_pawn4 = obj(0.75, LoadTexture("../res/w-pawn.png"), 'p');
    obj w_pawn5 = obj(0.75, LoadTexture("../res/w-pawn.png"), 'p');
    obj w_pawn6 = obj(0.75, LoadTexture("../res/w-pawn.png"), 'p');
    obj w_pawn7 = obj(0.75, LoadTexture("../res/w-pawn.png"), 'p');
    obj w_pawn8 = obj(0.75, LoadTexture("../res/w-pawn.png"), 'p');

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

        if (selector) {
            DrawTextureEx(selector_texture, (Vector2){selector_x, selector_y}, 0, 1, (Color){255, 0, 0, 50});
        }

        EndDrawing();

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
            
        }
    }

    CloseWindow();
    return 0;
}