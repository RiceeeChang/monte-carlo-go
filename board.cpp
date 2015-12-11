#include "board.h"
#include <stdio.h>

double _komi = DEFAULT_KOMI;

void get_neighbors(int point, int neighbors[4]){
    static const int _NORTH_VECTOR = -(SIZE+1);
    static const int _WEST_VECTOR  = -1;
    static const int _EAST_VECTOR  = 1;
    static const int _SOUTH_VECTOR = SIZE+1;


    neighbors[0] = point + _NORTH_VECTOR;
    neighbors[1] = point + _WEST_VECTOR;
    neighbors[2] = point + _EAST_VECTOR;
    neighbors[3] = point + _SOUTH_VECTOR;
}

int get_opponent(int player){
    return !player;
}

Board::Board(){
    for(int i = 0; i<BOARD_SIZE; i++){
        if(i/(SIZE+1) == 0 || i%(SIZE+1) == 0 || i>(SIZE+1)*(SIZE+1))
            board[i] = BOUNDARY;
        else
            board[i] = EMPTY;   
    }
}

int Board::num_liberties(int point){
    int liberties=0;
    current_liberty_mark++;
    //int current_liberty_mark = 1;
    //int liberty_marks[BOARD_SIZE] = {0};
    int q = point;
    do{
        int neighbors[4];
        get_neighbors(q, neighbors);
        for(int i=0; i<4; i++){
            if(board[neighbors[i]] == EMPTY && 
               liberty_marks[neighbors[i]] != current_liberty_mark){
                liberties++;
                liberty_marks[neighbors[i]] = current_liberty_mark;
            }
        }
        if(liberties >= 2)
            return liberties;
        q = next_point_in_chain[q];
    }while(q != point);
    return liberties;
}

void Board::add_point(int player, int point){
    stones[player]++;
    board[point] = player;
    next_point_in_chain[point] = point;
}

void Board::connect_chain(int point_a, int point_b){
    int temp = next_point_in_chain[point_b];
    next_point_in_chain[point_b] = next_point_in_chain[point_a];
    next_point_in_chain[point_a] = temp;
}

void Board::remove_chain(int point){
    int captured = 0;
    int color = board[point];
    int q = point;
    do{
        board[q] = EMPTY;
        stones[color]--;
        prisoners[color]++;
        captured++;

        int next = next_point_in_chain[q];
        next_point_in_chain[q] = 0;
        q = next;
    }while(q != point);

    if(captured == 1)
        ko_point = point;
    else
        ko_point = 0;
}

void Board::copy_board(Board* another){
    another->game_length = game_length;
    another->ko_point = ko_point;
    another->prisoners[BLACK] = prisoners[BLACK];
    another->prisoners[WHITE] = prisoners[WHITE];
    another->stones[BLACK] = stones[BLACK];
    another->stones[WHITE] = stones[WHITE];

    for(int i=0; i<BOARD_SIZE; i++){
        another->board[i] = board[i];
        another->next_point_in_chain[i] = next_point_in_chain[i];
    }
}

void Board::show(){
    for(int i=0; i<BOARD_SIZE; i++){
        switch(board[i]){
        case BOUNDARY:
            printf("#");
            break;
        case EMPTY:
            printf(".");
            break;
        case BLACK:
            printf("X");
            break;
        case WHITE:
            printf("O");
            break;
        }
        if( i%(SIZE+1) == SIZE)
            printf("\n");
    }
    printf("\n");
}

void Board::reset(){
    for(int i = 0; i<BOARD_SIZE; i++){
        if(i/(SIZE+1) == 0 || i%(SIZE+1) == 0 || i>(SIZE+1)*(SIZE+1))
            board[i] = BOUNDARY;
        else
            board[i] = EMPTY;   
    }

    game_length = 0;
    ko_point = 0;
    prisoners[2] = {0};
    stones[2] = {0};

    next_point_in_chain[BOARD_SIZE] = {0};
    current_liberty_mark = 0;
    liberty_marks[BOARD_SIZE] = {0};
}

/*
 * This function return the number of legal moves with clor "turn" and
 * saves all legal moves in MoveList
 * */
