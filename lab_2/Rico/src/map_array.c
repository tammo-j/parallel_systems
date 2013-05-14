#include "map.h"

#include <stdlib.h>
#include <string.h>


struct map_data_t {
	int blocks; // blocks per column
	void *cells;
};

typedef int block;

#define BLOCK_SIZE 4*sizeof(block)

void map_init(map *map, int duration, int width, int height) {
	map->duration = duration;
	map->width = width;
	map->height = height;
	map->data = malloc(sizeof(map_data));
	
	int blocks = height / BLOCK_SIZE;
	if (height % BLOCK_SIZE > 0)
		++blocks;

	size_t num = duration*width*blocks;
	
	map->data->blocks = blocks;
	map->data->cells = calloc(num, sizeof(block));
	
	memset(map->data->cells, 0, num * sizeof(block));
}

void map_free(map *map) {
	free(map->data->cells);
	free(map->data);
}

void map_set(map *map, int t, int x, int y, bool status) {
/*	printf("set t=%d x=%d y=%d ", t, x, y);*/
	
	block (*arr)[map->width][map->data->blocks] = map->data->cells;
	
	int b = y / BLOCK_SIZE;
	int bit = y % BLOCK_SIZE;

	if (status)
		arr[t][x][b] |=   (block) 1 << bit;  // map_set bit
	else
		arr[t][x][b] &= ~((block) 1 << bit); // clear bit
}

bool map_get(map *map, int t, int x, int y) {
/*	printf("get t=%d x=%d y=%d ", t, x, y); */
	
/*	if (t < 0 || t >= map->duration ||*/
/*		x < 0 || x >= map->width    ||*/
/*		y < 0 || y >= map->height     )*/
/*	{*/
/*		return false;*/
/*	}*/
	
	block (*arr)[map->width][map->data->blocks] = map->data->cells;
	
	int b = y / BLOCK_SIZE;
	int bit = y % BLOCK_SIZE;
	
	return arr[t][x][b] & ((block) 1 << bit);
}

