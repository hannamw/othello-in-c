/* Michael Hanna, mwhanna
 * CS 152, Spring 2017
 * Project 2
 */
  
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "players.h"

//Given the player type as its one-letter abbreviation
//return a player of that type
pos (*player_type(char type))(game*, strategy_config){
    switch(type){       
        case 'm':
            return minimax;
        
        case 'e':
            return immediate_tactics;
        
        case 'f':
            return first_move;
            
        default:
            return human;
    }
}

int main(int argc, char* argv[]) {
    //we only want to parse 4 arguments max here, so this is just done manually
    char black;
    char white;
    
    if(argc > 1){
        //one player type only specified
        if(argc == 3 && (!strcmp(argv[1],"-w") || !strcmp(argv[1], "-b"))){
            if(!strcmp(argv[1], "-w")){
                white = argv[2][0];
                black = 'h';
            } else {
                black = argv[2][0];
                white = 'h';
            }
        //two player types specified (-w and -b both set)
        } else if (argc == 5 && ((!strcmp(argv[1],"-w") && !strcmp(argv[3],"-b")) || (!strcmp(argv[1],"-b") && !strcmp(argv[3],"-w")))){
            if(!strcmp(argv[1], "-w")){
                white = argv[2][0];
                black = argv[4][0];
            } else {
                black = argv[2][0];
                white = argv[4][0];
            }
        //flags are set wrong
        } else {
            printf("You should run this program with either 0, 1, or 2 flags, and each flag should be accompanied by an argument.\n\n");
            printf("The -b and -w flags specify the plack and white player type. The corresponding arguments are h (human), m (minimax computer [hard]), ");
            printf("e (edge computer [easy]), and f (first-move computer [very easy]). See the readme for more info.\n\n");
            printf("Defaulting to two humans.\n\n");
            black = 'h';
            white = 'h';
        }
    //no flags set
    } else {
        black = 'h';
        white = 'h';
    }
    
	pos (*player_black)(game*, strategy_config) = player_type(black);
    pos (*player_white)(game*, strategy_config) = player_type(white);
    
    heuristic_config hce;
    hce.edge_weight = 2;
    
    heuristic_config hcecb;
    hcecb.edge_corner_weight.edge_weight = 2;
    hcecb.edge_corner_weight.corner_weight = 5;
    
    heuristic_config hcecw;
    hcecw.edge_corner_weight.edge_weight = 2;
    hcecw.edge_corner_weight.corner_weight = 10;
    
    strategy_config sc_black, sc_white;
    sc_black.minimax_config = make_mc(prefer_edges_and_corners, hcecb, 5);
    sc_white.minimax_config = make_mc(prefer_edges_and_corners, hcecw, 3);
    
    
    game* g = new_game(8, 8, CELLS);
    pos this_move;
    int go;
    char* v;
    
    while(!game_over(g)){
        game_show(g);
        if(!exists_move(g)){
            printf("No valid moves for %s; turn passed to %s\n", 
            (g->next)? "White": "Black", (!g->next)? "White": "Black");
            pass(g);
        }
        this_move = (g->next)? player_white(g, sc_white): 
                               player_black(g, sc_black);
        printf("Move applied at %d, %d:\n", this_move.r, this_move.c);
        apply_move(g, this_move);
    }
    game_show(g);
    go = game_outcome(g);
    v = (go < 2)? (go? "White wins": "Black wins"):"Draw";
    printf("Game Outcome: %s\n\n", v);
    game_free(g);
}
