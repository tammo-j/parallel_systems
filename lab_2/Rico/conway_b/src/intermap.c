#include "intermap.h"

#include <stdlib.h>
#include <string.h>


void intermap_init(intermap *inter, int width, int height) {
	inter->width = width;
	inter->height = height;
	
	cell_list_init(&inter->touched_oborder);
	cell_list_init(&inter->touched_iborder);
	cell_list_init(&inter->touched_core);
	cell_list_init(&inter->current_iborder);
	cell_list_init(&inter->current_core);
	cell_list_init(&inter->future_iborder);
	cell_list_init(&inter->future_core);
	
	inter->neighbors = calloc(width*height, sizeof(*inter->neighbors));
}

void intermap_free(intermap *inter) {
	cell_list_init(&inter->touched_oborder);
	cell_list_free(&inter->touched_iborder);
	cell_list_free(&inter->touched_core);
	cell_list_free(&inter->current_iborder);
	cell_list_free(&inter->current_core);
	cell_list_free(&inter->future_iborder);
	cell_list_free(&inter->future_core);
	
	free(inter->neighbors);
}

void intermap_reset_neighbors(intermap *inter) {
	int size = inter->width * inter->height;
	
	memset(inter->neighbors, 0, size * sizeof(*inter->neighbors));
}

void intermap_fill(intermap *inter, map *map) {
	int x, y;
	map_restart(map, 0);
	while (map_next(map, 0, &x, &y))
		intermap_append(inter, x, y);
}

void intermap_append(intermap *inter, int x, int y) {
	if ((x == 1 || x == inter->width -2) ||
		(y == 1 || y == inter->height-2))
	{
		cell_list_append(&inter->current_iborder, x, y);
	} else {
		cell_list_append(&inter->current_core, x, y);
	}
}

void intermap_commit(intermap *inter, map *map, int t) {
	cell_list *target = map_get_level(map, t);
	
	// move changes to target
	cell_list_merge(target, &inter->current_iborder);
	cell_list_merge(target, &inter->current_core);
	
	// future becomes intermediate state
	cell_list_merge(&inter->current_iborder, &inter->future_iborder);
	cell_list_merge(&inter->current_core   , &inter->future_core);
}

