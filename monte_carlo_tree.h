#ifndef _MONTE_CARLO_TREE_h_
#define _MONTE_CARLO_TREE_h_
#include <cstddef>
#include <cmath>
#include "board.h"

extern double ucb_constant;

class Node{
public:

    Node* parent = NULL;
    Node* child[BOARD_SIZE];
    int num_child = 0;

    int simulated = 0;

    int player;   // who decide next move at this node
    int turn;
    int is_opponent;
    
    double wins = 0;
    double playouts = 0;

    double win_rate = 0;
    Node* max_child = NULL;
    
    double ucb = 0;
    Node* ucb_max_child = 0;
    // the point that this child do.
    int move = 0;

    // construct
    Node(Node* _parent, int _player, int _turn, int _move, double _win_rate);
    void addChild(int move);
};

Node* selection(Node* root, Board* board);
Node* expansion(Node* root, Board* board);
void simulation(Node* root, Board* board, int player, int _times);
void propagation(Node* root);

#endif
