/* Michael Hanna, mwhanna
 * CS 152, Spring 2017
 * Project 2
 */
  
#include <stdlib.h>
#include <stdio.h>
#include "players.h"

/*returns the first valid move for a given turn*/
pos first_move(game* g, strategy_config unused){
    unsigned i, j;
    pos p;
    for(i = 0; i < g->b->nrows; i++){
        for(j = 0; j < g->b->ncols; j++){
            p = make_pos(i, j);
            if(outflanks(g, p)){
                return p;
            }
        }
    }
    fprintf(stderr, "first_move: no valid (outflanking) moves");
    exit(1);
}

/*returns a pseudo-boolean int for if the given char is a valid input*/
int valid_rc(char c){
    return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') ||
           (c >= 'a' && c <= 'z');
}

/*converts from a row / column display character to the corresponding row / 
column index*/
unsigned ctrc(char c){
    if(c <= '9'){
        return c - '0';
    } else if(c <= 'Z'){
        return c - 'A' + 10;
    } else{
        return c - 'a' + 36;
    }
}

/*prompts the user for a move, telling them if the move is invalid, and returns
the valid move*/
pos human(game* g, strategy_config unused){
    char c1, c2;
    unsigned u1, u2;
    int invalid_input, invalid_move;
    pos p;
    do{
        printf("%s", (g->next)? "White: ": "Black: ");
        scanf(" %c%c", &c1, &c2);
        printf("Your input: r: %c, c: %c\n", c1, c2);
        
        u1 = ctrc(c1);
        u2 = ctrc(c2);
        invalid_input = !in_bounds(g, u1, u2);
        if(invalid_input){
            printf("\nInput invalid; please input valid row / column #\n");
        }
        
        p = make_pos(u1, u2);
        if(!invalid_input){
            invalid_move = !outflanks(g, p);
            if(invalid_move){
                printf("\nInput move does not outflank; input valid move\n");
            }
        }
    }while(invalid_input || invalid_move);
    return make_pos(ctrc(c1), ctrc(c2));
}


/*given pos p, a valid direction (i.e. x and y that do ouflank), return the
number of pieces that will be outflanked in the x y direction from pos p*/
unsigned n_traverse (game* g, pos p, int x, int y){
    square ts = (g->next)? WHITE: BLACK;
    p.r += x;
    p.c += y;
    unsigned n = 0;
    square ns = board_get(g->b, p);
    
    while(ns != ts){
        n++;
        p.r += x;
        p.c += y;
        ns = board_get(g->b, p);
    }
    return n;
}

/*returns the number of outflanked pieces given a piece (of the color specified
by the turn in g) placed at p.*/
unsigned n_flips(game* g, pos p){
    int i, j;
    unsigned n = 0;
    for(i = -1; i <= 1; i++){
        for(j = -1; j <= 1; j++){
            if(outflank_path(g, p, i, j)){//outflank_path in logic.c
                n += n_traverse(g, p, i, j);
            }
        }
    }
    return n;
}

/*given a game, updates n to the number of flips of the highest-flipping valid
corner move, and p to the position thereof*/
void corners(game* g, unsigned* n, pos* p){
    unsigned nr = g->b->nrows, nc = g->b->ncols;
    pos tp = make_pos(0,0);
    unsigned nf;
    if(outflanks(g, tp)){
        nf = n_flips(g, tp);
        *p = tp;
        *n = nf;
    }
    tp = make_pos(0, nc - 1);
    if(outflanks(g, tp)){
        nf = n_flips(g, tp);
        if(nf > *n){
            *p = tp;
            *n = nf;
        }
    }
    tp = make_pos(nr - 1, 0);
    if(outflanks(g, tp)){
        nf = n_flips(g, tp);
        if(nf > *n){
            *p = tp;
            *n = nf;
        }
    }
    tp = make_pos(nr - 1, nc - 1);
    if(outflanks(g, tp)){
        nf = n_flips(g, tp);
        if(nf > *n){
            *p = tp;
            *n = nf;
        }
    }
}

