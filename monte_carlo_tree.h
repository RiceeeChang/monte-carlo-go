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
    int max_child = -1;

    int simulated = 0;

    int player;   // who decide next move at this node
    int opponent;
    int is_opponent;
    
    double wins = 0;
    double playouts = 0;
    double win_rate = 0;
    double ucb = 0;
    int ucb_max_child = 0;
    // the point that this child do.
    int move = 0;

    // construct
    Node(Node* _parent, int _player, int _is_opponent, int _move);
    void addChild(int move);
    void update();
};

Node* selection(Node* root, Board* board);
void expansion(Node* root, Board* board);
void simulation(Node* root, Board* board, int player, int _times);
void propagation(Node* root);

#endif
