#include <iostream>
#include <vector>
#include <random>
#include "engine.h"
#include "movement.h"

//TODO:
/*improve eval function, add alpha beta pruning, simplify code and use the main.cpp update function for updating game state*/


const int scale[8][8] = {
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 2, 2, 2, 2, 2, 2, 1},
    {1, 2, 3, 3, 3, 3, 2, 1},
    {1, 2, 3, 4, 4, 3, 2, 1},
    {1, 2, 3, 4, 4, 3, 2, 1},
    {1, 2, 3, 3, 3, 3, 2, 1},
    {1, 2, 2, 2, 2, 2, 2, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
};

int eval_board(int board[8][8]) {
    //returns a number evaluating the board, lower better for black, bigger better for white
    int score = 0;
    for (int i=0;i < 8; i++) {
        for (int j=0; j < 8; j++) {
            score += board[i][j]; //* scale[i][j] for now just add all pieces
        }
    }
    return score;
}

int minimax_helper(int board[8][8], bool w_castle[4], bool b_castle[4], bool w_turn, int en_passant, int depth) {
    if (depth == 0) {
        return eval_board(board);
    }
    int best;
    int captured;
    std::vector<std::vector<int>> all_moves = get_all_legal_moves(board, w_castle, b_castle, w_turn, en_passant);
    if (w_turn) { //return max move
        best = -(int)INFINITY;
        for (int i=0; i < (int)all_moves.size(); i++) {
            for (int j=1; j < (int)all_moves[i].size(); j++) {
                captured = move_piece(all_moves[i][0], all_moves[i][j], board);

                //update castle/en_passant/promotion 
                int promotion_pos = check_pawn_promotion(board);
                en_passant = check_en_passant(board, all_moves[i][0], all_moves[i][j]);
                bool b_castle_copy[4];
                for (int k=0; k < 4; k++) {
                    b_castle_copy[k] = b_castle[k];
                }
                check_castle_conditions(board, b_castle_copy, false); //white moved so check black castle conditions

                best = std::max(best, minimax_helper(board, w_castle, b_castle_copy, !w_turn, en_passant, depth-1));
                undo_move(all_moves[i][0], all_moves[i][j], board, captured, promotion_pos);
            }
        }
        return best;
    } else {
        best = (int)INFINITY;
        for (int i=0; i < (int)all_moves.size(); i++) {
            for (int j=1; j < (int)all_moves[i].size(); j++) {
                captured = move_piece(all_moves[i][0], all_moves[i][j], board);

                //update castle/en_passant/promotion TODO
                int promotion_pos = check_pawn_promotion(board);
                en_passant = check_en_passant(board, all_moves[i][0], all_moves[i][j]);
                bool w_castle_copy[4];
                for (int k=0; k < 4; k++) {
                    w_castle_copy[k] = w_castle[k];
                }
                check_castle_conditions(board, w_castle_copy, true); //black moved so check white castle

                best = std::min(best, minimax_helper(board, w_castle_copy, b_castle, !w_turn, en_passant, depth-1));
                undo_move(all_moves[i][0], all_moves[i][j], board, captured, promotion_pos);
            }
        }
        return best;
    }
}

std::vector<int> minimax(int board[8][8], bool w_castle[4], bool b_castle[4], bool w_turn, int en_passant, int depth) {
    int best;
    int captured;
    std::vector<std::vector<int>> all_moves = get_all_legal_moves(board, w_castle, b_castle, w_turn, en_passant);
    std::vector<int> result;
    if (w_turn) { //return max move
        best = -(int)INFINITY;
        for (int i=0; i < (int)all_moves.size(); i++) {
            for (int j=1; j < (int)all_moves[i].size(); j++) {
                captured = move_piece(all_moves[i][0], all_moves[i][j], board);

                //update castle/en_passant/promotion 
                int promotion_pos = check_pawn_promotion(board);
                en_passant = check_en_passant(board, all_moves[i][0], all_moves[i][j]);
                bool b_castle_copy[4];
                for (int k=0; k < 4; k++) {
                    b_castle_copy[k] = b_castle[k];
                }
                check_castle_conditions(board, b_castle_copy, false);

                int score = minimax_helper(board, w_castle, b_castle_copy, !w_turn, en_passant, depth-1);
                best = std::max(best, score);
                undo_move(all_moves[i][0], all_moves[i][j], board, captured, promotion_pos);
                if (best == score) {
                    result.clear();
                    result.push_back(all_moves[i][0]);
                    result.push_back(all_moves[i][j]);
                }
            }
        }
    } else {
        best = (int)INFINITY;
        for (int i=0; i < (int)all_moves.size(); i++) {
            for (int j=1; j < (int)all_moves[i].size(); j++) {
                captured = move_piece(all_moves[i][0], all_moves[i][j], board);

                //update castle/en_passant/promotion
                int promotion_pos = check_pawn_promotion(board);
                en_passant = check_en_passant(board, all_moves[i][0], all_moves[i][j]);
                bool w_castle_copy[4];
                for (int k=0; k < 4; k++) {
                    w_castle_copy[k] = w_castle[k];
                }
                check_castle_conditions(board, w_castle_copy, true);

                int score = minimax_helper(board, w_castle_copy, b_castle, !w_turn, en_passant, depth-1);
                best = std::min(best, score);
                undo_move(all_moves[i][0], all_moves[i][j], board, captured, promotion_pos);
                if (best == score) {
                    result.clear();
                    result.push_back(all_moves[i][0]);
                    result.push_back(all_moves[i][j]);
                }
            }
        }
    }
    return result;
}

std::vector<int> make_best_move(int board[8][8], bool w_castle[4], bool b_castle[4], bool w_turn, int en_passant) {
    //returns an array of the start pos / end pos of best move, for now w_turn is always false since engine is always black
    std::vector<int> result = minimax(board, w_castle, b_castle, w_turn, en_passant, 3);
    return result;
}