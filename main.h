#ifndef _MAIN_H_
#define _MAIN_H_
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>

#define MAX 32100

#define BOARDSIZE        9
#define BOUNDARYSIZE    11
#define COMMANDLENGTH 1000
#define DEFAULTTIME     10
#define DEFAULTKOMI      7

#define MAXGAMELENGTH 1000
#define MAXSTRING       50
#define MAXDIRECTION     4

#define NUMINTERSECTION 81
#define HISTORYLENGTH   200

#define EMPTY            0
#define BLACK            1
#define WHITE            2
#define BOUNDARY         3

#define SELF             1
#define OPPONENT         2

#define NUMGTPCOMMANDS  15

#define LOCALVERSION     1
#define GTPVERSION       2

# include "monte_carlo.h"

using namespace std;

int genmove(int Board[BOUNDARYSIZE][BOUNDARYSIZE], 
            int turn, int time_limit, int game_length, 
            int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE]);
int pick_move(int Board[BOUNDARYSIZE][BOUNDARYSIZE], 
              int turn, int game_length, int time_limit, 
              int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE],
              int num_legal_moves, int MoveList[HISTORYLENGTH]);
int gen_legal_move(int Board[BOUNDARYSIZE][BOUNDARYSIZE], 
                   int turn, int game_length, 
                   int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE], 
                   int MoveList[HISTORYLENGTH]);

void do_move(int Board[BOUNDARYSIZE][BOUNDARYSIZE], int turn, int move);
void record(int Board[BOUNDARYSIZE][BOUNDARYSIZE], 
            int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE], 
            int game_length);
void update_board(int Board[BOUNDARYSIZE][BOUNDARYSIZE], int X, int Y, int turn);
double final_score(int Board[BOUNDARYSIZE][BOUNDARYSIZE]);


void gtp_final_score(int Board[BOUNDARYSIZE][BOUNDARYSIZE]);
void gtp_undo(int Board[BOUNDARYSIZE][BOUNDARYSIZE], 
              int game_length, 
              int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE]);
void gtp_showboard(int Board[BOUNDARYSIZE][BOUNDARYSIZE]);
void gtp_protocol_version();
void gtp_name();
void gtp_version();
void gtp_list_commands();
void gtp_known_command(const char Input[]);
void gtp_boardsize(int size);
void gtp_clear_board(int Board[BOUNDARYSIZE][BOUNDARYSIZE], int NumCapture[]);
void gtp_komi(double komi);
void gtp_play(char Color[], char Move[], int Board[BOUNDARYSIZE][BOUNDARYSIZE], int game_length, int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE]);
void gtp_genmove(int Board[BOUNDARYSIZE][BOUNDARYSIZE], char Color[], int time_limit, int game_length, int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE]);
void gtp_main(int display);

void count_neighboorhood_state(int Board[BOUNDARYSIZE][BOUNDARYSIZE], int X, int Y, int turn, int* empt, int* self, int* oppo ,int* boun, int NeighboorhoodState[MAXDIRECTION]);
int find_liberty(int X, int Y, int label, int Board[BOUNDARYSIZE][BOUNDARYSIZE], int ConnectBoard[BOUNDARYSIZE][BOUNDARYSIZE]);
void count_liberty(int X, int Y, int Board[BOUNDARYSIZE][BOUNDARYSIZE], int Liberties[MAXDIRECTION]);

#endif
