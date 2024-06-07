#include <iostream>
#include <unordered_map>
#include <tuple>
#include <vector>
#include <array>
#include "game.h"
#include "raylib.h"

Game::Game() {
    board_texture = LoadTexture("../res/board.png");
    select_texture = LoadTexture("../res/selector.png");

    //skins
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

    //coord
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
}

void Game::draw_game() {
    DrawTexture(board_texture, 0, 0, WHITE); //board
    int x;
    int y;
    for (int i=0; i < 80; i += 10) {
        for (int j=0; j < 8; j++) {
            if (board.data[i/10][j] != 0) {
                int ind = i + j;
                x = std::get<0>(coord[ind]);
                y = std::get<1>(coord[ind]);
                DrawTextureEx(skins[board.data[i/10][j]], (Vector2){(float)x, (float)y}, 0, SCALE, WHITE); //pieces
            }
        }
    }

    //shades in selected squares
    if (select) {
        draw_select(select_pos, Color{0, 0, 0, 150});
        for (int i=0; i < (int)moves.size(); i++) {
            draw_select(moves[i], Color{0, 0, 0, 150});
        }
    }

    if (board.w_check) {
        draw_select(board.w_king_pos, Color{255, 0, 0, 150});
    } else if (board.b_check) {
        draw_select(board.b_king_pos, Color{255, 0, 0, 150});
    }

    //game over
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
}

void Game::draw_select(int pos, Color color) {
    //shades in a square at the given pos of form i*10 + j
    int x = std::get<0>(coord[pos]);
    int y = std::get<1>(coord[pos]);
    DrawTextureEx(select_texture, (Vector2){(float)x, (float)y}, 0, 1, color);
}

void Game::select_move(int pos) {
    //takes in a position and selects either a piece or a move
    if (select) {
        //check if mouse was clicked on a available move
        for (int i=0; i < (int)moves.size(); i++) {
            if (pos == moves[i]) {
                move_piece(select_pos, pos, board);
                swap_turn();
                select = false;
                break;
            }
        }
        if (select) { //if still selecting and didnt make viable move
            check_for_selection(pos); //checks to find selection and sets moves to all moves
        } else { //no longer selecting so we moved a piece
            update_board(select_pos, pos);
        }
    } else {
        check_for_selection(pos);
    }
}

void Game::check_for_selection(int pos) {
    //checks if there is a piece to select at given pos, if so sets select to true, select_pos to pos, sets moves to all moves of that piece
    int i = pos/10;
    int j = pos % 10;
    if ((board.w_turn && board.data[i][j] > 0) || (!board.w_turn && board.data[i][j] < 0)) { //pos num 0-100 representing 2d array indices
        //below code runs if there is a move to select
        select = true;
        select_pos = pos;
        moves = get_legal_moves(pos, board); //gets possible moves
    } else {
        select = false;
    }
}

int Game::get_index(int x, int y) {
    //takes in mouse x and y pos and returns i*10 + j position
    int i = x - BEVEL;
    i = i / SQUARE_WIDTH;
    int j = y - BEVEL;
    j = j / SQUARE_WIDTH;
    if (i >= 0 && i <= 7 && j >= 0 && j <= 7) {
        return 10*j + i;
    } else {
        return -1; //cannot find index
    }
}

void Game::swap_turn() {
    if (board.w_turn) {
        board.w_turn = false;
    } else {
        board.w_turn = true;
    }
}

void Game::update_board(int start_pos, int end_pos) {
    //updates board by calling our checking functions, takes in data about last move for en passant
    promote_pawns(board);
    check_en_passant(start_pos, end_pos, board);
    std::vector<int> enemy_moves = get_enemy_moves(board.w_turn, board);
    if (board.w_turn) { //updating before white moves
        board.b_check = false;
        board.w_king_pos = get_piece_pos(6);
        board.w_check = under_attack(board.w_king_pos, enemy_moves);
    } else {
        board.w_check = false;
        board.b_king_pos = get_piece_pos(-6);
        board.b_check = under_attack(board.b_king_pos, enemy_moves);
    }
    check_castle(board.w_turn, board);
    check_game_over(board);
}

bool Game::under_attack(int pos, std::vector<int> enemy_moves) {
    //returns true if position is in enemy moves array
    for (int i=0; i < (int)enemy_moves.size(); i++) {
        if (enemy_moves[i] == pos) {
            return true;
        }
    }
    return false;
}

