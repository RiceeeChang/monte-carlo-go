#include "monte_carlo.h"
#include "main.h"
/*
  Monte Carlo Search pure
    turn = BLACK or WHITE
*/

/*


MCTS 次數 總次數要夠多才有希望
要在重點的地方分配 1k~3k

*/
/*
 * MCS
 * */
int pick_move(int Board[BOUNDARYSIZE][BOUNDARYSIZE], 
              int turn, int game_length, int time_limit, 
              int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE],
              int num_legal_moves, int MoveList[HISTORYLENGTH]) {

    int win_rate[num_legal_moves] = {0};
    for(int times = 0; times<10; times++){
    for(int i = 0; i < num_legal_moves; i++){

        int temp_board[BOUNDARYSIZE][BOUNDARYSIZE] = {{0}};
        int temp_game_length = game_length;

        for(int x = 0; x<BOUNDARYSIZE; x++)
            for(int y = 0; y<BOUNDARYSIZE; y++)
                temp_board[x][y] = Board[x][y];

        do_move(temp_board, turn, MoveList[i]);
        temp_game_length++;
        record(temp_board, GameRecord, temp_game_length);

        int move = 0, pass = 0;
        while(temp_game_length < MAXGAMELENGTH){
            turn = (turn%2) + 1;
            move = quick_move(temp_board, turn, time_limit, temp_game_length, GameRecord);

            do_move(temp_board, turn, move);
            temp_game_length++;
            record(Board, GameRecord, temp_game_length);

            if(move == 0 && pass == 1)
                break;
            else if(move == 0)
                pass = 1;
            else
                pass = 0;
        }
        //cout << temp_game_length << endl;
        int result = final_score(temp_board);
        if((turn == BLACK && result > 0) || (turn == WHITE && result < 0))
            win_rate[i]++;
    }}
    cout << endl;
    int max = -MAX, target_move = 0;

    for(int i = 0; i<num_legal_moves; i++){
        // cout << "win rate: " << win_rate[i] << endl;
        if(win_rate[i] > max){
            
            max = win_rate[i];
            target_move = MoveList[i];
        }
    }
    return target_move;
}

int quick_move(int Board[BOUNDARYSIZE][BOUNDARYSIZE], 
            int turn, int time_limit, int game_length, 
            int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE]){
    clock_t start_t, end_t, now_t;
    // record start time
    start_t = clock();
    // calculate the time bound
    end_t = start_t + CLOCKS_PER_SEC * time_limit;

    int MoveList[HISTORYLENGTH];
    int num_legal_moves = 0;
    int return_move = 0;

    num_legal_moves = gen_legal_move(Board, turn, game_length, GameRecord, MoveList);

    if (num_legal_moves == 0)
       return 0;
    else {
       int move_id = rand()%num_legal_moves;
       return MoveList[move_id];
    }
}
