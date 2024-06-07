#include <iostream>
#include <unordered_map>
#include <tuple>
#include <vector>
#include "raylib.h"
#include "game.h"

//to do: use two stacks with board structs to undo moves

//idea: have update_board push_back all old values into array, make new undo_update that takes in this array and resets them all.
//can use this function in perft, minimax and legal moves <--- work on these funcs

//all positions in this program are of form i*10 + j

//TO DO TOMORROW:

//for promotion, make end_pos a number like 204, which represented a promotion to a bishop at pos 04. this way can undo move just given the positions.
//will need to change the way we select moves

//if a move with end_pos > 100 is in moves list, open a pawn promotion menu that allows user to pick
//must mod all positions by 100 to properly get their position on the board


//implement minimax inside of game.cpp (not in its own file)

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