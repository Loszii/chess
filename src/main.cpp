#include <iostream>
#include <unordered_map>
#include <tuple>
#include <vector>
#include "helper.h"
#include "movement.h"
#include "engine.h"
#include "raylib.h"

//to do: draw if no material, make color given random, rematch button and score, timer as well
/*
General rule is no pawns on the board and both sides of either of the following for insufficcient material:
1.a lone king
2.a king and bishop
3.a king and knight
*/

/*in this program positions on the board are represented by i*10 + j for simplicity and stored as an integer
this means accessing a position is [pos/10][pos%10] and saving a pos is i*10 + j*/

void updater(int board[8][8], bool w_turn, int& w_king_pos, int& b_king_pos, bool& w_check, bool& b_check, int past_moves[2], bool w_castle[4], bool b_castle[4], int& game_over, int& en_passant, int& promotion_pos) {
    //code to run after moving piece, if called with w_turn = true then will check these conditions for white before they can move
    //check for pawn promotion before below since can affect check/castle
    en_passant = check_en_passant(board, past_moves[0], past_moves[1]);
    promotion_pos = check_pawn_promotion(board);
    //checks
    std::vector<int> king_coord = get_king_coord(board);
    w_king_pos = king_coord[0];
    b_king_pos = king_coord[1];
    if (w_turn) {
        w_check = in_check(board, true);
        b_check = false; //black just moved so must be false
        check_castle_conditions(board, w_castle, true);
    } else {
        w_check = false;
        b_check = in_check(board, false);
        check_castle_conditions(board, b_castle, false);
    }
    //checkmate or stalemate
    check_game_state(board, game_over, w_check, b_check, w_turn, en_passant);
}


int main() {
    //main game loop

    //window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Chess");
    SetTargetFPS(30);

    int board[8][8] = {
        {-4, -3, -2, -5, -6, -2, -3, -4},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {4, 3, 2, 5, 6, 2, 3, 4},
    };

    int game_over = 0; //0 for none, 1 for stalemate, 2 for checkmate, -1 / -2 for black winning or stale
    bool w_turn = true;
    bool w_castle[4] = {false, true, false, true}; //[right temp, right perm, left temp, left perm]
    bool b_castle[4] = {false, true, false, true};
    int en_passant = -1; //-1 if no squares available, if there is then this will be the pos
    int promotion_pos = -1; //-1 if nothing promoted, else will be square of promotion
    int w_king_pos = 74;
    int b_king_pos = 4;
    bool w_check = false;
    bool b_check = false;
    int past_moves[2] = {-1, -1}; //first is start position second is last position of the previous move made on board
    std::vector<int> moves;
    
    std::unordered_map<int, Texture2D> skins = get_skins(); //skin textures maps num to Texture2D
    std::unordered_map<int, std::tuple<int, int>> coord = get_coord(); //mapping of indices to x-y coord
    //2d indices of form vvvvv
    //00, 01, 02, 03, 04, 05, 06, 07,
    //10, 11, 12, 13, 14, 15, 16, 17...


    Texture2D board_texture = LoadTexture("../res/board.png");
    Texture2D select_texture = LoadTexture("../res/selector.png");
    bool select = false;
    int select_pos; //position of select in form i*10 + j

    fill_values(); //for engine

    //Game loop
    while (!WindowShouldClose()) {

        //drawing objects
        BeginDrawing();
        ClearBackground(WHITE);
        
        //draws pieces and board
        drawGame(board, coord, skins, board_texture);

        //shades in selected squares
        if (select) {
            drawSelect(coord, select_pos, select_texture, Color{0, 0, 0, 150});
            for (int i=0; i < (int)moves.size(); i++) {
                drawSelect(coord, moves[i], select_texture, Color{0, 0, 0, 150});
            }
        }

        //check notifier
        if (w_check) {
            drawSelect(coord, w_king_pos, select_texture, Color{255, 0, 0, 100});
        } else if (b_check) {
            drawSelect(coord, b_king_pos, select_texture, Color{255, 0, 0, 100});
        }

        //draw in last move:
        if (past_moves[0] != -1) {
            drawSelect(coord, past_moves[0], select_texture, Color{0, 0, 255, 100});
            drawSelect(coord, past_moves[1], select_texture, Color{0, 0, 255, 100});
        }

        //checking for game over and displaying end game status
        if (game_over == 2 || game_over == -2) {
            int text_width = MeasureText("CHECKMATE", FONT_SIZE);
            int x = (SCREEN_WIDTH - text_width) / 2;
            int y = (SCREEN_HEIGHT - FONT_SIZE) / 2;
            DrawText("CHECKMATE", x, y, FONT_SIZE, RED);
        } else if (game_over == 1 || game_over == -1) {
            int text_width = MeasureText("STALEMATE", FONT_SIZE);
            int x = (SCREEN_WIDTH - text_width) / 2;
            int y = (SCREEN_HEIGHT - FONT_SIZE) / 2;
            DrawText("STALEMATE", x, y, FONT_SIZE, RED);
        }

        EndDrawing();

        //game functionality 
        if (game_over == 0) {
            if (w_turn) {
                if (IsMouseButtonPressed(0)) {
                    int pos = get_index(GetMouseX(), GetMouseY(), coord);
                    if (pos != -1) { //-1 means not a square
                        if (select) {
                            //check if mouse was clicked on a available move
                            for (int i=0; i < (int)moves.size(); i++) {
                                if (pos == moves[i]) {
                                    past_moves[0] = select_pos;
                                    past_moves[1] = pos;
                                    move_piece(select_pos, pos, board);
                                    if (w_turn) {
                                        w_turn = false;
                                    } else {
                                        w_turn = true;
                                    }
                                    select = false;
                                    break;
                                }
                            }
                            if (select) { //if still selecting and didnt make viable move
                                check_for_selection(board, select, select_pos, moves, w_turn, pos, w_castle, b_castle, en_passant); //checks to find selection and sets moves to all moves
                            } else { //no longer selecting so we moved a piece
                                updater(board, w_turn, w_king_pos, b_king_pos, w_check, b_check, past_moves, w_castle, b_castle, game_over, en_passant, promotion_pos);
                            }
                        } else { //get pos of selection if piece
                            check_for_selection(board, select, select_pos, moves, w_turn, pos, w_castle, b_castle, en_passant);
                        }
                    }
                }
            } else {
                //engine code
                std::vector<int> result = make_best_move(board, w_castle, b_castle, w_turn, en_passant);
                //need to update past moves
                past_moves[0] = result[0];
                past_moves[1] = result[1];
                move_piece(result[0], result[1], board);
                if (w_turn) {
                    w_turn = false;
                } else { //for now this will only run since engine always black
                    w_turn = true;
                }

                updater(board, w_turn, w_king_pos, b_king_pos, w_check, b_check, past_moves, w_castle, b_castle, game_over, en_passant, promotion_pos);
            }
        }
    }
    //unloading textures
    UnloadTexture(board_texture);
    UnloadTexture(select_texture);
    for (auto& skin : skins) {
        UnloadTexture(skin.second);
    }

    CloseWindow();
    return 0;
}