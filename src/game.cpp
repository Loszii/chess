#include <iostream>
#include <unordered_map>
#include <tuple>
#include <vector>
#include <array>
#include <random>
#include "game.h"
#include "raylib.h"

Game::Game(bool textures) {
    if (textures) {
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
    }

    //player turn random
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, 1);
    int num = distr(gen);
    if (num == 0) {
        player_turn = false;
    } else {
        player_turn = true;
    }

    //coord
    int x = BEVEL;
    int y = BEVEL;
    if (player_turn) {
        for (int i=0; i < 64; i ++) {
            coord[i] = std::make_tuple(x, y);
            x += SQUARE_WIDTH;
            if (x == SCREEN_WIDTH - BEVEL) {
                x = BEVEL;
                y += SQUARE_WIDTH;
            }
        }
    } else { //flipped
        for (int i=0; i < 64; i ++) {
            coord[63-i] = std::make_tuple(x, y);
            x += SQUARE_WIDTH;
            if (x == SCREEN_WIDTH - BEVEL) {
                x = BEVEL;
                y += SQUARE_WIDTH;
            }
        }
    }
    //engine
    init_engine();
    //hashing first board
    history[board] = 1;
}

void Game::draw_game() {
    //draws the board, pieces, check indicators, available moves, and end game screens
    DrawTexture(board_texture, 0, 0, WHITE); //board
    int x;
    int y;
    for (int i=0; i < 64; i ++) {
        if (board.data[i] != 0) {
            x = std::get<0>(coord[i]);
            y = std::get<1>(coord[i]);
            DrawTextureEx(skins[board.data[i]], (Vector2){(float)x, (float)y}, 0, SCALE, WHITE); //pieces
        }
    }

    //shades in piece and possible moves
    if (select) {
        draw_select(select_pos, Color{0, 0, 0, 150});
        if (!moves.empty()) {
            if (moves[0] < 100) { //not a promoting pawn
                for (int i=0; i < (int)moves.size(); i++) {
                    draw_select(moves[i], Color{0, 0, 0, 150});
                }
            } else {
                for (int i=0; i < (int)promotion_positions.size(); i++) {
                    draw_select(promotion_positions[i], Color{0, 0, 0, 150});
                }
            }
        }
    }

    if (board.w_check) {
        draw_select(board.w_king_pos, Color{255, 0, 0, 150});
    } else if (board.b_check) {
        draw_select(board.b_king_pos, Color{255, 0, 0, 150});
    }
}

void Game::draw_game_over() {
    //draws the conclusion of match and play again button
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
    } else if (game_over == 3) { //3 draw
        int text_width = MeasureText("DRAW", FONT_SIZE);
        int x = (SCREEN_WIDTH - text_width) / 2;
        int y = (SCREEN_HEIGHT - FONT_SIZE) / 2;
        DrawText("DRAW", x, y, FONT_SIZE, RED);
    }
    //play again button

    //button 100x400
    int button_x = (SCREEN_WIDTH-400)/2;
    int button_y = (SCREEN_HEIGHT-100)/2 + SCREEN_HEIGHT/4;
    DrawRectangle(button_x-5, button_y-5, 410, 110, BLACK); //drop shadow
    DrawRectangle(button_x, button_y, 400, 100, WHITE);
    int text_width = MeasureText("PLAY AGAIN", FONT_SIZE/2);
    int padding_left = (400 - text_width)/2;
    int padding_top = (100 - FONT_SIZE/2)/2;
    DrawText("PLAY AGAIN", button_x + padding_left, button_y + padding_top, FONT_SIZE/2, BLACK);

}

void Game::draw_select(int pos, Color color) {
    //shades in a square at the given pos of form i*10 + j
    int x = std::get<0>(coord[pos]);
    int y = std::get<1>(coord[pos]);
    DrawTextureEx(select_texture, (Vector2){(float)x, (float)y}, 0, 1, color);
}

void Game::set_promotion_pos() {
    //sets promotion_positions to the positions a pawn can promote to or clears it if no current promotion
    //removes duplicates (303, 203, 403, 503 = 03)
    if (moves.empty()) {
        promotion_positions.clear();
        return;
    } else if (moves[0] < 100) { //nothing promoting
        promotion_positions.clear();
        return;
    }
    std::vector<int> result;
    bool already_checked = false;
    for (int i=0; i < (int)moves.size(); i++) {
        for (int j=0; j < (int)result.size(); j++) {
            if (moves[i]%100 == result[j]) {
                already_checked = true;
            }
        }
        if (!already_checked) {
            result.push_back(moves[i]%100);
        }
        already_checked = false;
    }
    promotion_positions = result;
}

