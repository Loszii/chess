#include <iostream>
#include <unordered_map>
#include <tuple>
#include <vector>
#include "raylib.h"
#include "board.h"

#pragma once

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
        void update_board(int start_pos, int end_pos);
        //movement
        int move_piece(int start_pos, int end_pos);
        void undo_move_piece(int start_pos, int end_pos, int captured_piece);
        std::vector<std::vector<int>> get_all_legal_moves();

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
        void swap_turn();
        bool under_attack(int pos, std::vector<int> enemy_moves);
        int get_piece_pos(int piece);
        void check_game_over();
        //movement
        std::array<bool, 4> check_castle();
        int check_en_passant(int start_pos, int end_pos);
        int promote_pawns();
        std::vector<int> get_pawn_moves(int i, int j);
        std::vector<int> get_bishop_moves(int i, int j);
        std::vector<int> get_knight_moves(int i, int j);
        std::vector<int> get_rook_moves(int i, int j);
        std::vector<int> get_queen_moves(int i, int j);
        std::vector<int> get_king_moves(int i, int j);
        std::vector<int> get_trajectory(int pos);
        std::vector<int> get_enemy_moves();
        std::vector<int> get_legal_moves(int pos);
};