#ifndef CELL_LIST_H
#define CELL_LIST_H


#include <stdbool.h>


typedef struct {
	cell *head;
	cell *tail;
	cell *cur;
} cell_list;


void cell_list_init(cell_list *l);
void cell_list_free(cell_list *l);

void cell_list_append(cell_list *l, int x, int y);
bool cell_list_next(cell_list *l, int *x, int *y);
void cell_list_restart(cell_list *l);


#endif // CELL_LIST_H
