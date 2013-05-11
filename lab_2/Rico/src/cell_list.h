#ifndef CELL_LIST_H
#define CELL_LIST_H


#include <stdbool.h>

typedef struct cell_list_node_t cell_list_node;

typedef struct {
	cell_list_node *head;
	cell_list_node *tail;
	cell_list_node *cur;
} cell_list;


void cell_list_init(cell_list *l);
void cell_list_free(cell_list *l);
void cell_list_empty(cell_list *l);

void cell_list_append(cell_list *l, int x, int y);
void cell_list_merge(cell_list *l1, cell_list *l2);
bool cell_list_next(cell_list *l, int *x, int *y);
void cell_list_restart(cell_list *l);



#endif // CELL_LIST_H
