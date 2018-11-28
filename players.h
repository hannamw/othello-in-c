/* Michael Hanna, mwhanna
 * CS 152, Spring 2017
 * Project 2
 */

#ifndef __PLAYERS_H
#define __PLAYERS_H

#include "logic.h"

struct edge_corner_weight {
    unsigned int edge_weight;
    unsigned int corner_weight;
};

union heuristic_config {
    unsigned int edge_weight;
    struct edge_corner_weight edge_corner_weight;
};

typedef union heuristic_config heuristic_config;

struct minimax_config {
    int (*heuristic)(game*, heuristic_config);
    heuristic_config hc;
    unsigned int ply;
};

typedef struct minimax_config minimax_config;

union strategy_config {
    minimax_config minimax_config;
};

typedef union strategy_config strategy_config;

/*returns the first valid move for a given turn*/
pos first_move(game* g, strategy_config unused);

/*prompts the user for a move, telling them if the move is invalid, and returns
the valid move*/
pos human(game* g, strategy_config unused);

/*returns a pos move, prioritizing corners over edges over interior pieces*/
pos immediate_tactics(game* g, strategy_config unused);

/*checks if the game is over, and returns the winner*/
int did_i_win(game* g, heuristic_config unused);

/*counts the number of pieces on the board, unweighted*/
int piece_counting(game* g, heuristic_config unused);

/*evaluates a game, weighting edges as specified by the heuristic_config*/
int prefer_edges(game* g, heuristic_config hc);

/*evaluates a game, weighting edges and corners as specified by the
heuristic_config*/
int prefer_edges_and_corners(game* g, heuristic_config hc);

/*given a game, looks ahead the specified number of plies (minimum 1 ply) and
returns the "optimal" pos*/
pos minimax(game* g, strategy_config sc);

/*makes a minimax_config given all of its fields*/
minimax_config make_mc(int (*heuristic)(game*, heuristic_config),
                        heuristic_config hc, unsigned ply);
                        
/*takes in the value to be incremented, # rows - 1, # cols - 1, edge weight,
corner weight, position row, position col, and increments the value*/
void add_weight(unsigned* n, unsigned nr1, unsigned nc1, int ew, int cw, 
                unsigned pr, unsigned pc);
                        
#endif