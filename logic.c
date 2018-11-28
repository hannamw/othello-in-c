/* Michael Hanna, mwhanna
 * CS 152, Spring 2017
 * Project 1
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "logic.h"

/*creates new game with given size and desired data representation and 
two pieces in the correct starting position.*/
game* new_game(unsigned int nrows, unsigned int ncols, enum type type){
    if(nrows < 2 || ncols < 2){
        fprintf(stderr, "newgame: nrows and ncols must be >= 2");
        exit(1);
    }
    
    game* g = (game*) malloc(sizeof(game));
    g->next = BLACK_NEXT;
    g->b = board_new(nrows, ncols, type);
    board* tb = g->b;
    
    unsigned hrow = nrows/2, hcol = ncols/2;
    pos p = make_pos(hrow, hcol);
    board_set(tb, p, WHITE);
    p.r--;
    board_set(tb, p, BLACK);
    p.c--;
    board_set(tb, p, WHITE);
    p.r++;
    board_set(tb, p, BLACK);
    return g;
}

/*returns a pseudo-boolean int for if x and y are on the board of g*/
int in_bounds(game* g, int x, int y){
    return (x >= 0 && y >= 0 && x < g->b->nrows && y < g->b->ncols);
}

/*returns a pseudo-boolean int for if a piece placed at pos p (of the color 
specified by game g) would outflank any pieces in the given x, y direction.
Should only be called with integer x and y in [-1,1]. 0,0 does not ouflank.*/
int outflank_path(game* g, pos p, int x, int y){
    square ts = (g->next)? WHITE: BLACK;
    int ox = p.r + x, oy = p.c + y;
    /*test square next to origin)*/
    if(!(in_bounds(g, ox, oy) && (x || y))){
        return 0;
    }
         
    p = make_pos(ox, oy);
    square ns = board_get(g->b, p);
       
    if(ts == ns || !ns){
        return 0;
    }
    /*test following squares in line*/
    ox += x;
    oy += y;
    while(in_bounds(g, ox, oy)){
        p = make_pos(ox, oy);
        ns = board_get(g->b, p);
         
        if(!ns){
            return 0;
        }
        if(ns == ts){
            return 1;
        }
        ox += x;
        oy += y;
    }    
    return 0;
}

/*returns a pseudo-boolean int for if a piece (of the color specified by
the turn in g) placed at p would outflank (i.e. would flip over pieces
of the opposite color*/
int outflanks(game* g, pos p){
    if(!in_bounds(g, p.r, p.c)){
        fprintf(stderr, "outflanks: move %c, %c is out of bounds", p.r, p.c);
        exit(1);
    }
    if(board_get(g->b, p)){
        return 0; //already occupied
    }
    
    int i, j;
    for(i = -1; i <= 1; i++){
        for(j = -1; j <= 1; j++){
            if(outflank_path(g, p, i, j)){
                return 1;
            }
        }
    }
    return 0;
}

/*given pos p, a valid direction (i.e. x and y that do ouflank), creates a 
linked list consisting of all pieces that will be outflanked in the x y 
direction from pos p, attached to acc*/
poslist* traverse (game* g, pos p, poslist* acc, int x, int y){
    square ts = (g->next)? WHITE: BLACK;
    p.r += x;
    p.c += y;
    poslist* ps = pos_cons(p, acc);
    square ns = board_get(g->b, p);
    
    while(ns != ts){
        ps = pos_cons(p, ps);
        p.r += x;
        p.c += y;
        ns = board_get(g->b, p);
    }
    return ps;
}

/*returns a list of outflanked pieces given a piece (of the color specified
by the turn in g) placed at p.*/
poslist* flips(game* g, pos p){
    int i, j;
    poslist* acc = NULL;
    for(i = -1; i <= 1; i++){
        for(j = -1; j <= 1; j++){
            if(outflank_path(g, p, i, j)){
                acc = traverse(g, p, acc, i, j);
            }
        }
    }
    return acc;
}

/*changes the turn of g to the next player*/
void pass(game* g){
    g->next = (g->next)? BLACK_NEXT: WHITE_NEXT;
}

/*places a piece (of the color specified by the turn in g) at p and flips
any pieces it would outflank, then moves to the next turn*/
void apply_move(game* g, pos p){
    board* b = g->b;
    
    if(!outflanks(g, p)){
        fprintf(stderr, "apply_move: move does not outflank");
        exit(1);
    }
    
    poslist* tf = flips(g, p);
    poslist* st = tf;
    square color = (g->next)? WHITE: BLACK;
    
    board_set(b, p, color);
    while(tf){
        board_set(b, tf->p, color);
        tf = tf->next;
    }
    poslist_free(st);
    pass(g);
}

/*for the color whose turn it is, returns a pseudo-boolean int representing
 if there is a valid move for that color*/
int exists_move(game* g){
    unsigned i, j, nr = g->b->nrows, nc = g->b->ncols;
    pos p;
    for(i = 0; i < nr; i++){
        for (j = 0; j < nc; j++){
            p = make_pos(i, j);
            if(outflanks(g, p)){
                return 1;
            }
        }
    }
    return 0;
}