/*given a game, updates n to the number of flips of the highest-flipping valid
edge move, and p to the position thereof*/
void edges(game* g, unsigned* n, pos* p){
    unsigned nr = g->b->nrows, nc = g->b->ncols;
    pos tp;
    unsigned nf, i, j, rb = nr - 1, cb = nc - 1;
    
    for(j = 1; j < cb; j++){
        tp = make_pos(0, j);
        if(outflanks(g, tp)){
            nf = n_flips(g, tp);
            if(nf > *n){
                *p = tp;
                *n = nf;
            }
        }
    }
    for(i = 1; i < rb; i++){
        tp = make_pos(i, 0);
        if(outflanks(g, tp)){
            nf = n_flips(g, tp);
            if(nf > *n){
                *p = tp;
                *n = nf;
            }
        }
        tp = make_pos(i, cb);
        if(outflanks(g, tp)){
            nf = n_flips(g, tp);
            if(nf > *n){
                *p = tp;
                *n = nf;
            }
        }
    }
    for(j = 1; j < cb; j++){
        tp = make_pos(rb, j);
        if(outflanks(g, tp)){
            nf = n_flips(g, tp);
            if(nf > *n){
                *p = tp;
                *n = nf;
            }
        }
    }
}

/*given a game, updates n to the number of flips of the highest-flipping valid
edge move, and p to the position thereof*/
void interior(game* g, unsigned* n, pos* p){
    unsigned nr = g->b->nrows, nc = g->b->ncols;
    pos tp;
    unsigned nf, i, j, rb = nr - 1, cb = nc - 1;
    
    for(i = 1; i < cb; i++){
        for(j = 1; j < rb; j++){
            tp = make_pos(i, j);
            if(outflanks(g, tp)){
                nf = n_flips(g, tp);
                if(nf > *n){
                    *p = tp;
                    *n = nf;
                }
            }
        }
    }
}

/*returns a pos move, prioritizing corners over edges over interior pieces*/
pos immediate_tactics(game* g, strategy_config unused){
    unsigned n = 0;
    pos p;
    corners(g, &n, &p);
    if(n){
        return p;
    }
    edges(g, &n, &p);
    if(n){
        return p;
    }
    interior(g, &n, &p);
    if(n){
        return p;
    }
    fprintf(stderr, "immediate_tactics: found no valid moves");
    exit(1);
}

/*checks if the game is over, and returns the winner*/
int did_i_win(game* g, heuristic_config unused){
    if(!game_over(g)){
        return 0;
    }
    unsigned go = game_outcome(g);
    return (go < 2)? (go? -1: 1):0;
}

/*counts the number of pieces on the board, unweighted*/
int piece_counting(game* g, heuristic_config unused){
    unsigned black = 0, white = 0;
    piece_count(g, &black, &white);//in logic.c
    return black - white;
}

/*takes in the value to be incremented, # rows - 1, # cols - 1, edge weight,
corner weight, position row, position col, and increments the value*/
void add_weight(unsigned* n, unsigned nr1, unsigned nc1, int ew, int cw, 
                unsigned pr, unsigned pc){
    if(!pr){
        if(!pc || pc == nc1){
            (*n) += cw;
        }else{
            (*n) += ew;
        }
    }else if(pr == nr1){
        if(!pc || pc == nc1){
            (*n) += cw;
        }else{
            (*n) += ew;
        }
    }else if(!pc || pc == nc1){
        (*n) += ew;
    }else {
        (*n)++;
    }
}          

/*uses out parameters to return the # of black and white pieces on the board,
weighting corners and edges by the given amount*/
void piece_count_weighted(game* g, unsigned* black, unsigned* white,
                          int ew, int cw){
    unsigned i, j, nr = g->b->nrows, nc = g->b->ncols;
    unsigned nr1 = nr - 1, nc1 = nc - 1;
    pos p;
    for(i = 0; i < nr; i++){
        for (j = 0; j < nc; j++){
            p = make_pos(i, j);
            switch(board_get(g->b, p)){
                case BLACK:
                    add_weight(black, nr1, nc1, ew, cw, i, j);
                    break;
                case WHITE:
                    add_weight(white, nr1, nc1, ew, cw, i, j);
                    break;
                default:
                    break;
            }
        }
    }
}

/*evaluates a game, weighting edges as specified by the heuristic_config*/
int prefer_edges(game* g, heuristic_config hc){
    unsigned black = 0, white = 0;
    piece_count_weighted(g, &black, &white, hc.edge_weight, hc.edge_weight);
    return black - white;
}

/*evaluates a game, weighting edges and corners as specified by the
heuristic_config*/
int prefer_edges_and_corners(game* g, heuristic_config hc){
    unsigned black = 0, white = 0;
    piece_count_weighted(g, &black, &white, hc.edge_corner_weight.edge_weight, 
                         hc.edge_corner_weight.corner_weight);
    return black - white;
}

