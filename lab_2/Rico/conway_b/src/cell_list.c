#include "cell_list.h"

#include <stdlib.h>
#include <string.h>


struct cell_list_node_t {
	int x;
	int y;
	struct cell_list_node_t *next;
};


void cell_list_init(cell_list *l) {
	cell_list_node *cell = malloc(sizeof(cell));
	
	// initialize head dummy element
	l->head = cell;
	l->tail = cell;
	l->cur  = cell;
	
	cell->x    = -1;
	cell->y    = -1;
	cell->next = NULL;
}

void cell_list_free(cell_list *l) {
	cell_list_empty(l);
	
	// free dummy
	free(l->head);
}

void cell_list_empty(cell_list *l) {
	cell_list_node *cur = l->head->next;
	
	// free each non-dummy element
	while (cur) {
		cell_list_node *next = cur->next;
		free(cur);
		cur = next;
	}
	
	l->head->next = NULL;
	l->tail = l->head;
	
	cell_list_restart(l);
}

bool cell_list_isempty(cell_list *l) {
	return l->head == l->tail;
}

void cell_list_append(cell_list *l, int x, int y) {
	// create new cell
	cell_list_node *cell = malloc(sizeof(cell));
	cell->x = x;
	cell->y = y;
	cell->next = NULL;
	
	// append new cell
	l->tail->next = cell;
	l->tail = cell;
}

void cell_list_merge(cell_list *l1, cell_list *l2) {
	if (cell_list_isempty(l2))
		return;
		
	// append second list on first one
	l1->tail->next = l2->head->next;
	l1->tail = l2->tail;
	
	// empty second list
	l2->head->next = NULL;
	l2->tail = l2->head;
	
	cell_list_restart(l2);
}

bool cell_list_next(cell_list *l, int *x, int *y) {
	cell_list_node *next = l->cur->next;
	
	// unless at the end
	if (next) {
		*x = next->x;
		*y = next->y;
		l->cur = next;
		
		return true;
	} else {
		return false;
	}
}

void cell_list_restart(cell_list *l) {
	l->cur = l->head;
}

