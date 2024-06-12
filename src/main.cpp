#include <iostream>
#include <unordered_map>
#include <tuple>
#include <vector>
#include "raylib.h"
#include "game.h"

/*
to do: 
-use two stacks with board structs to undo moves

-add evaluation boards for all pieces, and make the pieces weigh more (map of piece to a quantity >= 10)

-look into alpha beta pruning

-look over move generation and remove/improve redundant code
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
            } else {
                game.engine_move();
            }
        } else if (game.is_promoting != -1) {
            if (IsMouseButtonPressed(0)) {
                int x = GetMouseX();
                int y = GetMouseY();

                game.pick_a_piece(x, y);
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