int Game::get_piece_pos(int piece) {
    //returns first occurance of piece within the current board, -1 if none
    for (int i=0; i < 8; i++) {
        for (int j=0; j < 8; j++) {
            if (board.data[i][j] == piece) {
                return i*10 + j;
            }
        }
    }
    return -1;
};

void Game::check_game_over(Board& board) {
    //checks if game has ended, 2 = checkmate, 1 = stalemate, negative is if black won
    std::vector<std::vector<int>> all_moves = get_all_legal_moves(board);
    if ((int)all_moves.size() == 0) {
        if (board.w_turn && board.w_check) {
            game_over = -2;
        } else {
            game_over = -1;
        }
        if (!board.w_turn && board.b_check) {
            game_over = 2;
        } else {
            game_over = 1;
        }
    }
}

int Game::move_piece(int start_pos, int end_pos, Board& board) {
    //given a start pos and end pos, moves piece at start to end
    //returns the piece that was captured if any
    int i = start_pos/10;
    int j = start_pos%10;
    //for castling:
    int row;
    int side;
    bool check_castle = false;
    if (board.data[i][j] == 6) { //is king
        row = 70;
        side = 1;
        check_castle = true;
    } else if (board.data[i][j] == -6) {
        row = 0;
        side = -1;
        check_castle = true;
    }
    if (check_castle) {
        if (start_pos == row + 4 && end_pos == row + 6) {
            //right castle so move rook
            board.data[row/10][5] = side * 4;
            board.data[row/10][7] = 0;
        } else if (start_pos == row + 4 && end_pos == row + 2) {
            //left castle so move rook
            board.data[row/10][3] = side * 4;
            board.data[row/10][0] = 0;
        }
    }

    //en passant
    //if pawn moving diagonal and square is empty then delete the pawn below
    int dir;
    int u_bound;
    bool check_en_passant = false;
    if (board.data[i][j] == 1) { //white pawn
        dir = -1;
        u_bound = 0;
        check_en_passant = true;
    } else if (board.data[i][j] == -1) {
        dir = 1;
        u_bound = 7;
        check_en_passant = true;
    }
    if (check_en_passant) {
        if (i != u_bound) { //diags
            if (j > 0) {
                if (end_pos == (i+dir)*10 + (j-1) && board.data[i+dir][j-1] == 0) {
                    board.data[end_pos/10 - dir][end_pos%10] = 0;
                }
            }
            if (j < 7) {
                if (end_pos == (i+dir)*10 + j+1 && board.data[i+dir][j+1] == 0) {
                    board.data[end_pos/10 - dir][end_pos%10] = 0;
                }
            }
        }
    }

    //actually move piece
    int captured_piece = board.data[end_pos/10][end_pos%10];
    board.data[end_pos/10][end_pos%10] = board.data[start_pos/10][start_pos%10];
    board.data[start_pos/10][start_pos%10] = 0;
    return captured_piece;
}

void Game::undo_move_piece(int start_pos, int end_pos, int captured_piece, Board& board) {
    //undoes what the move_piece function changes
    //promotion
    if (board.promotion_pos == end_pos) {
        if (board.data[end_pos/10][end_pos%10] > 0) {
            board.data[start_pos/10][start_pos%10] = 1;
            board.data[end_pos/10][end_pos%10] = captured_piece;
            return;
        } else {
            board.data[start_pos/10][start_pos%10] = -1;
            board.data[end_pos/10][end_pos%10] = captured_piece;
            return;
        }
    }
    //en passant or castle
    if (captured_piece == 0) {
        if (board.data[end_pos/10][end_pos%10] == 1) { //is pawn that moved
            if (end_pos == start_pos - 9 || end_pos == start_pos - 11) { //diagonal
                int pawn_pos = end_pos + 10;
                board.data[pawn_pos/10][pawn_pos%10] = -1;
            }
        } else if (board.data[end_pos/10][end_pos%10] == -1) {
            if (end_pos == start_pos + 11 || end_pos == start_pos + 9) { //diagonal
                int pawn_pos = end_pos - 10;
                board.data[pawn_pos/10][pawn_pos%10] = 1;
            }
        }
        //castle
        if (board.data[end_pos/10][end_pos%10] == 6 || board.data[end_pos/10][end_pos%10] == -6) {
            if (end_pos - start_pos == 2) { //castled right
                int rook_pos = end_pos - 1;
                int rook_dest = end_pos + 1;
                board.data[rook_dest/10][rook_dest%10] = board.data[rook_pos/10][rook_pos%10];
                board.data[rook_pos/10][rook_pos%10] = 0;
            } else if (end_pos - start_pos == -2) { //castled left
                int rook_pos = end_pos + 1;
                int rook_dest = end_pos - 2;
                board.data[rook_dest/10][rook_dest%10] = board.data[rook_pos/10][rook_pos%10];
                board.data[rook_pos/10][rook_pos%10] = 0;
            }
        }
    }

    //undoing actual move
    board.data[start_pos/10][start_pos%10] = board.data[end_pos/10][end_pos%10];
    board.data[end_pos/10][end_pos%10] = captured_piece;
}

