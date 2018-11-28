/* Michael Hanna, mwhanna
 * CS 152, Spring 2017
 * Project 1
 */

#include "pos.h"

void binary_int(unsigned int n);

/*a square, either empty, black, or white*/
enum square {
    EMPTY,
    BLACK,
    WHITE
};

typedef enum square square;

/*a board representation, either done with enums or bits*/
union board_rep {
    enum square** cells;
    unsigned int* bits;
};

typedef union board_rep board_rep;

/*a tag to store the kind of board representation being used*/
enum type {
    CELLS, BITS
};

/*a board with a board representation, tag for the rep, and # rows /cols*/
struct board {
    unsigned int nrows, ncols;
    enum type type;
    board_rep u;
};

typedef struct board board;

/*creates a new nrows by ncols board with the given certain type of rep*/
board* board_new(unsigned int nrows, unsigned int ncols, enum type type);

/*frees the given board*/
void board_free(board* b);

/*prints the given board*/
void board_show(board* b);

/*gets a square from the board given a pos*/
square board_get(board* b, pos p);

/*sets a square at pos p to the given square s*/
void board_set(board* b, pos p, square s);

/*prints a label given an index*/
void print_label(unsigned n);