void Game::apply_promotion(int pos) {
    //given a position of three digits, promote pawn at select_pos to pos
    save_board();
    update_board(select_pos, pos);
    check_draw();
    check_game_over(); //checks for end of game
    is_promoting = -1; //disables promotion menu
}

void Game::pick_a_piece(int x, int y) {
    if ((SCREEN_WIDTH - 256) / 2 <= x && x <= ((SCREEN_WIDTH - 256) / 2) + 128) {//bishop
        if ((SCREEN_WIDTH - 256) / 2 <= y && y <= ((SCREEN_WIDTH - 256) / 2) + 128) {
            apply_promotion(200 + is_promoting);
        }
    }
    if ((SCREEN_WIDTH - 256) / 2 + 128 <= x && x <= ((SCREEN_WIDTH - 256) / 2) + 256) {//knight
        if ((SCREEN_WIDTH - 256) / 2 <= y && y <= ((SCREEN_WIDTH - 256) / 2) + 128) {
            apply_promotion(300 + is_promoting);
        }
    }
    if ((SCREEN_WIDTH - 256) / 2 <= x && x <= ((SCREEN_WIDTH - 256) / 2) + 128) {//rook
        if (((SCREEN_WIDTH - 256) / 2) + 128 <= y && y <= ((SCREEN_WIDTH - 256) / 2) + 256) {
            apply_promotion(400 + is_promoting);
        }
    }
    if ((SCREEN_WIDTH - 256) / 2 + 128 <= x && x <= ((SCREEN_WIDTH - 256) / 2) + 256) {//queen
        if (((SCREEN_WIDTH - 256) / 2) + 128 <= y && y <= ((SCREEN_WIDTH - 256) / 2) + 256) {
            apply_promotion(500 + is_promoting);
        }
    }
}

void Game::go_back_board() {
    //sets board back to the previous board
    if (select) {
        select = false; //unselect piece
    }
    if (!past_boards.empty()) {
        future_boards.push(board); //push current 
        board = past_boards.top(); //get last board
        past_boards.pop();
        is_paused = true;
    }
}

void Game::go_foward_board() {
    //sets board to the future board if there is one
    if (!future_boards.empty()) {
        save_board();
        board = future_boards.top();
        future_boards.pop();

        //if empty now resume game
        if (future_boards.empty()) {
            is_paused = false;
        }
    }
}

bool Game::check_play_again(int x, int y) {
    //returns true if the user clicks play again button
    //button 100x400
    int button_x = (SCREEN_WIDTH-400)/2;
    int button_y = (SCREEN_HEIGHT-100)/2 + SCREEN_HEIGHT/4;
    if (button_x <= x && x <= button_x + 400) {
        if (button_y <= y && y <= button_y + 100) {
            return true;
        }
    }
    return false;

}

void Game::promotion_menu() {
    //prompts the player to select a piece to promote

    //menu 256px by 256px
    int side = 1;
    int menu_x = (SCREEN_WIDTH - 256) / 2;
    int menu_y = (SCREEN_WIDTH - 256) / 2;
    int piece_width = 128;
    //menu
    DrawRectangle(menu_x - 5, menu_y - 5, 256 + 10, 256 + 10, BLACK); //dropshadow
    DrawRectangle(menu_x, menu_y, 256, 256, WHITE);
    if (!board.w_turn) {
        side = -1;
    }
    DrawTextureEx(skins[side * 2], (Vector2){(float)menu_x, (float)menu_y}, 0, 1.0, WHITE);
    DrawTextureEx(skins[side * 3], (Vector2){(float)(menu_x + piece_width), (float)menu_y}, 0, 1.0, WHITE);
    DrawTextureEx(skins[side * 4], (Vector2){(float)menu_x, (float)(menu_y + piece_width)}, 0, 1.0, WHITE);
    DrawTextureEx(skins[side * 5], (Vector2){(float)(menu_x + piece_width), (float)(menu_y + piece_width)}, 0, 1.0, WHITE);
}