std::vector<int> Game::get_pawn_moves(int i, int j, Board& board) {
    std::vector<int> result;
    bool w_turn; //must have local turn since we can be checking enemy pawn moves
    int u_bound;
    int dir;
    int start_row;
    if (board.data[i][j] > 0) {
        w_turn = true;
        u_bound = 0;
        dir = -1;
        start_row = 6;
    } else {
        w_turn = false;
        u_bound = 7;
        dir = 1;
        start_row = 1;
    }

    if (i == start_row && board.data[i+dir][j] == 0 && board.data[i+(2*dir)][j] == 0) {//straight
        result.push_back(((i+dir)*10)+j);
        result.push_back(((i+(2*dir))*10)+j);
    } else if (i != u_bound && board.data[i+dir][j] == 0) {
        result.push_back(((i+dir)*10)+j);
    }
    if (i != u_bound) { //diags
        if (j > 0) {
            if (w_turn && board.data[i+dir][j-1] < 0) {
                result.push_back((i+dir)*10 + (j-1));
            } else if (!w_turn && board.data[i+dir][j-1] > 0) {
                result.push_back((i+dir)*10 + (j-1));
            }
            //en passant
            if (w_turn && ((i+dir)*10 + (j-1) == board.en_passant) && board.en_passant/10 == 2) {
                result.push_back((i+dir)*10 + (j-1));
            } else if (!w_turn && ((i+dir)*10 + (j-1) == board.en_passant) && board.en_passant/10 == 5) {
                result.push_back((i+dir)*10 + (j-1));
            }
        }
        if (j < 7) {
            if (w_turn && board.data[i+dir][j+1] < 0) {
                result.push_back((i+dir)*10 + (j+1));
            } else if (!w_turn && board.data[i+dir][j+1] > 0) {
                result.push_back((i+dir)*10 + (j+1));
            }
            //en passant
            if (w_turn && ((i+dir)*10 + (j+1) == board.en_passant) && board.en_passant/10 == 2) {
                result.push_back((i+dir)*10 + (j+1));
            } else if (!w_turn && ((i+dir)*10 + (j+1) == board.en_passant) && board.en_passant/10 == 5) {
                result.push_back((i+dir)*10 + (j+1));
            }
        }
    }
    return result;
}

std::vector<int> Game::get_bishop_moves(int i, int j, Board& board) {
    std::vector<int> result;
    bool w_turn;
    if (board.data[i][j] > 0) {
        w_turn = true;
    } else {
        w_turn = false;
    }
    for (int k=1; k < j+1; k++) { //upper left
        if (i-k < 0) {
            break;
        }
        if (board.data[i-k][j-k] == 0) {
            result.push_back((i-k)*10 + (j-k));
        } else if ((w_turn && board.data[i-k][j-k] < 0) || (!w_turn && board.data[i-k][j-k] > 0)) { //piece there so break after
            result.push_back((i-k)*10 + (j-k));
            break;
        } else {
            break; //hit own piece so break
        }
    }
    for (int k=1; k < j+1; k++) { //lower left
        if (i+k > 7) {
            break;
        }
        if (board.data[i+k][j-k] == 0) {
            result.push_back((i+k)*10 + (j-k));
        } else if ((w_turn && board.data[i+k][j-k] < 0) || (!w_turn && board.data[i+k][j-k] > 0)) { 
            result.push_back((i+k)*10 + (j-k));
            break;
        } else {
            break;
        }
    }
    for (int k=1; k < (7-j)+1; k++) { //upper right
        if (i-k < 0) { //off screen
            break;
        }
        if (board.data[i-k][j+k] == 0) {
            result.push_back((i-k)*10 + (j+k));
        } else if ((w_turn && board.data[i-k][j+k] < 0) || (!w_turn && board.data[i-k][j+k] > 0)) { 
            result.push_back((i-k)*10 + (j+k));
            break;
        } else {
            break;
        }
    }
    for (int k=1; k < (7-j)+1; k++) { //lower right
        if (i+k > 7) { //off screen
            break;
        }
        if (board.data[i+k][j+k] == 0) {
            result.push_back((i+k)*10 + (j+k));
        } else if ((w_turn && board.data[i+k][j+k] < 0) || (!w_turn && board.data[i+k][j+k] > 0)) { 
            result.push_back((i+k)*10 + (j+k));
            break;
        } else {
            break;
        }
    }
    return result;
}

