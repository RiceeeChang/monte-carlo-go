#include "monte_carlo_tree.h"
#include <cstdio> 
double ucb_constant = 0.1;

Node::Node(Node* _parent, int _player, int _turn, int _move, double _win_rate){
    parent = _parent;
    player = _player;
    turn = _turn;
    is_opponent = (player != turn);
    move = _move;
    win_rate = _win_rate;
}


Node* selection(Node* node, Board* board){
    while(node->num_child != 0){
        do_move(board, node->turn, node->move);
        node = node->max_child;
    }
    return node;
}

Node* expansion(Node* node, Board* board){
    if(!node->simulated)
        return node;

    int MoveList[BOARD_SIZE];
    int num_legal_moves = gen_legal_move(board, node->player, MoveList);
    double default_win_rate = 1.1;
    for(int i=0; i<num_legal_moves; i++)
        node->child[node->num_child++] = new Node(node, node->player, 
                                            !node->turn, MoveList[i], 
                                            default_win_rate);
    node->max_child = node->child[0];
    return node->max_child;

}

void simulation(Node* node, Board* board, int player, int _times){
    node->simulated = 1;

    for(int times=0; times<_times; times++){
        if(quick_move(board, node->player, node->move)
           ^ node->player==node->turn){
            node->wins++;
        }
        (node->playouts)++;
    }
}

void propagation(Node* node){
    
    Node* parent = node->parent;
    Node* child = node;
    while(parent != NULL){
        //node->update();
        parent->wins     += node->wins;
        parent->playouts += node->playouts;
        parent->win_rate = parent->wins/parent->playouts;

        // count win_rate for mcst_pure
        child->win_rate = child->wins/child->playouts;
        if(child == parent->max_child){
            double max = -MAX;
            int count = 0;
            for(int i=0; i<parent->num_child; i++){
                if(parent->child[i]->win_rate > 1){
                    parent->max_child = parent->child[i];
                    break;
                }
                if(parent->child[i]->win_rate > max){
                    max = parent->child[i]->win_rate;
                    parent->max_child = parent->child[i];
                }
            }
        }else{
            if(child->win_rate >parent->max_child->win_rate)
                parent->max_child = child;
        }
        // count ucb
        double ucb_second = ucb_constant*sqrt(log(parent->playouts)/child->playouts);
        if(child->player == child->turn)
            child->ucb = child->win_rate + ucb_second;
        else
            child->ucb = (1-child->win_rate) + ucb_second;
        if(child == parent->ucb_max_child){
            int max = -MAX;
            for(int i=0; i<parent->num_child; i++){
                if(parent->child[i]->ucb = MAX){
                    parent->ucb_max_child = parent->child[i];
                    break;
                }
                if(parent->child[i]->ucb > -MAX){
                    max = parent->child[i]->ucb;
                    parent->ucb_max_child = parent->child[i];
                }
            }
        }

        child = parent;
        parent = parent->parent;
    }
}