void Game::select_move(int pos) {
    //takes in a position and selects either a piece or a move to make
    if (select) {
        //check if mouse was clicked on a available move
        if (!moves.empty()) {
            if (moves[0] < 100) { //not promoting
                for (int i=0; i < (int)moves.size(); i++) {
                    if (pos == moves[i]) {
                        select = false;
                        save_board();
                        update_board(select_pos, pos);
                        check_draw();
                        check_game_over(); //checks for end of game
                        break;
                    }
                }
            } else {
                for (int i=0; i < (int)promotion_positions.size(); i++) {
                    if (pos == promotion_positions[i]) {
                        select = false;
                        is_promoting = pos;
                        break;
                    }
                }
            }
        }
        if (select) { //if still selecting and didnt make viable move
            check_for_selection(pos); //checks to find selection and sets moves to all moves
        }
    } else {
        check_for_selection(pos);
    }
}

void Game::check_for_selection(int pos) {
    //checks if there is a piece to select at given pos, if so sets select to true, select_pos to pos, sets moves to all moves of that piece
    if ((board.w_turn && board.data[pos] > 0) || (!board.w_turn && board.data[pos] < 0)) { //pos num 0-100 representing 2d array indices
        //below code runs if there is a move to select
        select = true;
        select_pos = pos;
        moves.clear();
        get_legal_moves(pos, moves); //gets possible moves
        set_promotion_pos(); //adds any promo squares
    } else {
        select = false;
    }
}

int Game::get_index(int x, int y) {
    //takes in mouse x and y pos and returns i*10 + j position
    int pos;
    y -= BEVEL;
    y = y / SQUARE_WIDTH;
    x -= BEVEL;
    x = x / SQUARE_WIDTH;
    pos = y*8 + x;
    if (pos >= 0 && pos <= 63) {
        if (player_turn && board.w_turn) {
            return pos;
        } else { //swap for playing as black
            return 63 - pos;
        }
    } else {
        return -1; //cannot find index
    }
}

void Game::swap_turn() {
    board.w_turn = !board.w_turn;
    player_turn = !player_turn;
}

Board Game::update_board(int start_pos, int end_pos) {
    //moves piece given params, then updates board attributes, first swaps the turn and then checks conditions before player moves
    //returns old board
    Board old_board = Board(board.data, board.w_turn, board.w_king_pos, board.b_king_pos, board.w_check, board.b_check, board.w_castle,
    board.b_castle, board.en_passant);

    move_piece(start_pos, end_pos);
    swap_turn();

    std::vector<int> enemy_moves = get_all_trajectories(!board.w_turn);
    std::vector<int> ally_moves = get_all_trajectories(board.w_turn);
    board.w_king_pos = get_piece_pos(6);
    board.b_king_pos = get_piece_pos(-6);
    if (board.w_turn) {
        board.w_check = under_attack(board.w_king_pos, enemy_moves);
        board.b_check = under_attack(board.b_king_pos, ally_moves);
    } else {
        board.w_check = under_attack(board.w_king_pos, ally_moves);
        board.b_check = under_attack(board.b_king_pos, enemy_moves);
    }
    check_castle(board.w_turn, enemy_moves);
    check_castle(!board.w_turn, ally_moves);
    check_en_passant(start_pos, end_pos);
    return old_board;
}

void Game::undo_update_board(Board old_board) {
    board = old_board;
    player_turn = !player_turn;
}

bool Game::under_attack(int pos, const std::vector<int>& enemy_moves) {
    //returns true if position is in enemy moves array
    for (int i=0; i < (int)enemy_moves.size(); i++) {
        if (enemy_moves[i]%100 == pos) { //mod 100 to ignore promotion code
            return true;
        }
    }
    return false;
}

int Game::get_piece_pos(int piece) {
    //returns first occurance of piece within the current board, -1 if none
    for (int i=0; i < 64; i++) {
        if (board.data[i] == piece) {
            return i;
        }
    }
    return -1;
};

void Game::check_game_over() {
    //checks if game has ended, 2 = checkmate, 1 = stalemate, negative is if black won
    std::vector<std::vector<int>> all_moves = get_all_legal_moves();
    if ((int)all_moves.size() == 0) {
        if (board.w_turn) {
            if (board.w_check) {
                game_over = -2;
            } else {
                game_over = -1;
            }
        } else {
            if (board.b_check) {
                game_over = 2;
            } else {
                game_over = 1;
            }
        }
    }
}

void Game::check_draw() {
    //sets game_over to 3 if draw
    if (hash_board()) { //3 repition
        game_over = 3;
    } else if (check_material()) { //insufficient material
        game_over = 3;
    }
}

