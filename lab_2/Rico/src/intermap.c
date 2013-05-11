#include "intermap.h"

#include <stdlib.h>
#include <string.h>


void intermap_init(intermap *inter, int size) {
	cell_list_init(&inter->oborder);
	cell_list_init(&inter->iborder);
	cell_list_init(&inter->core);
	cell_list_init(&inter->touched_border);
	cell_list_init(&inter->touched_core);
	
	inter->neighbors = calloc(size, sizeof(char));
}

void intermap_free(intermap *inter) {
	cell_list_free(&inter->oborder);
	cell_list_free(&inter->iborder);
	cell_list_free(&inter->core);
	cell_list_free(&inter->touched_border);
	cell_list_free(&inter->touched_core);
	
	free(inter->neighbors);
}

void intermap_reset_neighbors(intermap *inter, int size) {
	memset(inter->neighbors, 0, size*sizeof(char));
}

void intermap_append(intermap *inter, int width, int height, int x, int y) {
	if ((x >= 3 && x <= width -4) ||
		(y >= 3 && y <= height-4))
	{
		cell_list_append(&inter->core, x, y);
	} else if (
		(x == 0 && x == width -1) ||
		(y == 0 && y == height-1))
	{
		cell_list_append(&inter->oborder, x, y);
	} else {
		cell_list_append(&inter->iborder, x, y);
	}
}

void intermap_finalize(intermap *inter, cell_list *l) {
	cell_list_merge(l, &inter->iborder);
	cell_list_merge(l, &inter->core);
}

void intermap_merge_border(intermap *inter, cell_list *l) {
	cell_list_merge(l, &inter->iborder);
}

void intermap_merge_core(intermap *inter, cell_list *l) {
	cell_list_merge(l, &inter->core);
}

