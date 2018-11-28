/* Michael Hanna, mwhanna
 * CS 152, Spring 2017
 * Project 1
 */

#include <stdlib.h>
#include <stdio.h>
#include "pos.h"

/*makes and returns a pos with the given row and column*/
pos make_pos(unsigned int r, unsigned int c){
    pos this_pos;
    this_pos.r = r;
    this_pos.c = c;
    return this_pos;
}

/*prepends the pos p onto the poslist* ps*/
poslist* pos_cons(pos p, poslist* ps){
    poslist* this_list = (poslist*) malloc(sizeof(poslist));
    if(!this_list){
        fprintf(stderr, "pos_cons: malloc failed");
        exit(1);
    }
    
    this_list->p = p;
    this_list->next = ps;
    return this_list;
}

/*frees a poslist in its entirety*/
void poslist_free(poslist* ps){
    if(!ps){
        fprintf(stderr, "poslist_free: tried to free NULL");
        exit(1);
    }
    
    poslist* next_list;
    for(; ps; ps = next_list){
        next_list = ps->next;
        free(ps);
    }
}

void pos_struct_show(pos p){
    printf("Row: %d, Col: %d\n", p.r, p.c);
}

void poslist_show(poslist* ps){
    for(; ps; ps = ps->next){
        pos_struct_show(ps->p);
    }
}