bool Game::check_material() {
    //returns true if there is insufficient material and draw must be concluded
    //also checks if endgame and if so initializes endgame engine evaluation board

    //checking if in endgame for engine
    std::unordered_map<int, int> mats = get_material();
    int w_sum = 0;
    int b_sum = 0;
    for (int i=1; i < 6; i++) {
        w_sum += mats[i] * i; //*i to get value of piece
    }
    for (int i=-5; i < 0; i++) {
        b_sum += mats[i] * -i;
    }

    if (!end_game && (w_sum < 6 || b_sum < 6)) { //one side has only a few pieces (val <= 5)
        init_end_game();
        end_game = true;
    }

    //checking if insuff material results in draw
    //can only have one of below for each side and no other pieces (and king ofc)
    if (mats[-5] > 0 || mats[5] > 0) { //either have a queen
        return false;
    } else if (mats[-4] > 0 || mats[4] > 0) { //either have a rook
        return false;
    } else if (mats[-3] > 1 || mats[3] > 1) { //either have more than one knight
        return false;
    } else if (mats[-2] > 1 || mats[2] > 1) { //either have more than one bishop
        return false;
    } else if (mats[-3] == 1 && mats[-2] == 1) { //black has a bishop and a knight (can only have one)
        return false;
    } else if (mats[3] == 1 && mats[2] == 1) { //white has a bishop and a knight
        return false;
    } else if (mats[-1] > 0 || mats[1] > 0) { //either havs a pawn
        return false;
    } else { //ran out of material so must be draw
        return true;
    }
}

std::unordered_map<int, int> Game::get_material() {
    //returns a mapping of piece to num occurance
    std::unordered_map<int, int> result;
    //init values
    for (int i=-6; i < 7; i++) {
        if (i != 0) {
            result[i] = 0;
        }
    }
    //count em
    for (int i=0; i < 64; i++) {
        if (board.data[i] != 0) {
            result[board.data[i]] += 1;
        }
    }
    return result;
}

bool Game::hash_board() {
    //returns true if 3 repition is achieved, hashes current board
    if (history.find(board) != history.end()) { //in map
        history[board] += 1;
    } else {
        history[board] = 1;
    }
    if (history[board] == 3) { //must be draw
        return true;
    } else {
        return false;
    }
}

bool Game::is_prev_board() {
    //function to see if achieving current board state could end up in a draw (has been previously hashed)
    if (history.find(board) != history.end()) {
        return true;
    } else {
        return false;
    }
}

void Game::save_board() {
    //pushes current board onto stack, to be called after updating
    past_boards.push(board);
}

void Game::move_piece(int start_pos, int end_pos) {
    //given a start pos and end pos, moves piece at start to end position
    //this function handles en passant, castling, and promotions

    if (end_pos >= 100) { //promotion
        board.data[start_pos] = 0;
        int chopped_pos = end_pos % 100;
        int new_piece = end_pos / 100;
        if (!board.w_turn) {
            new_piece *= -1;
        }
        board.data[chopped_pos] = new_piece;
        return;
    }

    //for castling:
    int row;
    int side;
    bool check_castle = false;
    if (board.data[start_pos] == 6) { //is king
        row = 56;
        side = 1;
        check_castle = true;
    } else if (board.data[start_pos] == -6) {
        row = 0;
        side = -1;
        check_castle = true;
    }
    if (check_castle) {
        if (start_pos == row + 4 && end_pos == row + 6) {
            //right castle so move rook
            board.data[row+5] = side * 4;
            board.data[row+7] = 0;
        } else if (start_pos == row + 4 && end_pos == row + 2) {
            //left castle so move rook
            board.data[row+3] = side * 4;
            board.data[row] = 0;
        }
    }

    //en passant
    //if pawn moving diagonal and square is empty then delete the pawn below
    if (board.data[start_pos] == 1 || board.data[start_pos] == -1) {
        if (end_pos - start_pos == 7 || end_pos - start_pos == 9) { //black
            if (board.data[end_pos] == 0) {
                board.data[end_pos - 8] = 0;
            }
        } else if (end_pos - start_pos == -7 || end_pos - start_pos == -9) {
            if (board.data[end_pos] == 0) {
                board.data[end_pos + 8] = 0;
            }
        }
    }

    //actually move piece
    board.data[end_pos] = board.data[start_pos];
    board.data[start_pos] = 0;
}

