/* Michael Hanna, mwhanna
 * CS 152, Spring 2017
 * Project 1
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "board.h"

/*creates a new cell board representation for the given board*/
void cell_br_new(board* this_board){
    unsigned nrows = this_board->nrows, ncols = this_board->ncols;
    
    this_board->u.cells = (square**) malloc(sizeof(square*) * nrows);
    if(!this_board->u.cells){
        fprintf(stderr, "board_new: malloc failed");
        exit(1);
    }
        
    unsigned i, j;
    for(i = 0; i < nrows; i++){
        this_board->u.cells[i] = (square*) malloc(sizeof(square) * ncols);
        if(!this_board->u.cells[i]){
            fprintf(stderr, "board_new: malloc failed");
            exit(1);
        }

        for(j = 0; j < ncols; j++){
            this_board->u.cells[i][j] = EMPTY;
        }
    }
}

/*creates a new bit board representation for the given board*/
void bit_br_new(board* this_board){
    unsigned nrows = this_board->nrows, ncols = this_board->ncols, i;
    unsigned nrc = nrows*ncols, num_entries = nrc / 16;
    if(nrc%16){
        num_entries++;
    }
    
    this_board->u.bits = (unsigned*) malloc(sizeof(unsigned) * num_entries);
    
    if(!this_board->u.bits){
            fprintf(stderr, "board_new: malloc failed");
            exit(1);
    }
    
    for (i = 0; i < num_entries; i++){
        this_board->u.bits[i] = 0;
    }
}

/*creates a new nrows by ncols board with the given certain type of rep*/
board* board_new(unsigned int nrows, unsigned int ncols, enum type type){
    board* this_board = (board*) malloc(sizeof(board));
    if(!this_board){
        fprintf(stderr, "board_new: malloc failed");
        exit(1);
    }
    
    this_board->nrows = nrows;
    this_board->ncols = ncols;
    this_board->type = type;
    
    switch(type){
        case CELLS:
            cell_br_new(this_board);
            break;

        case BITS:
            bit_br_new(this_board);
            break;
        
        default:
            fprintf(stderr, "board_new: type not CELLS or BIT");
            exit(1);
    }
    
    return this_board;
}

/*frees a cell rep of a board*/
void cell_free(board* b){
    square** c = b->u.cells;
    if(!c){
        fprintf(stderr, "board_free: tried to free NULL cell rep");
        exit(1);
    }
    
    unsigned nrows = b->nrows, i;
    for(i = 0; i < nrows; i++){
        free(c[i]);
    }
    free(c);
}

/*frees a bit rep of a board*/
void bit_free(board* b){
    unsigned* c = b->u.bits;
    if(!c){
        fprintf(stderr, "board_free: tried to free NULL bit rep");
        exit(1);
    }
    free(c);
}

/*frees the given board*/
void board_free(board* b){
    if(!b){
        fprintf(stderr, "board_free: tried to free NULL");
        exit(1);
    }
    
    switch(b->type){
        case CELLS:
            cell_free(b);
            break;

        case BITS:
            bit_free(b);
            break;
        
        default:
            fprintf(stderr, "board_free: type not CELLS or BITS");
            exit(1);
    }
    free(b);
}

/*prints a label given an index*/
void print_label(unsigned n){
    if(n < 10){
        printf("%d", n);
    } else if (n < 36) {
        printf("%c", 'A' + n - 10);
    } else if (n < 62) {
        printf("%c", 'a' + n - 36);
    } else {
        printf("?");
    }
}

