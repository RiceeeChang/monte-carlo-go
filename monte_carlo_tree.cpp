#include "monte_carlo_tree.h"
#include <cstdio> 
double ucb_constant = 0.1;

Node::Node(Node* _parent, int _player, int _is_opponent, int _move){
    parent = _parent;
    player = _player;
    opponent = !player;
    is_opponent = _is_opponent;
    move = _move;
}

void Node::update(){
    double max = -1;
    max_child = 0;
    //printf("update\nchild_number= %d\n", num_child);
    // count win_rate
    for(int i=0; i<num_child; i++){
        

        wins += child[i]-> wins;
        playouts += child[i]-> playouts;
        child[i]->win_rate = child[i]->wins/child[i]->playouts;

        if((child[i]->win_rate > max)^(is_opponent)){
            max = child[i]->win_rate;
            max_child = i;
        }
        //printf("i=%d, child=%p, move=%d, wins= %f, playouts=%f\n", 
        //    i, child[i], child[i]->move, child[i]->wins, child[i]->playouts);
        //printf("win_rate=%f, max=%f, max_child=%d\n", child[i]->win_rate, max, max_child);
    }
    // count ucb
    max = -1;
    ucb_max_child = 0;
    for(int i=0; i<num_child; i++){
        if(is_opponent)
            child[i]->ucb = child[i]->win_rate + 
            ucb_constant*sqrt(log(playouts)/child[i]->playouts);
        else
            child[i]->ucb = (1-child[i]->win_rate) + 
            ucb_constant*sqrt(log(playouts)/child[i]->playouts);
        if((child[i]->ucb > max)^(is_opponent)){
            max = child[i]->ucb;
            ucb_max_child = i;
        }
    }
}

Node* selection(Node* root, Board* board){
    while(root->num_child != 0){
        do_move(board, root->opponent, root->move);
        root = root->child[root->max_child];
    }
    return root;
}

void expansion(Node* root, Board* board){
    int MoveList[BOARD_SIZE];
    int num_legal_moves = gen_legal_move(board, root->player, MoveList);

    int opponent = !(root->player);
    for(int i=0; i<num_legal_moves; i++){
        Node* child = new Node(root, opponent, !(root->is_opponent), MoveList[i]);
        root->child[root->num_child++] = child;
    }
}

void simulation(Node* root, Board* board, int player, int _times){
    for(int times=0; times<_times; times++){
        for(int i=0; i<root->num_child; i++){
            Node* child = root->child[i];
            if(quick_move(board, child->player, child->move)
               ^ root->is_opponent){
                child->wins++;
            }
            (child->playouts)++;
        }    
    }
}

void propagation(Node* root){
    while(root != NULL){
        root->update();
        root = root->parent;
    } 
}