void Game::get_pawn_moves(int pos, std::vector<int>& result) {
    //adds all pawn moves to result

    bool w_turn; //must have local turn since we can be checking enemy pawn moves
    //promotion checks
    bool will_promote = false;
    if (board.data[pos] > 0) { //whites turn
        if (pos >= 8 && pos <= 15) {
            will_promote = true;
        }
        //diagonal
        if ((pos-7) % 8 != 0 && board.data[pos-7] < 0) {
            result.push_back(pos-7);
        }
        if (pos % 8 != 0 && board.data[pos-9] < 0) {
            result.push_back(pos-9);
        }
        //en passant
        if ((pos - 7 == board.en_passant || pos - 9 == board.en_passant) && pos >= 24 && pos <= 31) {
            result.push_back(board.en_passant);
        }
        //straight squares
        if (board.data[pos-8] == 0) {
            result.push_back(pos-8);
        }
        if (pos >= 48 && pos <= 55 && board.data[pos-8] == 0 && board.data[pos-16] == 0) { //add extra
            result.push_back(pos-16);
        }
    } else {
        if (pos >= 48 && pos <= 55) {
            will_promote = true;
        }
        //diagonal
        if (pos % 8 != 0 && board.data[pos+7] > 0) {
            result.push_back(pos+7);
        }
        if ((pos-7) % 8 != 0 && board.data[pos+9] > 0) {
            result.push_back(pos+9);
        }
        //en passant
        if ((pos + 7 == board.en_passant || pos + 9 == board.en_passant) && pos >= 32 && pos <= 39) {
            result.push_back(board.en_passant);
        }
        //straight squares
        if (board.data[pos + 8] == 0) {
            result.push_back(pos+8);
        }
        if (pos >= 8 && pos <= 15 && board.data[pos+8] == 0 && board.data[pos+16] == 0) { //add extra
            result.push_back(pos+16);
        }
    }
    //if will promote loop through all promotion
    if (will_promote) {
        std::vector<int> promotion_result;
        for (int i=0; i < (int)result.size(); i++) {
            int pos = result[i];
            promotion_result.push_back(500 + pos); //bishop
            promotion_result.push_back(400 + pos);
            promotion_result.push_back(300 + pos);
            promotion_result.push_back(200 + pos);
        }
        result = promotion_result;
    }
}

void Game::get_bishop_moves(int pos, std::vector<int>& result) {
    //adds all bishop moves to result
    bool w_turn;
    if (board.data[pos] > 0) {
        w_turn = true;
    } else {
        w_turn = false;
    }
    int temp;
    //left side
    if (pos % 8 != 0) {
        //upper left (pos - 9)
        temp = pos - 9;
        while (temp >= 0 && (temp-7) % 8 != 0) {
            if (board.data[temp] == 0) {
                result.push_back(temp);
            } else if ((w_turn && board.data[temp] < 0) || (!w_turn && board.data[temp] > 0)) {
                result.push_back(temp);
                break;
            } else {
                break;
            }
            temp -= 9;
        }
        //lower left
        temp = pos + 7;
        while (temp <= 63 && (temp-7) % 8 != 0) {
            if (board.data[temp] == 0) {
                result.push_back(temp);
            } else if ((w_turn && board.data[temp] < 0) || (!w_turn && board.data[temp] > 0)) {
                result.push_back(temp);
                break;
            } else {
                break;
            }
            temp += 7;
        }
    }
    //right side
    if ((pos-7) % 8 != 0) {
        //upper right
        temp = pos - 7;
        while (temp >= 0 && temp % 8 != 0) {
            if (board.data[temp] == 0) {
                result.push_back(temp);
            } else if ((w_turn && board.data[temp] < 0) || (!w_turn && board.data[temp] > 0)) {
                result.push_back(temp);
                break;
            } else {
                break;
            }
            temp -= 7;
        }
        //lower right
        temp = pos + 9;
        while (temp <= 63 && temp % 8 != 0) {
            if (board.data[temp] == 0) {
                result.push_back(temp);
            } else if ((w_turn && board.data[temp] < 0) || (!w_turn && board.data[temp] > 0)) {
                result.push_back(temp);
                break;
            } else {
                break;
            }
            temp += 9;
        }
    }
}