/*returns a pseudo-boolean int for if the game g is over (i.e. both players
must pass consecutively or the board is full)*/
int game_over(game* g){
    unsigned can_go = exists_move(g);
    if(can_go){
        return 0;
    }
    pass(g);
    can_go = exists_move(g);
    pass(g);
    
    return !can_go;
}

/*uses out parameters to return the # of black and white pieces on the board*/
void piece_count(game* g, unsigned* black, unsigned* white){
    unsigned i, j, nr = g->b->nrows, nc = g->b->ncols;
    pos p;
    for(i = 0; i < nr; i++){
        for (j = 0; j < nc; j++){
            p = make_pos(i, j);
            switch(board_get(g->b, p)){
                case BLACK:
                    (*black)++;
                    break;
                case WHITE:
                    (*white)++;
                    break;
                default:
                    break;
            }
        }
    }
}

/*returns whether the game was won by black or white or if there was a draw*/
outcome game_outcome(game* g){
    unsigned black = 0, white = 0;
    piece_count(g, &black, &white);
    if(black > white){
        return BLACK_WIN;
    } else if (white > black) {
        return WHITE_WIN;
    } else {
        return DRAW;
    }
}

/*frees the board struct within the game and then frees the game*/
void game_free(game* g){
    board_free(g->b);
    free(g);
}

/*shows the board of a game (created to avoid calling board-level functions
while playing a game (no manual dereferencing)*/
void game_show(game* g){
    board_show(g->b);
}

/*applies a move to a game, and shows the new board state*/
void apply_show(game* g, pos p){
    apply_move(g, p);
    game_show(g);
}

/* CODE EXCLUSIVELY FOR TESTING*/

/*shows a pos, either the piece contained, or whether a piece placed there
would outflank*/
void outflanks_show(unsigned n, game* g, pos p){
    switch(n){
        case 0:
            printf("%c", (outflanks(g, p))? '.': ' ');
            break;
        case 1:
            printf("x");
            break;
        case 2:
            printf("o");
            break;
        default:
            fprintf(stderr, "board_show: encountered invalid square");
            exit(1);
    }
}


/*Shows all pieces on a game and board, and whether, for a given turn, every 
spot would outflank*/
void game_outflanks_show(game* g){
    board* b = g->b;
    if(!b){
        fprintf(stderr, "board_show: tried to show NULL board");
        exit(1);
    }
    printf("Showing valid moves for %s:\n", (g->next)? "WHITE": "BLACK");
    
    /*first row of labels*/
    unsigned i, j, nrows = b->nrows, ncols = b->ncols;
    printf(" ");
    for (i = 0; i < ncols; i++){
        print_label(i);
    }
    
    /*actual board*/
    pos p;
    square tq;
    printf("\n");
    for(i = 0; i < nrows; i++){
        print_label(i);
        for (j = 0; j < ncols; j++){
            p = make_pos(i, j);
            tq = board_get(b, p);
            outflanks_show(tq, g, p);
        }
        printf("\n");
    }
    printf("\n");
}

/*searches a poslist to see if it contains p*/
unsigned match(pos p, poslist* fl){
    while(fl){
        if(p.r == fl->p.r && p.c ==fl->p.c){
            return 1;
        }
        fl = fl->next;
    }
    return 0;
}

/*shows a pos, and whether it would be flipped given placement of tp*/
void flips_show(unsigned n, game* g, pos p, pos tp, poslist* fl){
    if(n == (g->next + 1)){     
        printf("%c", (g->next)? 'o':'x');
    } else {
        if(n){
            if(match(p, fl)){
                printf("#");
            } else {
                printf("%c", (n - 1)? 'o':'x');
            }   
        } else {
            if(p.r == tp.r && p.c == tp.c){
                printf("@");
            } else {
                printf(" ");
            }
        }
    }
}

/*Given a piece to be placed, shows what it would flip*/
void game_flips_show(game* g, pos pl){
    board* b = g->b;
    if(!b){
        fprintf(stderr, "game_flips_show: tried to show NULL board");
        exit(1);
    }
    if(!outflanks(g, pl)){
        fprintf(stderr, "game_flips_show: tried to show flips for invalid \
move");
        exit(1);
    }
    
    printf("Showing flips for %s placed at row: %d, col %d:\n", 
        (g->next)? "WHITE": "BLACK", pl.r, pl.c);
    printf("The \"@\" represents the newly-placed %s piece. \nThe \"#\" \
signs represent the %s pieces to be flipped.\n", (g->next)? "WHITE": "BLACK", 
        (g->next)? "BLACK": "WHITE");
        
    /*first row of labels*/
    unsigned i, j, nrows = b->nrows, ncols = b->ncols;
    printf(" ");
    for (i = 0; i < ncols; i++){
        print_label(i);
    }
    
    /*actual board*/
    pos p;
    square tq;
    poslist* fl = flips(g, pl);
    printf("\n");
    for(i = 0; i < nrows; i++){
        print_label(i);
        for (j = 0; j < ncols; j++){
            p = make_pos(i, j);
            tq = board_get(b, p);
            flips_show(tq, g, p, pl, fl);
        }
        printf("\n");
    }
    printf("\n");
    poslist_free(fl);
}