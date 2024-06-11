#include <iostream>
#include <unordered_map>
#include <tuple>
#include <vector>
#include "raylib.h"
#include "board.h"

#pragma once

struct BoardHasher {
    //hash function for board
    std::size_t operator()(const Board& b) const {
        std::size_t hash = 0;
        //board
        for (int i=0; i < 64; i++) {
            hash ^= std::hash<int>()(b.data[i]);
        }
        //turn
        hash ^= std::hash<bool>()(b.w_turn);
        //castles
        for (int i=0; i < 4; i++) {
            hash ^= std::hash<bool>()(b.w_castle[i]);
            hash ^= std::hash<bool>()(b.b_castle[i]);
        }
        //en passant
        hash ^= std::hash<int>()(b.en_passant);
        return hash;
    }
};

class Game {
    public:

        Board board;
        int game_over = 0;
        int is_promoting = -1;
        bool player_turn;

        std::unordered_map<int, Texture2D> skins; //skin textures maps num to Texture2D
        Texture2D board_texture;
        Texture2D select_texture;

        //constructor
        Game(bool textures=true);
        //drawing
        void draw_game();
        void promotion_menu();
        //functionality
        void select_move(int pos);
        int get_index(int x, int y);
        Board update_board(int start_pos, int end_pos);
        void undo_update_board(Board old_board);
        void apply_promotion(int pos);
        //movement
        std::vector<std::vector<int>> get_all_legal_moves();
        //engine
        void engine_move();

    private:
        const int SCREEN_WIDTH = 784;
        const int SCREEN_HEIGHT = 784;
        const int FONT_SIZE = 100;
        const int SQUARE_WIDTH = 96;
        const int BEVEL = 8;
        const double SCALE = 0.75;
        
        std::unordered_map<Board, int, BoardHasher> history; //prev boards
        std::vector<int> promotion_positions;
        std::vector<int> moves; //storage for current possible moves of selected piece
        bool select = false;
        int select_pos = -1;
        std::unordered_map<int, std::tuple<int, int>> coord; //mapping of indices to x-y coord in window
        //drawing
        void draw_select(int pos, Color color);
        //functionality
        void set_promotion_pos();
        void check_for_selection(int pos);
        void swap_turn();
        bool under_attack(int pos, std::vector<int> enemy_moves);
        int get_piece_pos(int piece);
        void check_game_over();
        void check_draw();
        bool insuf_material();
        std::unordered_map<int, int> get_material();
        bool hash_board();
        void undo_hash_board();
        //movement
        void move_piece(int start_pos, int end_pos);
        void check_castle(std::vector<int> enemy_moves);
        void check_en_passant(int start_pos, int end_pos);
        std::vector<int> get_pawn_moves(int pos);
        std::vector<int> get_bishop_moves(int pos);
        std::vector<int> get_knight_moves(int pos);
        std::vector<int> get_rook_moves(int pos);
        std::vector<int> get_queen_moves(int pos);
        std::vector<int> get_king_moves(int pos);
        std::vector<int> get_trajectory(int pos);
        std::vector<int> get_all_trajectories();
        std::vector<int> get_all_trajectories(bool w_turn);
        std::vector<int> get_legal_moves(int pos);
        //engine
        std::array<int, 2> minimax();
};