void Game::get_knight_moves(int pos, std::vector<int>& result) {
    //adds all knight moves to result
    bool w_turn;
    if (board.data[pos] > 0) {
        w_turn = true;
    } else {
        w_turn = false;
    }
    //top 2
    int temp = pos - 16;
    if (temp >= 0) {
        if ((temp - 7) % 8 != 0) {
            if ((w_turn && board.data[temp+1] <= 0) || (!w_turn && board.data[temp+1] >= 0)) {
                result.push_back(temp+1);
            }
        }
        if (temp % 8 != 0) {
            if ((w_turn && board.data[temp-1] <= 0) || (!w_turn && board.data[temp-1] >= 0)) {
                result.push_back(temp-1);
            }
        }
    }
    //right 2
    if ((pos-7) % 8 != 0 && (pos - 6) % 8 != 0) {
        temp = pos + 2;
        if (temp-8 >= 0) {
            if ((w_turn && board.data[temp-8] <= 0) || (!w_turn && board.data[temp-8] >= 0)) {
                result.push_back(temp-8);
            }
        }
        if (temp+8 <= 63) {
            if ((w_turn && board.data[temp+8] <= 0) || (!w_turn && board.data[temp+8] >= 0)) {
                result.push_back(temp+8);
            }
        }
    }
    //bottom 2
    temp = pos + 16;
    if (temp <= 63) {
        if ((temp - 7) % 8 != 0) {
            if ((w_turn && board.data[temp+1] <= 0) || (!w_turn && board.data[temp+1] >= 0)) {
                result.push_back(temp+1);
            }
        }
        if (temp % 8 != 0) {
            if ((w_turn && board.data[temp-1] <= 0) || (!w_turn && board.data[temp-1] >= 0)) {
                result.push_back(temp-1);
            }
        }
    }
    //left 2
    if (pos % 8 != 0 && (pos - 1) % 8 != 0) {
        temp = pos - 2;
        if (temp-8 >= 0) {
            if ((w_turn && board.data[temp-8] <= 0) || (!w_turn && board.data[temp-8] >= 0)) {
                result.push_back(temp-8);
            }
        }
        if (temp+8 <= 63) {
            if ((w_turn && board.data[temp+8] <= 0) || (!w_turn && board.data[temp+8] >= 0)) {
                result.push_back(temp+8);
            }
        }
    }
}

void Game::get_rook_moves(int pos, std::vector<int>& result) {
    //adds all rook moves to result
    bool w_turn;
    if (board.data[pos] > 0) {
        w_turn = true;
    } else {
        w_turn = false;
    }
    //top col
    int temp = pos - 8;
    while (temp >= 0) {
        if (board.data[temp] == 0) {
            result.push_back(temp);
        } else if ((w_turn && board.data[temp] < 0) || (!w_turn && board.data[temp] > 0)) {
            result.push_back(temp);
            break;
        } else {
            break;
        }
        temp -= 8;
    }
    //bottom col
    temp = pos + 8;
    while (temp <= 63) {
        if (board.data[temp] == 0) {
            result.push_back(temp);
        } else if ((w_turn && board.data[temp] < 0) || (!w_turn && board.data[temp] > 0)) {
            result.push_back(temp);
            break;
        } else {
            break;
        }
        temp += 8;
    }
    //right row
    if ((pos - 7) % 8 != 0) {
        temp = pos + 1;
        while (temp % 8 != 0) {
            if (board.data[temp] == 0) {
                result.push_back(temp);
            } else if ((w_turn && board.data[temp] < 0) || (!w_turn && board.data[temp] > 0)) {
                result.push_back(temp);
                break;
            } else {
                break;
            }
            temp += 1;
        }
    }
    //left row
    if (pos % 8 != 0) {
        temp = pos - 1;
        while ((temp-7) % 8 != 0) {
            if (board.data[temp] == 0) {
                result.push_back(temp);
            } else if ((w_turn && board.data[temp] < 0) || (!w_turn && board.data[temp] > 0)) {
                result.push_back(temp);
                break;
            } else {
                break;
            }
            temp -= 1;
        }
    }
}

void Game::get_queen_moves(int pos, std::vector<int>& result) {
    //adds all queen moves to result
    get_bishop_moves(pos, result);
    get_rook_moves(pos, result);
}

