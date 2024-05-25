#include "raylib.h"
#include <iostream>
#include <unordered_map>
#include <tuple>
#include <vector>
#include "movement.h"

const int SCREEN_WIDTH = 784;
const int SCREEN_HEIGHT = 784;

const int SQUARE_WIDTH = 96;
const int BEVEL = 8;
const double SCALE = 0.75;

const int FONT_SIZE = 100;

/*in this program positions on the board are represented by i*10 + j for simplicity and stored as an integer
this means accessing a position is [pos/10][pos%10] and saving a pos is i*10 + j*/


//to do: checkmate, castling, en pessant
//implementation: when in check iterate through all moves possible and see if player still in check, if so that move will not be added to moveList, if no moves left that break out of check then game over

std::unordered_map<int, Texture2D> get_skins() {
    //returns a map that takes in integers representing chess pieces and returns their Texture
    /*key:
    + = white, - = black
    0: empty
    1: pawn
    2: bishop
    3: knight
    4: rook
    5: queen
    6: king*/
    
    std::unordered_map<int, Texture2D> skins;
    skins[1] = LoadTexture("../res/w-pawn.png");
    skins[2] = LoadTexture("../res/w-bishop.png");
    skins[3] = LoadTexture("../res/w-knight.png");
    skins[4] = LoadTexture("../res/w-rook.png");
    skins[5] = LoadTexture("../res/w-queen.png");
    skins[6] = LoadTexture("../res/w-king.png");

    skins[-1] = LoadTexture("../res/b-pawn.png");
    skins[-2] = LoadTexture("../res/b-bishop.png");
    skins[-3] = LoadTexture("../res/b-knight.png");
    skins[-4] = LoadTexture("../res/b-rook.png");
    skins[-5] = LoadTexture("../res/b-queen.png");
    skins[-6] = LoadTexture("../res/b-king.png");

    return skins;
}

std::unordered_map<int, std::tuple<int, int>> get_coord() {
    //makes the coord, a map of 2d indices of form i*10 + j to a tuple of form (x,y)
    //this tuple represents the 2d-array squares physical position on the window.
    std::unordered_map<int, std::tuple<int, int>> coord;
    int x;
    int y;
    for (int i=0; i < 80; i += 10) {
        x = BEVEL;
        y = BEVEL + ((i/10) * SQUARE_WIDTH);
        for (int j=0; j < 8; j++) {
            coord[i+j] = std::make_tuple(x, y);
            x += SQUARE_WIDTH;
        }
    }
    
    return coord;
}

void drawGame(int board[8][8], std::unordered_map<int, std::tuple<int, int>> coord, std::unordered_map<int, Texture2D> skins, Texture2D board_texture) {
    //draws the board and then all of the pieces on it
    DrawTexture(board_texture, 0, 0, WHITE);
    int x;
    int y;
    for (int i=0; i < 80; i += 10) {
        for (int j=0; j < 8; j++) {
            if (board[i/10][j] != 0) {
                int ind = i + j;
                x = std::get<0>(coord[ind]);
                y = std::get<1>(coord[ind]);
                DrawTextureEx(skins[board[i/10][j]], (Vector2){(float)x, (float)y}, 0, SCALE, WHITE);
            }
        }
    }
}

void drawSelect(std::unordered_map<int, std::tuple<int, int>> coord, int pos, Texture2D select_texture) {
    //shades in a square at the given pos of form i*10 + j
    int x = std::get<0>(coord[pos]);
    int y = std::get<1>(coord[pos]);
    DrawTextureEx(select_texture, (Vector2){(float)x, (float)y}, 0, 1, (Color){50, 0, 0, 100});
}

int get_index(int x, int y, std::unordered_map<int, std::tuple<int, int>> coord) {
    //takes in mouse x and y pos and returns i*10 + j
    int top;
    int bottom;
    int left;
    int right;
    for (int i=0; i < 80; i += 10) {
        for (int j=0; j < 8; j++) {
            top = std::get<1>(coord[i+j]);
            bottom = top + SQUARE_WIDTH;
            left = std::get<0>(coord[i+j]);
            right = left + SQUARE_WIDTH;
            if (left <= x && x <= right && top <= y && y <= bottom) {
                return i+j;
            }
        }
    }
    return -1; //cannot find index
}

//debug VVV func
void print_all_move(std::vector<std::vector<int>> all) {
    std::cout << '\n';
    for (int i=0; i < (int)all.size(); i++) {
        std::cout << '\n';
        for (int j=0; j < (int)all[i].size(); j++) {
            std::cout << all[i][j] << ", ";
        }
    }
}

void check_for_selection(int board[8][8], bool& select, int& select_coord, std::vector<int>& moves, bool white_turn, int pos) {
    //checks if there is a move to select at given pos, if so sets select to true, coord to pos, sets moves to all moves of that piece, and adjusts the size of moves. all passed by ref
    int i = pos/10;
    int j = pos % 10;
    if ((white_turn && board[i][j] > 0) || (!white_turn && board[i][j] < 0)) { //pos num 0-100 representing 2d array indices
        //below code runs if there is a move to select
        select = true;
        select_coord = pos;
        moves = get_moves(board, select_coord, true); //gets possible moves
    } else {
        select = false;
    }
}


