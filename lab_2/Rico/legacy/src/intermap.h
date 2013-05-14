#ifndef INTERMAP_H
#define INTERMAP_H


#include "cell_list.h"


typedef struct {
	cell_list touched_oborder; // touched by inner border
	cell_list touched_iborder; // touched by inner border, core, and foreigners
	cell_list touched_core;    // touched by inner border and core
	cell_list current_iborder; // touches outer/inner border and core
	cell_list current_core;    // touches inner border and core
	cell_list future_iborder;
	cell_list future_core;
	char      *neighbors;
} intermap;


void intermap_init(intermap *inter, int size);
void intermap_free(intermap *inter);
void intermap_reset_neighbors(intermap *inter, int size);

void intermap_append(intermap *inter, int width, int height, int x, int y);
void intermap_commit(intermap *inter, cell_list *target);


#endif // INTERMEDIATE_MAP_H