void Game::get_king_moves(int pos, std::vector<int>& result) {
    //adds all king moves to result
    bool w_turn;
    if (board.data[pos] > 0) {
        w_turn = true;
    } else {
        w_turn = false;
    }

    //castling
    int king;
    std::array<bool, 4> castle;
    if (w_turn) {
        king = 60;
        castle = board.w_castle;
    } else {
        king = 4;
        castle = board.b_castle;
    }
    if (castle[0] && castle[1]) { //right
        result.push_back(king + 2);
    }
    if (castle[2] && castle[3]) { //left
        result.push_back(king - 2);
    }

    //normal moves
    int temp;
    if (pos % 8 == 0) { //on left
        temp = pos - 8;
        if (temp >= 0) {
            if ((w_turn && board.data[temp] <= 0) || (!w_turn && board.data[temp] >= 0)) {
                result.push_back(temp);
            }
            if ((w_turn && board.data[temp+1] <= 0) || (!w_turn && board.data[temp+1] >= 0)) {
                result.push_back(temp+1);
            }
        }
        temp = pos + 8;
        if (temp <= 63) {
            if ((w_turn && board.data[temp] <= 0) || (!w_turn && board.data[temp] >= 0)) {
                result.push_back(temp);
            }
            if ((w_turn && board.data[temp+1] <= 0) || (!w_turn && board.data[temp+1] >= 0)) {
                result.push_back(temp+1);
            }
        }
        if ((w_turn && board.data[pos+1] <= 0) || (!w_turn && board.data[pos+1] >= 0)) {
            result.push_back(pos+1);
        }
    } else if ((pos-7) % 8 == 0) { //on right
        temp = pos - 8;
        if (temp >= 0) {
            if ((w_turn && board.data[temp] <= 0) || (!w_turn && board.data[temp] >= 0)) {
                result.push_back(temp);
            }
            if ((w_turn && board.data[temp-1] <= 0) || (!w_turn && board.data[temp-1] >= 0)) {
                result.push_back(temp-1);
            }
        }
        temp = pos + 8;
        if (temp <= 63) {
            if ((w_turn && board.data[temp] <= 0) || (!w_turn && board.data[temp] >= 0)) {
                result.push_back(temp);
            }
            if ((w_turn && board.data[temp-1] <= 0) || (!w_turn && board.data[temp-1] >= 0)) {
                result.push_back(temp-1);
            }
        }
        if ((w_turn && board.data[pos-1] <= 0) || (!w_turn && board.data[pos-1] >= 0)) {
            result.push_back(pos-1);
        }
    } else { //in center of board
        temp = pos - 8;
        if (temp >= 0) {
            if ((w_turn && board.data[temp] <= 0) || (!w_turn && board.data[temp] >= 0)) {
                result.push_back(temp);
            }
            if ((w_turn && board.data[temp+1] <= 0) || (!w_turn && board.data[temp+1] >= 0)) {
                result.push_back(temp+1);
            }
            if ((w_turn && board.data[temp-1] <= 0) || (!w_turn && board.data[temp-1] >= 0)) {
                result.push_back(temp-1);
            }
        }
        temp = pos + 8;
        if (temp <= 63) {
            if ((w_turn && board.data[temp] <= 0) || (!w_turn && board.data[temp] >= 0)) {
                result.push_back(temp);
            }
            if ((w_turn && board.data[temp+1] <= 0) || (!w_turn && board.data[temp+1] >= 0)) {
                result.push_back(temp+1);
            }
            if ((w_turn && board.data[temp-1] <= 0) || (!w_turn && board.data[temp-1] >= 0)) {
                result.push_back(temp-1);
            }
        }
        if ((w_turn && board.data[pos+1] <= 0) || (!w_turn && board.data[pos+1] >= 0)) {
            result.push_back(pos+1);
        }
        if ((w_turn && board.data[pos-1] <= 0) || (!w_turn && board.data[pos-1] >= 0)) {
            result.push_back(pos-1);
        }
    }
}

void Game::get_trajectory(int pos, std::vector<int>& result) {
    //adds all possible trajectories (legal or not) of a piece at pos to result

    int piece = board.data[pos];

    if (piece == 1 || piece == -1) {
        get_pawn_moves(pos, result);
    } else if (piece == 2 || piece == -2) {
        get_bishop_moves(pos, result);
    } else if (piece == 3 || piece == -3) {
        get_knight_moves(pos, result);
    } else if (piece == 4 || piece == -4) {
        get_rook_moves(pos, result);
    } else if (piece == 5 || piece == -5) {
        get_queen_moves(pos, result);
    } else if (piece == 6 || piece == -6) {
        get_king_moves(pos, result);
    }
}

std::vector<int> Game::get_all_trajectories(bool w_turn) {
    //returns all trajectories of the players to move pieces in a vector
    std::vector<int> result;
    result.reserve(50); //to prevent copying

    for (int i=0; i < 64; i++) {
        if ((w_turn && board.data[i] > 0) || (!w_turn && board.data[i] < 0)) { //determines black or white side
            get_trajectory(i, result); //get all piece
        }
    }

    return result;
}

void Game::get_legal_moves(int pos, std::vector<int>& result) {
    //appends all legal moves at pos in board to result
    std::vector<int> all_moves;
    all_moves.reserve(10);
    get_trajectory(pos, all_moves);

    for (int i=0; i < (int)all_moves.size(); i++) {
        Board old_board = update_board(pos, all_moves[i]);
        if ((!board.w_turn && !board.w_check) || (board.w_turn && !board.b_check)) { //update swaps turn so must swap back
            result.push_back(all_moves[i]);
        }
        undo_update_board(old_board);
    }
}


