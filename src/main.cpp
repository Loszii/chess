#include <iostream>
#include <unordered_map>
#include <tuple>
#include <vector>
#include "raylib.h"
#include "game.h"

/*
to do: 

-create a end game table for king to be aggressive and checkmate

-use two stacks with board structs to undo moves

-make a function get all captures that returns all most that are captures, continue our negamax after depth 0 through all captures to prevent horizon

-put castling in front of king for alpha beta pruning
*/


//board represented as array, squares range from [0, 63]
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
        if (game.is_promoting != -1) {
            game.promotion_menu();
        }
        EndDrawing();

        //functionality
        if (game.game_over == 0 && game.is_promoting == -1) {
            if (game.player_turn) {
                if (IsMouseButtonPressed(0)) {
                    int pos = game.get_index(GetMouseX(), GetMouseY());
                    if (pos != -1) {
                        game.select_move(pos);
                    }
                }
                //game.engine_move(3); (to have engine vs engine)
            } else {
                game.engine_move(3);
            }
        } else if (game.is_promoting != -1) {
            if (IsMouseButtonPressed(0)) {
                int x = GetMouseX();
                int y = GetMouseY();

                game.pick_a_piece(x, y); //pick piece to promote
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