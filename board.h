#ifndef _BOARD_H_
#define _BOARD_H_
#include <cstdlib>

// Size parameters for a 9x9 board
#define SIZE          9
#define BOARD_SIZE    111 // (SIZE+2)*(SIZE+1)+1
#define DEFAULT_TIME     10
#define DEFAULT_KOMI      7
extern double _komi;



// Max number of previous moves to store
#define MAX_HISTORY   600

#define MAX_POINTS    81  // SIZE*SIZE;

// State of a board intersection
#define BLACK    0
#define WHITE    1
#define EMPTY    2
#define BOUNDARY 3

#define NORTH 0
#define WEST  1
#define EAST  2
#define SOUTH 3

void get_neighbors(int point, int neighbors[4]);
int get_opponent(int player);

class Board{
public:
    // Arrays for storing states
    int board[BOARD_SIZE];
    int game_length = 0;
    int ko_point = 0;
    // Number of stones captured from each player
    int prisoners[2] = {0};
    int stones[2] = {0};

    // Chain
    int next_point_in_chain[BOARD_SIZE] = {0};
    int current_liberty_mark = 0;
    int liberty_marks[BOARD_SIZE] = {0};

    Board();

    int num_liberties(int point);
    
    void add_point(int player, int point);
    void connect_chain(int point_a, int point_b);
    void remove_chain(int point);
    void copy_board(Board* another);

    void show();
    void reset();


    void record();
};

int gen_legal_move(Board* board, int turn, int MoveList[BOARD_SIZE]);
int quick_move(Board* board, int player, int point);
void do_move(Board* board, int turn, int point);
void record(Board* board, int game_record[MAX_HISTORY][BOARD_SIZE], int game_length);
void update_board(Board* board, int turn, int point);
int final_score(Board* board);

#endif