int main() {
    //main game loop

    //window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Chess");
    SetTargetFPS(60);

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

    int game_over = 0; //0 for none, 1 for white winning, 2 for black winning, 3 for stalemate

    //implement castling here
    int white_king_pos = 74;
    bool white_check = false;
    int black_king_pos = 4;
    bool black_check = false;
    bool white_turn = true;
    std::vector<int> moves;

    std::vector<std::vector<int>> all_moves;
    std::vector<std::vector<int>> all_legal_moves;
    
    std::unordered_map<int, Texture2D> skins = get_skins(); //skin textures maps num to Texture2D
    std::unordered_map<int, std::tuple<int, int>> coord = get_coord(); //mapping of indices to x-y coord
    //2d indices of form vvvvv
    //00, 01, 02, 03, 04, 05, 06, 07,
    //10, 11, 12, 13, 14, 15, 16, 17...


    Texture2D board_texture = LoadTexture("../res/board.png");
    Texture2D select_texture = LoadTexture("../res/selector.png");
    bool select = false;
    int select_coord;

    //Game loop
    while (!WindowShouldClose()) {

        //drawing objects
        BeginDrawing();
        ClearBackground(WHITE);
        
        //draws pieces and board
        drawGame(board, coord, skins, board_texture);

        //shades in selected squares
        if (select) {
            drawSelect(coord, select_coord, select_texture);
            for (int i=0; i < (int)moves.size(); i++) {
                drawSelect(coord, moves[i], select_texture);
            }
        }

        if (white_check) {
            drawSelect(coord, white_king_pos, select_texture);
        } else if (black_check) {
            drawSelect(coord, black_king_pos, select_texture);
        }

        if (game_over == 1 || game_over == 2) {
            int text_width = MeasureText("CHECKMATE", FONT_SIZE);
            int x = (SCREEN_WIDTH - text_width) / 2;
            int y = (SCREEN_HEIGHT - FONT_SIZE) / 2;
            DrawText("CHECKMATE", x, y, FONT_SIZE, RED);
        } else if (game_over == 3) {
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
                            move_piece(select_coord, moves[i], board);
                            if (white_turn) {
                                white_turn = false;
                            } else {
                                white_turn = true;
                            }
                            select = false;
                            break;
                        }
                    }
                    if (select) { //if still selecting and didnt make viable move
                        check_for_selection(board, select, select_coord, moves, white_turn, pos); //checks to find selection and sets moves to all moves
                    } else {

                            //code to run after breaking out of loop (moving piece)

                            //first recalculate king pos
                            std::vector<int> king_positions = get_king_coord(board);
                            white_king_pos = king_positions[0];
                            black_king_pos = king_positions[1];

                            all_moves = get_all_moves(board, false);

                            //check all moves and see if kings in check, disgregarding first index signifying piece pos
                            white_check = false;
                            black_check = false;
                            for (int i=0; i < (int)all_moves.size(); i++) {
                                for (int j=1; j < (int)all_moves[i].size(); j++) {
                                    if (all_moves[i][j] == white_king_pos) {
                                        white_check = true;
                                    } else if (all_moves[i][j] == black_king_pos) {
                                        black_check = true;
                                    }
                                }
                            }

                            //get all legal moves, if none then either draw or checkmate.
                            //checkmate if in check, draw if not in check
                            all_legal_moves = get_all_moves(board, true);
                            if (white_turn) {
                                int white_available_moves = 0;
                                for (int i=0; i < (int)all_legal_moves.size(); i++) {
                                    int temp_pos = all_legal_moves[i][0];
                                    if (board[temp_pos/10][temp_pos%10] > 0) { //is white
                                        for (int j=1; j < (int)all_legal_moves[i].size(); j++) {
                                            white_available_moves += 1;
                                        }
                                    }
                                }
                                if (white_available_moves == 0 && white_check) { //checked and no moves so lost
                                    game_over = 2;
                                } else if (white_available_moves == 0) { //no moves but not in check so draw
                                    game_over = 3;
                                }
                            } else { //blacks turn
                                int black_available_moves = 0;
                                for (int i=0; i < (int)all_legal_moves.size(); i++) {
                                    int temp_pos = all_legal_moves[i][0];
                                    if (board[temp_pos/10][temp_pos%10] < 0) { //is black
                                        for (int j=1; j < (int)all_legal_moves[i].size(); j++) {
                                            black_available_moves += 1;
                                        }
                                    }
                                }
                                if (black_available_moves == 0 && black_check) {
                                    game_over = 1;
                                } else if (black_available_moves == 0) {
                                    game_over = 3;
                                }
                            }


                    }
                } else { //get pos of selection if piece
                    check_for_selection(board, select, select_coord, moves, white_turn, pos);
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