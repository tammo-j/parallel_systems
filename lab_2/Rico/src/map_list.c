#include "map_list.h"

#include <stdlib.h>
#include <string.h>

// TODO delete me
#include <stdio.h>


struct map_data_t {
	cell_list *levels;
	intermap inter;
};


static cell_list *map_get_level(map *map, int t);


void map_init(map *map, int duration, int width, int height) {
	map->duration = duration;
	map->width    = width;
	map->height   = height;
	map->data = malloc(sizeof(map_data));
	
	map->data->levels = calloc(duration, sizeof(cell_list));
	intermap_init(&map->data->inter, width*height);
	
	// init lists
	for (int i = 0; i < duration; ++i)
		cell_list_init(&map->data->levels[i]);
}

void map_free(map *map) {
	cell_list *levels = map->data->levels;
	
	for (int t = 0; t < map->duration; ++t)
		cell_list_free(&levels[t]);
	
	intermap_free(&map->data->inter);
	free(map->data->levels);
	free(map->data);
}

intermap *map_get_intermap(map *map) {
	return &map->data->inter;
}

cell_list *map_get_level(map *map, int t) {
	return &map->data->levels[t];
}

void map_fill_intermap(map *map) {
	int x, y;
	map_restart(map, 0);
	while (map_next(map, 0, &x, &y))
		intermap_append(&map->data->inter, map->width, map->height, x, y);
}

void map_list_finalize_border(map *map, int t) {
	cell_list *level = map_get_level(map, t);
	
	intermap_merge_border(&map->data->inter, level);
}

void map_list_finalize_core(map *map, int t) {
	cell_list *level = map_get_level(map, t);
	
	intermap_merge_core(&map->data->inter, level);
}

void map_list_reset_neighbors(map *map) {
	intermap_reset_neighbors(&map->data->inter, map->width * map->height);
}

void map_append(map *map, int t, int x, int y) {
	cell_list *level = map_get_level(map, t);
	
	cell_list_append(level, x, y);
}

bool map_next(map *map, int t, int *x, int *y) {
	cell_list *level = map_get_level(map, t);
	
	return cell_list_next(level, x, y);
}

void map_restart(map *map, int t) {
	cell_list *level = map_get_level(map, t);
	
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