/*shows a pos given an integer that should be 0, 1, or 2*/
void pos_show(unsigned n){
    switch(n){
        case 0:
            printf(" ");
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

/*shows a cell rep of a board*/
void cell_show(board* b){
    unsigned nrows = b->nrows, ncols = b->ncols, i, j;
    square** s = b->u.cells;
    if(!s){
        fprintf(stderr, "board_show: tried to show NULL cell rep");
        exit(1);
    }
    for(i = 0; i < nrows; i++){
        print_label(i);
        for (j = 0; j < ncols; j++){
           pos_show(s[i][j]);
        }
        printf("\n");
   }
}

void index_bitshift(board*b, pos p, unsigned *index, unsigned *bt){
    unsigned loc = 2*(p.r * b->ncols + p.c);
    *index = loc / 32;
    *bt = 30 - (loc % 32);
}

/*returns a square from a bit representation*/
square bit_get(board* b, pos p){
    unsigned* s = b->u.bits;
    if(!s){
        fprintf(stderr, "board_get: tried to get from NULL bit rep");
        exit(1);
    }
    
    unsigned i, bt;
    index_bitshift(b, p, &i, &bt);
    return (s[i]>>bt)&3;
}

/*shows a bit rep of a board*/
void bit_show(board *b){
    if(!(b->u.bits)){
        fprintf(stderr, "board_show: tried to show NULL bit rep");
        exit(1);
    }
    
    unsigned nrows = b->nrows, ncols = b->ncols, i, j;

    for(i = 0; i < nrows; i++){
        print_label(i);
        for (j = 0; j < ncols; j++){
           pos_show(bit_get(b, make_pos(i, j)));
        }
        printf("\n");
   }
    
    /*I'M NOT SURE IF THIS WAY WAS MORE EFFICIENT, BUT IT ALSO WORKS. HOWEVER,
    IT'S RATHER UNSIGHTLY AND DIFFICULT TO READ, SO I'VE GONE WITH THE ABOVE*/
    
    /*
    unsigned nrows = b->nrows, ncols = b->ncols, num_entries = nrows * ncols;
    unsigned bitshift = (sizeof(int)*8 - 2), i, index = 0, label = 0;
    unsigned this_num = b->u.bits[index];
    
    for(i = 0; i < num_entries; i++){
        //print label
        if(!(i % ncols)){
            print_label(label++);
        }
        //print square
        pos_show((this_num>>bitshift)&3);
        //print newline for column
        if(!((i + 1)%ncols)){
            printf("\n");
        }
        //change to next index if necessary
        if(!bitshift){
            this_num = b->u.bits[++index];
            bitshift = sizeof(int)*8 - 2;
        } else {
            bitshift -= 2;
        }
    }*/
}

/*prints the given board*/
void board_show(board* b){
    if(!b){
        fprintf(stderr, "board_show: tried to show NULL board");
        exit(1);
    }
    /*first row of labels*/
    unsigned i, ncols = b->ncols;
    printf(" ");
    for (i = 0; i < ncols; i++){
        print_label(i);
    }
    /*actual board*/
    printf("\n");
    switch(b->type){     
        case CELLS:
            cell_show(b);
            break;

        case BITS:
            bit_show(b);
            break;
        
        default:
            fprintf(stderr, "board_show: type not CELLS or BITS");
            exit(1);
    }
    printf("\n");
}

/*gets a square from a cell rep*/
square cell_get(board* b, pos p){
    square** s = b->u.cells;
    if(!s){
        fprintf(stderr, "board_get: tried to get from NULL cell rep");
        exit(1);
    }
    return s[p.r][p.c];
}

/*gets a square from the board given a pos*/
square board_get(board* b, pos p){
    if(!b){
        fprintf(stderr, "board_get: tried to get pos from NULL board");
        exit(1);
    }
    switch(b->type){
        case CELLS:
            return cell_get(b, p);
            break;

        case BITS:
            return bit_get(b, p);
            break;
        
        default:
            fprintf(stderr, "board_get: type not CELLS or BITS");
            exit(1);
    }
}

/*sets pos p on board b of a cell rep to square s*/
void cell_set(board* b, pos p, square s){
    square** q = b->u.cells;
    if(!q){
        fprintf(stderr, "board_get: tried to get from NULL cell rep");
        exit(1);
    }
    q[p.r][p.c] = s;
}

/*sets pos p on board b of a bit rep to square s*/
void bit_set(board* b, pos p, square s){
    unsigned* q = b->u.bits;
    if(!q){
        fprintf(stderr, "board_get: tried to get from NULL bit rep");
        exit(1);
    }
    unsigned i, bt;
    index_bitshift(b, p, &i, &bt);
    unsigned n = q[i];
    //for some reason, n>>32 returns n, not 0, so set half1 to 0 instead.
    unsigned half1 = (bt==30)? 0: n>>(bt+2), half2 = n&((1<<bt)-1);
    q[i] = (half1<<(bt+2)) + (s<<bt) + half2;
}

/*sets a square at pos p to the given square s*/
void board_set(board* b, pos p, square s){
    if(!b){
        fprintf(stderr, "board_set: tried to set pos on NULL board");
        exit(1);
    }
    switch(b->type){
        case CELLS:
            cell_set(b, p, s);
            break;

        case BITS:
            bit_set(b, p, s);
            break;
        
        default:
            fprintf(stderr, "board_set: type not CELLS or BITS");
            exit(1);
    }
}
