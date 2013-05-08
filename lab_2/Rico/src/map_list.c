#include "map.h"

#include <stdlib.h>
#include <string.h>

// TODO delete me
#include <stdio.h>

typedef struct map_cell_t {
	int x;
	int y;
	struct map_cell_t *next;
} map_cell;

typedef struct {
	map_cell *head;
	map_cell *tail;
	map_cell *cur;
} map_data;


void map_init(map *map, int duration, int width, int height) {
	map->duration = duration;
	map->width    = width;
	map->height   = height;
	
	map_data *levels = calloc(duration, sizeof(map_data));
	map_cell *cells  = calloc(duration, sizeof(map_cell));
	
	map->data = levels;
	
	// prepare dummies
	for (int i = 0; i < duration; ++i) {
		levels[i].head = &cells[i];
		levels[i].tail = &cells[i];
		levels[i].cur = &cells[i];
		
		cells[i].next = NULL;
	}
}

void map_free(map *map) {
	map_data *data = (map_data *) map->data;
	
	for (int t = 0; t < map->duration; ++t) {
		// first non-dummy element
		map_cell *cur = data[t].head->next;
		
		// free each non-dummy element
		while (cur) {
			map_cell *next = cur->next;
			free(cur);
			cur = next;
		}
	}
	
	// free list dummies
	free(data[0].head);
	// free levels
	free(map->data);
}

void map_append(map *map, int t, int x, int y) {
	map_data *levels = (map_data *) map->data;
	map_data *level = &levels[t];
	
	// create new cell
	map_cell *cell = calloc(1, sizeof(cell));
	cell->x = x;
	cell->y = y;
	cell->next = NULL;
	
	// append new cell
	level->tail->next = cell;
	level->tail = cell;
}

bool map_next(map *map, int t, int *x, int *y) {
	map_data *levels = (map_data *) map->data;
	map_data *level = &levels[t];
	
	map_cell *next = level->cur->next;
	
	if (next) {
		*x = next->x;
		*y = next->y;
		level->cur = next;
		
		return true;
	} else {
		return false;
	}
	
}
void map_restart(map *map, int t) {
	map_data *levels = (map_data *) map->data;
	map_data *level = &levels[t];
	
	level->cur = level->head;
}

void map_print(map *map, int t, int x0, int y0, int x1, int y1) {
	int width  = x1 - x0;
	int height = y1 - y0;
	
	bool arr[width][height];
	memset(arr, 0, width*height);
	
	map_restart(map, t);
	
	int x, y;
	
	// collect living cells
	while (map_next(map, t, &x, &y)) {
		if (x >= x0 && x <= x1 &&
			y >= y0 && y <= y1   )
		{
			arr[x-x0][y-y0] = true;
		}
	}
	
	// print cells
	printf("t=%d x=%d-%d y=%d-%d:\n", t, x0, x1, y0, y1);

	for (int x = 0; x < width; ++x) {
		for (int y = 0; y < height; ++y) {
			if (arr[x][y])
				printf("X");
			else
				printf(" ");
		}
		
		printf("\n");
	}
}