/*hard-copies a game*/
game* game_copy(game* g){
    if(!g){
        fprintf(stderr, "game_copy: game is null");
        exit(1);
    }
    unsigned nrows = g->b->nrows, ncols = g->b->ncols;
    game* ng = new_game(nrows, ncols, g->b->type);
    ng->next = g->next;    
    
    unsigned i, j;
    if(g->b->type == CELLS){
        for(i = 0; i < nrows; i++){
            for(j = 0; j < ncols; j++){
                ng->b->u.cells[i][j] = g->b->u.cells[i][j];
            }
        }
    } else {//copying a bitrep board is much faster
        unsigned nrc = nrows*ncols, num_entries = nrc / 16;
        if(nrc%16){
            num_entries++;
        }
        for(i = 0; i < num_entries; i++){
            ng->b->u.bits[i] = g->b->u.bits[i];
        }
    }
    return ng;
}

/*makes a minimax_config given all of its fields*/
minimax_config make_mc(int (*heuristic)(game*, heuristic_config),
                        heuristic_config hc, unsigned ply){
    minimax_config mc;
    mc.heuristic = heuristic;
    mc.hc = hc;
    mc.ply = ply;
    return mc;
}

/*returns either the current min/max or the new value, ng_eval, depending on
those two parameters and the game turn`*/
int game_min_max(game* g, int curr_m, int ng_eval){
    if(g->next){//white
        return (curr_m <= ng_eval)? curr_m: ng_eval;
    } else {//black
        return (curr_m >= ng_eval)? curr_m: ng_eval;
    }
}

/*returns the score of a given gameboard, looking ahead the given number of
moves / plies*/
int minimax_eval(game* g, minimax_config mc){
    game* ng;
    int ng_eval, mm_set = 0, min_max, i, j;
    pos p;
    if(!mc.ply){
        return mc.heuristic(g, mc.hc);
    }
    for(i = 0; i < g->b->nrows; i++){
        for(j = 0; j < g->b->ncols; j++){
            p = make_pos(i, j);
            if(outflanks(g, p)){
                ng = game_copy(g);
                apply_move(ng, p);
                ng_eval = minimax_eval(ng, 
                    make_mc(mc.heuristic, mc.hc, mc.ply - 1));
                game_free(ng);
                if(!mm_set){
                    min_max = ng_eval;
                    mm_set = 1;
                } else {
                    min_max = game_min_max(g, min_max, ng_eval);
                }
            }
       }
    }
    if(mm_set){
        return min_max;
    }
    
    //Because we know there are no valid moves, we can pass and check only if
    //there are moves for the next player, because we know there are none for
    //the current player. If the next player has moves, we should pass and
    //go to them; if not, the simulated game is over.
    pass(g);
    
    if(exists_move(g)){
        return minimax_eval(g, make_mc(mc.heuristic, mc.hc, mc.ply - 1));
    }
    //Currently if the game is over, the heuristic returned is simply the 
    //board heuristic; however, an approach like the one in the below comment
    //would always prioritize a winning board state, even if there were a
    //higher-scoring, non-winning board state possible
    return mc.heuristic(g, mc.hc);
    
    /*
    int max = (1<<15) - 1;
    //If it's black's turn, then black won, and vice-versa.
    //Because the game-over state is the ultimate state in favor of
    //black or white, its value should be incredibly high / low
        
    return (g->next)? -max: max;
    */
}

/*returns a pseudo-boolean int for if the given value, ng_eval, is a min / max,
given the turn that it is*/
int mm_bool(game* g, int min_max, int ng_eval){
    return (g->next)? (ng_eval < min_max):(ng_eval > min_max);
}

/*given a game, looks ahead the specified number of plies (minimum 1 ply) and
returns the "optimal" pos*/
pos minimax(game* g, strategy_config sc){
    if(!sc.minimax_config.ply){
        fprintf(stderr, "minimax: ply must be greater than zero");
    exit(1);
    }
    pos mm_p, p;
    int mm, ng_eval, set = 0, i, j;
    game* ng;
    sc.minimax_config.ply--;//these two for loops do 1 ply of work
    for(i = 0; i < g->b->nrows; i++){
        for(j = 0; j < g->b->ncols; j++){
            p = make_pos(i, j);
            if(outflanks(g, p)){
                ng = game_copy(g);
                apply_move(ng, p);
                ng_eval = minimax_eval(ng, sc.minimax_config);
                game_free(ng);
                if(!set||mm_bool(g, mm, ng_eval)){
                    set = 1;
                    mm = ng_eval;
                    mm_p = p;
                }
            }
        }
    }
    if(set){
        return mm_p;
    }
    fprintf(stderr, "minimax: no valid moves found");
    exit(1);
}
