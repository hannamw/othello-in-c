/* Michael Hanna, mwhanna
 * CS 152, Spring 2017
 * Project 1
 */

#include "board.h"

/*enums representing whose turn it is*/
enum turn {
    BLACK_NEXT,
    WHITE_NEXT
};

typedef enum turn turn;

/*enums representing the outcome of the game*/
enum outcome {
    BLACK_WIN,
    WHITE_WIN,
    DRAW
};

typedef enum outcome outcome;

/*a game containing a game board and an enum for whose turn it is*/
struct game {
    board* b;
    turn next;
};

typedef struct game game;

/*creates new game with given size and desired data representation and 
two pieces in the correct starting position.*/
game* new_game(unsigned int nrows, unsigned int ncols, enum type type);

/*returns a pseudo-boolean int for if a piece (of the color specified by
the turn in g) placed at p would outflank (i.e. would flip over pieces
of the opposite color*/
int outflanks(game* g, pos p);

/*returns a list of outflanked pieces given a piece (of the color specified
by the turn in g) placed at p.*/
poslist* flips(game* g, pos p);

/*places a piece (of the color specified by the turn in g) at p and flips
any pieces it would outflank, then moves to the next turn*/
void apply_move(game* g, pos p);

/*returns a pseudo-boolean int for if the game g is over (i.e. both players
must pass consecutively or the board is full)*/
int game_over(game* g);

/*returns whether the game was won by black or white or if there was a draw*/
outcome game_outcome(game* g);

/* NON-REQUIRED FUNCTIONS*/

/*returns a pseudo-boolean int for if x and y are on the board of g*/
int in_bounds(game* g, int x, int y);

/*returns a pseudo-boolean int for if a piece placed at pos p (of the color 
specified by game g) would outflank any pieces in the given x, y direction.
Should only be called with integer x and y in [-1,1]. 0,0 does not ouflank.*/
int outflank_path(game* g, pos p, int x, int y);

/*returns a pseudo-boolean int representing if the current player has a valid
move*/
int exists_move(game* g);

/*uses out parameters to return the # of black and white pieces on the board*/
void piece_count(game* g, unsigned* black, unsigned* white);

/*changes the turn of g to the next player*/
void pass(game* g);

/*frees the board struct within the game and then frees the game*/
void game_free(game* g);

/*shows the board of a game (created to avoid calling board-level functions
while playing a game)*/
void game_show(game* g);

/*applies a move to a game, and shows the new board state (created to avoid 
calling board-level functions while playing a game)*/
void apply_show(game* g, pos p);

/* CODE EXCLUSIVELY FOR TESTING*/

/*Shows all pieces on a game and board, and whether, for a given turn, every 
spot would outflank*/
void game_outflanks_show(game* g);

/*Given a piece to be placed, shows what it would flip*/
void game_flips_show(game* g, pos pl);