std::vector<int> Game::get_knight_moves(int i, int j, Board& board) {
    std::vector<int> result;
    bool w_turn;
    if (board.data[i][j] > 0) {
        w_turn = true;
    } else {
        w_turn = false;
    }
    if (i > 1) {//top 2
        if (j > 0 && ((w_turn && board.data[i-2][j-1] <= 0) || (!w_turn && board.data[i-2][j-1] >= 0))) {
            result.push_back((i-2)*10 + (j-1));
        }
        if (j < 7 && ((w_turn && board.data[i-2][j+1] <= 0) || (!w_turn && board.data[i-2][j+1] >= 0))) {
            result.push_back((i-2)*10 + (j+1));
        }
    }
    if (j < 6) {//right 2
        if (i > 0 && ((w_turn && board.data[i-1][j+2] <= 0) || (!w_turn && board.data[i-1][j+2] >= 0))) {
            result.push_back((i-1)*10 + (j+2));
        }
        if (i < 7 && ((w_turn && board.data[i+1][j+2] <= 0) || (!w_turn &&  board.data[i+1][j+2] >= 0))) {
            result.push_back((i+1)*10 + (j+2));
        }
    }
    if (i < 6) {//bottom 2
        if (j > 0 && ((w_turn && board.data[i+2][j-1] <= 0) || (!w_turn &&  board.data[i+2][j-1] >= 0))) {
            result.push_back((i+2)*10 + (j-1));
        }
        if (j < 7 && ((w_turn && board.data[i+2][j+1] <= 0) || (!w_turn &&  board.data[i+2][j+1] >= 0))) {
            result.push_back((i+2)*10 + (j+1));
        }
    }
    if (j > 1) {//left 2
        if (i > 0 && ((w_turn && board.data[i-1][j-2] <= 0) || (!w_turn &&  board.data[i-1][j-2] >= 0))) {
            result.push_back((i-1)*10 + (j-2));
        }
        if (i < 7 && ((w_turn && board.data[i+1][j-2] <= 0) || (!w_turn &&  board.data[i+1][j-2] >= 0))) {
            result.push_back((i+1)*10 + (j-2));
        }
    }
    return result;
}

std::vector<int> Game::get_rook_moves(int i, int j, Board& board) {
    std::vector<int> result;
    bool w_turn;
    if (board.data[i][j] > 0) {
        w_turn = true;
    } else {
        w_turn = false;
    }
    for (int k=1; k < i+1; k++) { //top col
        if (board.data[i-k][j] == 0) {
            result.push_back((i-k)*10 + j);
        } else if ((w_turn && board.data[i-k][j] < 0) || (!w_turn && board.data[i-k][j] > 0)) {
            result.push_back((i-k)*10 + j);
            break;
        } else {
            break;
        }
    }
    for (int k=1; k < (7-j)+1; k++) { //right col
        if (board.data[i][j+k] == 0) {
            result.push_back(i*10 + (j+k));
        } else if ((w_turn && board.data[i][j+k] < 0) || (!w_turn && board.data[i][j+k] > 0)) {
            result.push_back(i*10 + (j+k));
            break;
        } else {
            break;
        }
    }
    for (int k=1; k < (7-i)+1; k++) { //bottom col
        if (board.data[i+k][j] == 0) {
            result.push_back((i+k)*10 + j);
        } else if ((w_turn && board.data[i+k][j] < 0) || (!w_turn && board.data[i+k][j] > 0)) {
            result.push_back((i+k)*10 + j);
            break;
        } else {
            break;
        }
    }
    for (int k=1; k < j+1; k++) { //left col
        if (board.data[i][j-k] == 0) {
            result.push_back(i*10 + (j-k));
        } else if ((w_turn && board.data[i][j-k] < 0) || (!w_turn && board.data[i][j-k] > 0)) {
            result.push_back(i*10 + (j-k));
            break;
        } else {
            break;
        }
    }
    return result;
}

