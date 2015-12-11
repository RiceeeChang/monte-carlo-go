#include "monte_carlo.h"

/*
  MCS_pure
*/
int genmove(Board* board, int turn, int time_limit){
    clock_t start_t, end_t, now_t;
    // record start time
    start_t = clock();
    // calculate the time bound
    end_t = start_t + CLOCKS_PER_SEC * time_limit;

    int MoveList[BOARD_SIZE];
    int num_legal_moves = 0;
    int return_move = 0;

    num_legal_moves = gen_legal_move(board, turn, MoveList);
    
    return_move = pick_move(board, turn, time_limit, num_legal_moves, MoveList);
    if(return_move != 0)
        do_move(board, turn, return_move);

    now_t = clock();

    return return_move;
}

int pick_move(Board* board, int player, int time_limit, 
              int num_legal_moves, int MoveList[BOARD_SIZE]) {

    int win_rate[num_legal_moves] = {0};

    for(int times = 0; times<100; times++){
        for(int i = 0; i < num_legal_moves; i++){            
            if(quick_move(board, player, MoveList[i]))
                win_rate[i]++;
        }
    }
    int max = -MAX, target_move = 0;

    for(int i = 0; i<num_legal_moves; i++){
        //printf("win rate: %d\n", win_rate[i]); 
        if(win_rate[i] > max){
            max = win_rate[i];
            target_move = MoveList[i];
        }
    }
    return target_move;
}



int mcts_basic(Board* board, int player, int time_limit){
    clock_t start_t, end_t, now_t;
    // record start time
    start_t = clock();
    // calculate the time bound
    end_t = start_t + CLOCKS_PER_SEC * time_limit;

    Node _root(NULL, player, 0, 0);
    Node* root = &_root;
    int count=0;
    while(end_t > clock()){
        Board _temp_board;
        Board* temp_board = &_temp_board;
        board->copy_board(temp_board);

        Node* max_leaf = selection(root, temp_board);
        expansion(max_leaf, temp_board);
        simulation(max_leaf, temp_board, player, 20);
        propagation(max_leaf);
        count++;
    }
    printf("return count=%d\n", count);
    printf("max_child = %d\n", root->child[root->max_child]->move);
    return root->child[root->max_child]->move;
}
