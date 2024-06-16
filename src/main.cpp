#include <iostream>
#include <unordered_map>
#include <tuple>
#include <vector>
#include "raylib.h"
#include "game.h"

const int SCREEN_WIDTH = 784;
const int SCREEN_HEIGHT = 784;

int main() {
    //window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Chess");
    SetTargetFPS(30);
    Game* game = new Game();

    //Game loop
    while (!WindowShouldClose()) {
        //drawing objects
        BeginDrawing();
        ClearBackground(WHITE);
        game->draw_game(); //draws pieces and board
        if (game->is_promoting != -1) {
            game->promotion_menu(); //lets user select their promo
        } else if (game->game_over != 0) {
            game->draw_game_over(); //displays winner and play again button
        }
        EndDrawing();

        //functionality
        if (game->game_over == 0 && game->is_promoting == -1) { //game not over and not promoting
            if (game->player_turn) {
                if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) { //checking last board
                    game->go_back_board();
                } else if (game->is_paused && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { //undo checking history
                    game->go_foward_board();
                } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { //making move
                    int pos = game->get_index(GetMouseX(), GetMouseY());
                    if (pos != -1) {
                        game->select_move(pos);
                    }
                }
            } else {
                game->engine_move(4);
            }

        } else if (game->game_over == 0) { //game not over and must be promoting
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                int x = GetMouseX();
                int y = GetMouseY();

                game->pick_a_piece(x, y); //pick piece to promote
            }
        } else { //game over, check for playing again
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                int x = GetMouseX();
                int y = GetMouseY();
                if (game->check_play_again(x, y)) { //check if play again was clicked
                    //unload textures
                    UnloadTexture(game->board_texture);
                    UnloadTexture(game->select_texture);
                    for (auto& skin : game->skins) {
                        UnloadTexture(skin.second);
                    }
                    //make new game
                    delete game;
                    game = new Game();
                }
            }
        }
    }
    //unloading textures
    UnloadTexture(game->board_texture);
    UnloadTexture(game->select_texture);
    for (auto& skin : game->skins) {
        UnloadTexture(skin.second);
    }
    delete game;
    CloseWindow();
    return 0;
}