#ifndef _MONTE_CARKLO_H_
#define _MONTE_CARKLO_H_
# include "main.h"

int pick_move(int Board[BOUNDARYSIZE][BOUNDARYSIZE], 
              int turn, int game_length, int time_limit, 
              int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE],
              int num_legal_moves, int MoveList[HISTORYLENGTH]);
int quick_move(int Board[BOUNDARYSIZE][BOUNDARYSIZE], 
            int turn, int time_limit, int game_length, 
            int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE]);

#endif
