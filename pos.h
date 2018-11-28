/* Michael Hanna, mwhanna
 * CS 152, Spring 2017
 * Project 1
 */

/*a pos that stores a row and column*/
struct pos {
    unsigned int r, c;
};

typedef struct pos pos;

typedef struct poslist poslist;

/*a linked list of pos structs*/
struct poslist {
    pos p;
    poslist* next;
};

/*makes and returns a pos with the given row and column*/
pos make_pos(unsigned int r, unsigned int c);

/*prepends the pos p onto the poslist* ps*/
poslist* pos_cons(pos p, poslist* ps);

/*frees a postlist in its entirety*/
void poslist_free(poslist* ps);

void pos_struct_show(pos p);

void poslist_show(poslist* ps);