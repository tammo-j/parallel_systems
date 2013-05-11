#ifndef INTERMAP_H
#define INTERMAP_H


#include "cell_list.h"


typedef struct {
	bool      tl_ocorner;
	bool      tr_ocorner;
	bool      bl_ocorner;
	bool      br_ocorner;
	cell_list oborder;
	cell_list iborder;
	cell_list core;
	cell_list touched_border;
	cell_list touched_core;
	char      *neighbors;
} intermap;


void intermap_init(intermap *inter, int size);
void intermap_free(intermap *inter);
void intermap_reset_neighbors(intermap *inter, int size);

void intermap_append(intermap *inter, int width, int height, int x, int y);
void intermap_merge_border(intermap *inter, cell_list *world);
void intermap_merge_core(intermap *inter, cell_list *l);


#endif // INTERMEDIATE_MAP_H