int gen_legal_move(Board* board, int player, int MoveList[BOARD_SIZE]) {
    int legal_moves = 0;
    for(int point=0; point<BOARD_SIZE; point++){
        // condition 1: check the intersection is empty or not.
        if(board->board[point] != EMPTY){
            //printf("condition1 fail\n");
            continue;
        }
        // condition 2: check the intersection is KO or not.
        if(point == board->ko_point){
            //printf("condtion2 fail\n");
            continue;
        }
        
        int neighbors[4];
        get_neighbors(point, neighbors);
        int N = board->board[neighbors[NORTH]];
        int W = board->board[neighbors[WEST ]];
        int E = board->board[neighbors[EAST ]];
        int S = board->board[neighbors[SOUTH]];
        int opponent = get_opponent(player);

           // condition 3.1: check their is a neighbor is empty.
        if((N == EMPTY) || (W == EMPTY) || (E == EMPTY) || (S == EMPTY)   ||
           // condition 3.2: check their is a neighbor chain of self has more than 2 liberties.  
           (N == player && board->num_liberties(neighbors[NORTH]) >= 2)   || 
           (W == player && board->num_liberties(neighbors[WEST ]) >= 2)   ||
           (E == player && board->num_liberties(neighbors[EAST ]) >= 2)   ||
           (S == player && board->num_liberties(neighbors[SOUTH]) >= 2)   ||
           // condition 3.3: check can capture a enmey neighbor or not.
           (N == opponent && board->num_liberties(neighbors[NORTH]) == 1) ||
           (W == opponent && board->num_liberties(neighbors[WEST ]) == 1) ||
           (E == opponent && board->num_liberties(neighbors[EAST ]) == 1) ||
           (S == opponent && board->num_liberties(neighbors[SOUTH]) == 1) ){
            //printf("---choose= %d\n", point);
            MoveList[legal_moves++] = point;
        }//else
         //   printf("---condtion3 fail.\n");
    }
    return legal_moves;
}
/*
    do quick game with random at temp board
*/
int quick_move(Board* board, int player, int point){
    Board _temp_board;
    Board* temp_board = &_temp_board;
    board->copy_board(temp_board);


    do_move(temp_board, player, point);
    
    int temp_player = player;
    int MoveList[BOARD_SIZE];
    int num_legal_moves = 0;
    int move = 0, pass = 0;
    int game_length = 0;
    while(1){
    //for(int i=0; i<100; i++){
        temp_player = (temp_player+1)%2;
        game_length++;

        num_legal_moves = gen_legal_move(temp_board, temp_player, MoveList);

        if (num_legal_moves == 0){
            if(pass == 1)
                break;
            else
                pass = 1;
        }else {
            int move_id = rand()%num_legal_moves;
            move = MoveList[move_id];
            do_move(temp_board, temp_player, move);
        }
    }
    //printf("game_length = %d\n", game_length);
    int result = final_score(temp_board);
    if((player == BLACK && result > 0) || (player == WHITE && result < 0))
        return 1;
    else
        return 0;
}
/*
 * This function update the Board with put 'turn' at (x,y)
 * where x = (move % 100) / 10 and y = move % 10.
 * Note this function will not check 'move' is legal or not.
 * */
void do_move(Board* board, int player, int move) {
    
    int captured = 0;
    int neighbor[4];
    get_neighbors(move, neighbor);
    int opponent = get_opponent(player);
    board->add_point(player, move);
    if(move != 0){
        for(int i=0; i<4; i++){
            if(board->board[neighbor[i]] == EMPTY){
                continue;
            }else if(board->board[neighbor[i]] == player){
                board->connect_chain(move, neighbor[i]);
            }else if(board->board[neighbor[i]] == opponent &&
                     board->num_liberties(neighbor[i]) == 0){
                board->remove_chain(neighbor[i]);
            }else if(board->board[neighbor[i]] == BOUNDARY){
                continue;
            }
        }    
    }
}
/*
 * This function update Board with place turn's piece at (X,Y).
 * Note that this function will not check if (X, Y) is a legal move or not.
 * */
void update_board(Board* board, int turn, int point) {

}
/* 
 * This function records the current game baord with current
 * game length "game_length"
 * */
void record(Board* board, int game_record[MAX_HISTORY][BOARD_SIZE], int game_length) {
    for(int i=0; i< BOARD_SIZE; i++)
        game_record[game_length][i] = board->board[i];
}
/*
 * This function counts the number of points remains in the board by Black's view
 * */
int final_score(Board* board) {
    return board->stones[BLACK] - (board->stones[WHITE] + _komi);
}
/* 
 * Following are commands for Go Text Protocol (GTP)
 *
 * */