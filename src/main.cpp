#include <iostream>
#include <unordered_map>
#include <tuple>
#include <vector>
#include "raylib.h"
#include "game.h"

//to do: re-implement most features, maybe use two stacks with board structs to undo moves
//board object will store en passant/castle/any board info
//add unit tests once concrete on implementation

//re-implement old features with old code, however use the Board object as a function call instead

//all positions in this program are of form i*10 + j

const int SCREEN_WIDTH = 784;
const int SCREEN_HEIGHT = 784;

int main() {
    //window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Chess");
    SetTargetFPS(30);
    Game game = Game();

    //Game loop
    while (!WindowShouldClose()) {
        //drawing objects
        BeginDrawing();
        ClearBackground(WHITE);
        game.draw_game(); //draws pieces and board
        EndDrawing();

        //functionality
        if (game.game_over == 0) {
            if (game.board.w_turn) {
                if (IsMouseButtonPressed(0)) {
                    int pos = game.get_index(GetMouseX(), GetMouseY());
                    if (pos != -1) {
                        game.select_move(pos);
                    }
                }
            } else {
                //oppponent turn
                //for now just same thing, this code will eventually be a minimax algorithm
                if (IsMouseButtonPressed(0)) {
                    int pos = game.get_index(GetMouseX(), GetMouseY());
                    if (pos != -1) {
                        game.select_move(pos);
                    }
                }
            }
        }
    }
    //unloading textures
    UnloadTexture(game.board_texture);
    UnloadTexture(game.select_texture);
    for (auto& skin : game.skins) {
        UnloadTexture(skin.second);
    }
    CloseWindow();
    return 0;
}