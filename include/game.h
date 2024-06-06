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

    private:

        const int SQUARE_WIDTH = 96;
        const int BEVEL = 8;
        const double SCALE = 0.75;
        
        std::unordered_map<int, std::tuple<int, int>> coord; //mapping of indices to x-y coord in window
        //drawing
        void draw_select(int pos, Color color);
        //functionality
        void check_for_selection(int pos);
        void update_board();
        void swap_turn();
        bool under_attack(int pos, std::vector<int> enemy_moves);
        int get_piece_pos(int piece);
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