#include "cell_list.h"

#include <stdlib.h>
#include <string.h>


typedef struct cell_t {
	int x;
	int y;
	struct cell_t *next;
} cell;


void cell_list_init(cell_list *l) {
	cell *cell = malloc(sizeof(cell));
	
	l->head = cell;
	l->tail = cell;
	l->cur  = cell;
	
	cell->next = NULL;
}

void cell_list_free(cell_list *l) {
	cell *cur = l->head;
	
	// free each element
	// note that the first element is the dummy
	do {
		cell *next = cur->next;
		free(cur);
		cur = next;
	} while (cur);
}

void cell_list_append(cell_list *l, int x, int y) {
	// create new cell
	cell *cell = malloc(sizeof(cell));
	cell->x = x;
	cell->y = y;
	cell->next = NULL;
	
	// append new cell
	l->tail->next = cell;
	l->tail = cell;
}

bool cell_list_next(cell_list *l, int *x, int *y) {
	cell *next = l->cur->next;
	
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

