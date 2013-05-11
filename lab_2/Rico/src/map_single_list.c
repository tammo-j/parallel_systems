#include "map.h"
#include "cell_list.h"

#include <stdlib.h>
#include <string.h>

// TODO delete me
#include <stdio.h>


void map_init(map *map, int duration, int width, int height) {
	map->duration = duration;
	map->width    = width;
	map->height   = height;
	
	cell_list *levels = calloc(duration, sizeof(cell_list));
	
	map->data = levels;
	
	// init lists
	for (int i = 0; i < duration; ++i)
		list_init(&levels[i]);
}

void map_free(map *map) {
	cell_list *levels = (cell_list *) map->data;
	
	for (int t = 0; t < map->duration; ++t)
		list_free(&levels[i]);
	
	// free levels
	free(map->data);
}

void map_append(map *map, int t, int x, int y) {
	cell_list *levels = (cell_list *) map->data;
	cell_list *level = &levels[t];
	
	cell_list_append(level, x, y);
}

bool map_next(map *map, int t, int *x, int *y) {
	cell_list *levels = (cell_list *) map->data;
	cell_list *level = &levels[t];
	
	return cell_list_next(level, x, y);
}

void map_restart(map *map, int t) {
	cell_list *levels = (cell_list *) map->data;
	cell_list *level = &levels[t];
	
	cell_list_restart(level);
}

void map_print(map *map, int t, int x0, int y0, int x1, int y1) {
	int width  = x1 - x0;
	int height = y1 - y0;
	
	bool arr[width][height];
	memset(arr, 0, width*height);
	
	int x, y;
	
	// collect living cells
	map_restart(map, t);
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