std::vector<int> Game::get_queen_moves(int i, int j, Board& board) {
    std::vector<int> result;
    std::vector<int> temp = get_bishop_moves(i, j, board);
    for (int k=0; k < (int)temp.size(); k++) {
        result.push_back(temp[k]);
    }
    temp = get_rook_moves(i, j, board);
    for (int k=0; k < (int)temp.size(); k++) {
        result.push_back(temp[k]);
    }
    return result;
}

std::vector<int> Game::get_king_moves(int i, int j, Board& board) {
    std::vector<int> result;
    bool w_turn;
    if (board.data[i][j] > 0) {
        w_turn = true;
    } else {
        w_turn = false;
    }
    if (i != 0) {//top row
        for (int k=-1; k < 2; k++) { //move left to right
            if (j+k >= 0 && j+k <= 7 && ((w_turn && board.data[i-1][j+k] <= 0) || (!w_turn && board.data[i-1][j+k] >= 0))) {
                result.push_back((i-1)*10 + (j+k));
            }
        }
    }
    if (i != 7) {//bottom row
        for (int k=-1; k < 2; k++) { 
            if (j+k >= 0 && j+k <= 7 && ((w_turn && board.data[i+1][j+k] <= 0) || (!w_turn && board.data[i+1][j+k] >= 0))) {
                result.push_back((i+1)*10 + (j+k));
            }
        }
    }
    if (j < 7 && ((w_turn && board.data[i][j+1] <= 0) || (!w_turn && board.data[i][j+1] >= 0))) {//right square
        result.push_back(i*10 + (j+1));
    }
    if (j > 0 && ((w_turn && board.data[i][j-1] <= 0) || (!w_turn && board.data[i][j-1] >= 0))) {//left square
        result.push_back(i*10 + (j-1));
    }
    //castling
    int row;
    std::array<bool, 4>* castle;
    if (w_turn) {
        row = 70;
        castle = &board.w_castle;
    } else {
        row = 0;
        castle = &board.b_castle;
    }
    if ((*castle)[0] && (*castle)[1]) { //right
        result.push_back(row + 6);
    }
    if ((*castle)[2] && (*castle)[3]) { //left
        result.push_back(row + 2);
    }
    return result;
}

std::vector<int> Game::get_trajectory(int pos, Board& board) {
    //given a board and square return all possible indices to move, legal or not
    //pos is a index of form i*10 + j
    //indices will be returned in same form

    std::vector<int> result;
    int i = pos / 10;
    int j = pos % 10;
    int piece = board.data[i][j];

    if (piece == 1 || piece == -1) {
        result = get_pawn_moves(i, j, board);
    } else if (piece == 2 || piece == -2) {
        result = get_bishop_moves(i, j, board);
    } else if (piece == 3 || piece == -3) {
        result = get_knight_moves(i, j, board);
    } else if (piece == 4 || piece == -4) {
        result = get_rook_moves(i, j, board);
    } else if (piece == 5 || piece == -5) {
        result = get_queen_moves(i, j, board);
    } else if (piece == 6 || piece == -6) {
        result = get_king_moves(i, j, board);
    }

    return result;
}

std::vector<int> Game::get_enemy_moves(bool w_turn, Board& board) {
    //returns all trajectories of the opposite color
    std::vector<int> result;
    for (int i=0; i < 80; i+=10) { //iterate over entire board
        for (int j=0; j < 8; j++) {
            if ((w_turn && board.data[i/10][j] < 0) || (!w_turn && board.data[i/10][j] > 0)) { //determines black or white side
                std::vector<int> trajectory = get_trajectory(i+j, board); //get all piece
                for (int k=0; k < (int)trajectory.size(); k++) {
                    result.push_back(trajectory[k]); //add to final array
                }
            }
        }
    }
    return result;
}

std::vector<int> Game::get_legal_moves(int pos, Board& board) {
    //uses board in parameter
    std::vector<int> result;
    std::vector<int> all_moves = get_trajectory(pos, board);
    int temp;
    int king;
    if (board.w_turn) {
        king = 6;
    } else {
        king = -6;
    }
    for (int i=0; i < (int)all_moves.size(); i++) {
        temp = move_piece(pos, all_moves[i], board);
        //disble en passant and promotion since not concerned with them
        int old_en_passant = board.en_passant;
        int old_promotion_pos = board.promotion_pos;
        board.en_passant = -1;
        board.promotion_pos = -1;
        std::vector<int> enemy_moves = get_enemy_moves(board.w_turn, board);
        if (!under_attack(get_piece_pos(king), enemy_moves)) {
            result.push_back(all_moves[i]);
        }
        undo_move_piece(pos, all_moves[i], temp, board);
        board.en_passant = old_en_passant;
        board.promotion_pos = old_promotion_pos;
    }
    return result;
}

