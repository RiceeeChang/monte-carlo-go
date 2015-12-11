#ifndef _MONTE_CARKLO_H_
#define _MONTE_CARKLO_H_
#include "main.h"
#include "board.h"
#include "monte_carlo_tree.h"

int genmove(Board* board, int turn, int time_limit);
int pick_move(Board* board, int turn, int time_limit, 
              int num_legal_moves, int MoveList[BOARD_SIZE]);
int mcts_basic(Board* board, int player, int time_limit);
#endif
