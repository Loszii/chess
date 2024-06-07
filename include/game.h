#include <iostream>
#include <unordered_map>
#include <tuple>
#include <vector>
#include "raylib.h"
#include "board.h"

#pragma once

//maybe make class with get_pawn_moves private methods and en passant n stuff
class Game {
    public:

        Board board;
        int game_over = 0;
        std::vector<int> moves; //storage for current possible moves of selected piece

        std::unordered_map<int, Texture2D> skins; //skin textures maps num to Texture2D
        Texture2D board_texture;
        Texture2D select_texture;

        bool select = false;
        int select_pos = -1;

        //constructor
        Game();
        //drawing
        void draw_game();
        //functionality
        void select_move(int pos);
        int get_index(int x, int y);
        //movement
        int move_piece(int start_pos, int end_pos, Board& board);
        void undo_move_piece(int start_pos, int end_pos, int captured_piece, Board& board);
        std::vector<std::vector<int>> get_all_legal_moves(Board& board);
        std::array<bool, 4> check_castle(bool w_turn, Board& board);
        int check_en_passant(int start_pos, int end_pos, Board& board);
        int promote_pawns(Board& board); //note: call undo_move_piece before reverting this value

    private:
        const int SCREEN_WIDTH = 784;
        const int SCREEN_HEIGHT = 784;
        const int FONT_SIZE = 100;
        const int SQUARE_WIDTH = 96;
        const int BEVEL = 8;
        const double SCALE = 0.75;
        
        std::unordered_map<int, std::tuple<int, int>> coord; //mapping of indices to x-y coord in window
        //drawing
        void draw_select(int pos, Color color);
        //functionality
        void check_for_selection(int pos);
        void update_board(int start_pos, int end_pos);
        void swap_turn();
        bool under_attack(int pos, std::vector<int> enemy_moves);
        int get_piece_pos(int piece);
        void check_game_over(Board& board);
        //movement
        std::vector<int> get_pawn_moves(int i, int j, Board& board);
        std::vector<int> get_bishop_moves(int i, int j, Board& board);
        std::vector<int> get_knight_moves(int i, int j, Board& board);
        std::vector<int> get_rook_moves(int i, int j, Board& board);
        std::vector<int> get_queen_moves(int i, int j, Board& board);
        std::vector<int> get_king_moves(int i, int j, Board& board);
        std::vector<int> get_trajectory(int pos, Board& board);
        std::vector<int> get_enemy_moves(bool w_turn, Board& board);
        std::vector<int> get_legal_moves(int pos, Board& board);
};