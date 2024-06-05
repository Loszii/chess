#include <iostream>
#include <unordered_map>
#include <tuple>
#include <vector>
#include "engine.h"
#include "raylib.h"
#include "game.h"

//to do: draw if no material, make color given random
/*
General rule is no pawns on the board and both sides of either of the following for insufficcient material:
1.a lone king
2.a king and bishop
3.a king and knight
*/
//the threefold repition rule can be calculated by hashing the board and counting its occurance, once one hits 3 it is a draw
//must also hash castle, and play turn/en passant

/*in this program positions on the board are represented by i*10 + j for simplicity and stored as an integer
this means accessing a position is [pos/10][pos%10] and saving a pos is i*10 + j*/
const int SCREEN_WIDTH = 784;
const int SCREEN_HEIGHT = 784;
const int FONT_SIZE = 100;

int main() {
    //main game loop

    //window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Chess");
    SetTargetFPS(30);

    Game game = Game();

    //fill_values(); //for engine

    //Game loop
    while (!WindowShouldClose()) {

        //drawing objects
        BeginDrawing();
        ClearBackground(WHITE);
        
        //draws pieces and board
        game.draw_game();

        //shades in selected squares
        if (game.select) {
            game.draw_select(game.select_pos, Color{0, 0, 0, 150});
            for (int i=0; i < (int)game.moves.size(); i++) {
                game.draw_select(game.moves[i], Color{0, 0, 0, 150});
            }
        }

        //check notifier
        if (game.w_check) {
            game.draw_select(game.w_king_pos, Color{255, 0, 0, 100});
        } else if (game.b_check) {
            game.draw_select(game.b_king_pos, Color{255, 0, 0, 100});
        }

        //draw in last move:
        if (game.past_moves[0] != -1) {
            game.draw_select(game.past_moves[0], Color{0, 0, 255, 100});
            game.draw_select(game.past_moves[1], Color{0, 0, 255, 100});
        }

        //checking for game over and displaying end game status
        if (game.game_over == 2 || game.game_over == -2) {
            int text_width = MeasureText("CHECKMATE", FONT_SIZE);
            int x = (SCREEN_WIDTH - text_width) / 2;
            int y = (SCREEN_HEIGHT - FONT_SIZE) / 2;
            DrawText("CHECKMATE", x, y, FONT_SIZE, RED);
        } else if (game.game_over == 1 || game.game_over == -1) {
            int text_width = MeasureText("STALEMATE", FONT_SIZE);
            int x = (SCREEN_WIDTH - text_width) / 2;
            int y = (SCREEN_HEIGHT - FONT_SIZE) / 2;
            DrawText("STALEMATE", x, y, FONT_SIZE, RED);
        }

        EndDrawing();

        //game functionality 
        if (game.game_over == 0) {
            if (game.w_turn) {
                if (IsMouseButtonPressed(0)) {
                    int pos = game.get_index(GetMouseX(), GetMouseY());
                    if (pos != -1) { //-1 means not a square
                        if (game.select) {
                            //check if mouse was clicked on a available move
                            for (int i=0; i < (int)game.moves.size(); i++) {
                                if (pos == game.moves[i]) {
                                    game.past_moves[0] = game.select_pos;
                                    game.past_moves[1] = pos;
                                    game.move_piece(game.select_pos, pos);
                                    if (game.w_turn) {
                                        game.w_turn = false;
                                    } else {
                                        game.w_turn = true;
                                    }
                                    game.select = false;
                                    break;
                                }
                            }
                            if (game.select) { //if still selecting and didnt make viable move
                                game.check_for_selection(pos); //checks to find selection and sets moves to all moves
                            } else { //no longer selecting so we moved a piece
                                game.updater();
                            }
                        } else { //get pos of selection if piece
                            game.check_for_selection(pos);
                        }
                    }
                }
            } else {
                //engine code
                std::vector<int> result = make_best_move(game);
                //need to update past moves
                game.past_moves[0] = result[0];
                game.past_moves[1] = result[1];
                game.move_piece(result[0], result[1]);
                if (game.w_turn) {
                    game.w_turn = false;
                } else { //for now this will only run since engine always black
                    game.w_turn = true;
                }
                game.updater();
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