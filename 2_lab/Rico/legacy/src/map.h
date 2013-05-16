#ifndef MAP_H
#define MAP_H

#include <stdbool.h>

typedef struct map_data_t map_data;

typedef struct {
	int duration;
	int width;
	int height;
	map_data *data;
} map;

void map_init(map *map, int duration, int width, int height);
void map_free(map *map);

void map_set(map *map, int t, int x, int y, bool status);
bool map_get(map *map, int t, int x, int y);

void map_append(map *map, int t, int x, int y);
bool map_next(map *map, int t, int *x, int *y);
void map_restart(map *map, int t);

void map_print(map *map, int t, int x0, int y0, int x1, int y1);


#endif // MAP_H