std::vector<std::vector<int>> Game::get_all_legal_moves() {
    //returns an array of arrays, with the first element being the position of piece and the rest its available moves
    //will return all legal moves of whoevers turn it is

    std::vector<std::vector<int>> result;
    result.reserve(10); //num of max pieces

    for (int i=0; i < 64; i++) {
        if ((board.w_turn && board.data[i] > 0) || (!board.w_turn && board.data[i] < 0)) { //determines black or white side
            std::vector<int> legal_moves;
            legal_moves.reserve(15);
            legal_moves.push_back(i); //add original pos
            get_legal_moves(i, legal_moves); //get all legal
            if (legal_moves.size() != 1) { //if not just original position
                result.push_back(legal_moves);
            }
        }
    }
    return result;
}

void Game::check_castle(bool w_turn, const std::vector<int>& enemy_moves) {
    //updates castle array for turn defiend in paramter, given a array of the enemies available moves
    std::array<bool, 4>* castle;
    if (w_turn) {
        castle = &board.w_castle;
    } else {
        castle = &board.b_castle;
    }
    //white
    //perms
    int king; //pos of king
    int side;
    if (w_turn) {
        side = 1;
        king = 60;
    } else {
        side = -1;
        king = 4;
    }
    if (board.data[king] != side * 6) { //king moved, all perm become false, dont undo these
        (*castle)[1] = false;
        (*castle)[3] = false;
    }
    if (board.data[king+3] != side * 4) { //right rook
        (*castle)[1] = false;
    }
    if (board.data[king-4] != side * 4) { //left rook
        (*castle)[3] = false;
    }

    //temps
    if ((*castle)[1] || (*castle)[3]) { //one of perms has to be true to bother with temps
        (*castle)[0] = true;
        (*castle)[2] = true;

        if (board.data[king+1] != 0 || board.data[king+2] != 0) { //piece in the way
            (*castle)[0] = false;
        }
        if (board.data[king-3] != 0 || board.data[king-2] != 0 || board.data[king-1] != 0) {
            (*castle)[2] = false;
        }

        //since checking if empty square is under attack must consider pawns moving diag (wont show in enemy_moves())
        //all spots except leftmost will stop castling
        if ((*castle)[0] || (*castle)[2]) {
            if (w_turn) {
                if (board.data[52] == -1) {
                    (*castle)[0] = false;
                    (*castle)[2] = false;
                } else {
                    if (board.data[53] == -1 || board.data[54] == -1 || board.data[55] == -1) {
                        (*castle)[0] = false;
                    }
                    if (board.data[49] == -1 || board.data[50] == -1 || board.data[51] == -1) {
                        (*castle)[2] = false;
                    }
                }
            } else {
                if (board.data[12] == 1) {
                    (*castle)[0] = false;
                    (*castle)[2] = false;
                } else {
                    if (board.data[13] == 1 || board.data[14] == 1 || board.data[15] == 1) {
                        (*castle)[0] = false;
                    }
                    if (board.data[9] == 1 || board.data[10] == 1 || board.data[11] == 1) {
                        (*castle)[2] = false;
                    }
                }
            }

            if ((*castle)[0] || (*castle)[2]) { //if both are not already both false
                for (int i=0; i < (int)enemy_moves.size(); i++) { //dont use under_attack to prevent looping multiple times
                    int e_pos = enemy_moves[i]%100;
                    if (e_pos == king) {
                        (*castle)[0] = false;
                        (*castle)[2] = false;
                    } else if (e_pos == king+1 || e_pos == king+2) {
                        (*castle)[0] = false;
                    } else if (e_pos == king-2 || e_pos == king-1) {
                        (*castle)[2] = false;
                    }
                }
            }
        }
    }
}

void Game::check_en_passant(int start_pos, int end_pos) {
    //checks for a pawn developing an en passant square.

    if (end_pos >= 100) { //promoting pawn so cant be en passant
        board.en_passant = -1;
        return;
    }
    int diff;
    if (board.data[end_pos] == 1) {
        //white pawn
        diff = end_pos - start_pos;
        if (diff == -16) {
            board.en_passant = start_pos - 8;
            return;
        }
    } else if (board.data[end_pos] == -1) {
        diff = end_pos - start_pos;
        if (diff == 16) {
            board.en_passant = start_pos + 8;
            return;
        }
    }
    board.en_passant = -1;
}