std::vector<std::vector<int>> Game::get_all_legal_moves(Board& board) {
    //returns an array of arrays, with the first element being the position of piece and the rest its available moves
    std::vector<std::vector<int>> result;
    for (int i=0; i < 80; i+=10) { //iterate over entire board
        for (int j=0; j < 8; j++) {
            if ((board.w_turn && board.data[i/10][j] > 0) || (!board.w_turn && board.data[i/10][j] < 0)) { //determines black or white side
                std::vector<int> legal_moves = get_legal_moves(i+j, board); //get all piece
                std::vector<int> temp; //temp to prepend position to
                if (!legal_moves.empty()) {
                    temp.push_back(i+j); //add piece in front
                    for (int k=0; k < (int)legal_moves.size(); k++) {
                        temp.push_back(legal_moves[k]); //add to final array
                    }
                    result.push_back(temp);
                }
            }
        }
    }
    return result;
}

std::array<bool, 4> Game::check_castle(bool w_turn, Board& board) {
    //given turn and board object updates that players castling abilities
    //the old castle array will be returned for undoing this
    std::vector<int> enemy_moves = get_enemy_moves(w_turn, board);
    std::array<bool, 4>* castle;
    std::array<bool, 4> old_array;
    if (w_turn) {
        old_array = board.w_castle;
        castle = &board.w_castle;
    } else {
        old_array = board.b_castle;
        castle = &board.b_castle;
    }
    //white
    //perms
    int row;
    int side;
    if (w_turn) {
        side = 1;
        row = 7;
    } else {
        side = -1;
        row = 0;
    }
    if (board.data[row][4] != side * 6) { //king moved, all perm become false, dont undo these
        (*castle)[1] = false;
        (*castle)[3] = false;
    }
    if (board.data[row][7] != side * 4) { //right rook
        (*castle)[1] = false;
    }
    if (board.data[row][0] != side * 4) { //left rook
        (*castle)[3] = false;
    }

    //temps
    (*castle)[0] = true;
    (*castle)[2] = true;

    if (board.data[row][5] != 0 || board.data[row][6] != 0) { //piece in the way
        (*castle)[0] = false;
    }
    if (board.data[row][1] != 0 || board.data[row][2] != 0 || board.data[row][3] != 0) {
        (*castle)[2] = false;
    }

    if ((*castle)[0] || (*castle)[2]) { //if both are not already both false
        for (int i=0; i < (int)enemy_moves.size(); i++) { //iterate through all moves to see if a piece can attack square
                if (enemy_moves[i] == (10*row) + 4) {
                    (*castle)[0] = false;
                    (*castle)[2] = false;
                    break;
                }
                if (enemy_moves[i] == (10*row) + 5 || enemy_moves[i] == (10*row) + 6) {
                    (*castle)[0] = false;
                } else if (enemy_moves[i] == (10*row) + 2 || enemy_moves[i] == (10*row) + 3) {
                    (*castle)[2] = false;
                }
        }
    }

    return old_array;
}

int Game::check_en_passant(int start_pos, int end_pos, Board& board) {
    //checks for a pawn developing a en passant square.
    //sets en_passant in board and returns old value for undoing
    int old_val = board.en_passant;

    int diff;
    if (board.data[end_pos/10][end_pos%10] == 1) {
        //white pawn
        diff = end_pos - start_pos;
        if (diff == -20) {
            board.en_passant = start_pos - 10;
        }
    } else if (board.data[end_pos/10][end_pos%10] == -1) {
        diff = end_pos - start_pos;
        if (diff == 20) {
            board.en_passant = start_pos + 10;
        }
    }
    if (board.en_passant == old_val) {//cant be same square twice so no en_passant
        board.en_passant = -1;
    }
    return old_val;
}

int Game::promote_pawns(Board& board) {
    //promotes any pawns and sets promotion_pos in Board, returns the old promotion position
    int old_pos = board.promotion_pos;
    for (int i=0; i < 8; i++) {
        if (board.data[0][i] == 1) {
            board.data[0][i] = 5;
            board.promotion_pos = i;
            return old_pos;
        }
        if (board.data[7][i] == -1) {
            board.data[7][i] = -5;
            board.promotion_pos = 70+i;
            return old_pos;
        }
    }
    board.promotion_pos = -1;//nothing was promoted
    return old_pos;
}