#ifndef _GTP_H_
#define _GTP_H_

#include "board.h"
#include "main.h"

#define COMMAND_LENGTH  1000
#define NUM_GTP_COMMANDS  15

#define LOCAL_VERSION     1
#define GTP_VERSION       2


void gtp_main(int display);
void gtp_genmove(Board* board, char Color[], int time_limit, int game_length, int game_record[MAX_HISTORY][BOARD_SIZE]);
void gtp_final_score(Board* board);
void gtp_undo(Board* board, int game_length, int game_record[MAX_HISTORY][BOARD_SIZE]);
void gtp_showboard(Board* board);
void gtp_protocol_version();
void gtp_name();
void gtp_version();
void gtp_list_commands();
void gtp_known_command(const char Input[]);
void gtp_boardsize(int size);
void gtp_clear_board(Board* board);
void gtp_komi(double komi);
void gtp_play(char Color[], char Move[], Board* board, int game_length, int game_record[MAX_HISTORY][BOARD_SIZE]);

#endif
