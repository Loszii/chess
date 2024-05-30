#include <iostream>
#include <unordered_map>
#include <tuple>
#include <vector>
#include "helper.h"
#include "movement.h"
#include "raylib.h"

//to do: draw if no material and pawn promotion as well
//to do: make test cases for edge cases like pawn promotion, en passant, castling and legality of moves

//for en pessant, can only be one possible square at time, so give get_moves a position integer representing a viable en pessant square,
//for pawns check if their diagonals are this position, if so add the move to get moves and remove the pawn below (will take new piece moving code)

/*in this program positions on the board are represented by i*10 + j for simplicity and stored as an integer
this means accessing a position is [pos/10][pos%10] and saving a pos is i*10 + j*/

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

    int game_over = 0; //0 for none, 1 for checkmate, 2 for stalemate
    bool w_turn = true;
    bool w_castle[4] = {false, true, false, true}; //[right temp, right perm, left temp, left perm]
    bool b_castle[4] = {false, true, false, true};
    int en_passant = -1; //-1 if no squares available, if there is then this will be the pos
    int w_king_pos = 74;
    int b_king_pos = 4;
    bool w_check = false;
    bool b_check = false;
    int past_moves[2] = {0, 0}; //first is start position second is last position of the previous move made on board
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

    //Game loop
    while (!WindowShouldClose()) {

        //drawing objects
        BeginDrawing();
        ClearBackground(WHITE);
        
        //draws pieces and board
        drawGame(board, coord, skins, board_texture);

        //shades in selected squares
        if (select) {
            drawSelect(coord, select_pos, select_texture);
            for (int i=0; i < (int)moves.size(); i++) {
                drawSelect(coord, moves[i], select_texture);
            }
        }

        //check notifier
        if (w_check) {
            drawSelect(coord, w_king_pos, select_texture);
        } else if (b_check) {
            drawSelect(coord, b_king_pos, select_texture);
        }

        //checking for game over and displaying end game status
        if (game_over == 1) {
            int text_width = MeasureText("CHECKMATE", FONT_SIZE);
            int x = (SCREEN_WIDTH - text_width) / 2;
            int y = (SCREEN_HEIGHT - FONT_SIZE) / 2;
            DrawText("CHECKMATE", x, y, FONT_SIZE, RED);
        } else if (game_over == 2) {
            int text_width = MeasureText("STALEMATE", FONT_SIZE);
            int x = (SCREEN_WIDTH - text_width) / 2;
            int y = (SCREEN_HEIGHT - FONT_SIZE) / 2;
            DrawText("STALEMATE", x, y, FONT_SIZE, RED);
        }

        EndDrawing();

        //game functionality 
        if (game_over == 0 && IsMouseButtonPressed(0)) {
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
                    } else {

                            //code to run after breaking out of loop (moving piece)
                            std::vector<int> king_coord = get_king_coord(board); //make this take in turn and only get coord of that color
                            w_king_pos = king_coord[0];
                            b_king_pos = king_coord[1];

                            //true to check white and false for black
                            w_check = in_check(board, true);
                            b_check = in_check(board, false);
                            en_passant = check_en_passant(board, past_moves[0], past_moves[1]);
                            check_castle_conditions(board, w_castle, true);
                            check_castle_conditions(board, b_castle, false);
                            check_game_state(board, game_over, w_check, b_check, w_turn, en_passant);

                    }
                } else { //get pos of selection if piece
                    check_for_selection(board, select, select_pos, moves, w_turn, pos, w_castle, b_castle, en_passant);
                }
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