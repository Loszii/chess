#include <iostream>
#include <unordered_map>
#include <tuple>
#include <vector>
#include "raylib.h"
#include "game.h"

//to do: use two stacks with board structs to undo moves

//3 move repition faulty regarding updating castling only when it is a players move to go. maybe fix

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
            //check if mouse selects a piece to promote, then calls apply_prom(is_promoting + pieceval*100)
            if (IsMouseButtonPressed(0)) {
                int x = GetMouseX();
                int y = GetMouseY();
                //put below in game function prob
                if ((SCREEN_WIDTH - 256) / 2 <= x && x <= ((SCREEN_WIDTH - 256) / 2) + 128) {//bishop
                    if ((SCREEN_WIDTH - 256) / 2 <= y && y <= ((SCREEN_WIDTH - 256) / 2) + 128) {
                        game.apply_promotion(200 + game.is_promoting);
                    }
                }
                if ((SCREEN_WIDTH - 256) / 2 + 128 <= x && x <= ((SCREEN_WIDTH - 256) / 2) + 256) {//knight
                    if ((SCREEN_WIDTH - 256) / 2 <= y && y <= ((SCREEN_WIDTH - 256) / 2) + 128) {
                        game.apply_promotion(300 + game.is_promoting);
                    }
                }
                if ((SCREEN_WIDTH - 256) / 2 <= x && x <= ((SCREEN_WIDTH - 256) / 2) + 128) {//rook
                    if (((SCREEN_WIDTH - 256) / 2) + 128 <= y && y <= ((SCREEN_WIDTH - 256) / 2) + 256) {
                        game.apply_promotion(400 + game.is_promoting);
                    }
                }
                if ((SCREEN_WIDTH - 256) / 2 + 128 <= x && x <= ((SCREEN_WIDTH - 256) / 2) + 256) {//queen
                    if (((SCREEN_WIDTH - 256) / 2) + 128 <= y && y <= ((SCREEN_WIDTH - 256) / 2) + 256) {
                        game.apply_promotion(500 + game.is_promoting);
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