#ifndef MAP_H
#define MAP_H

#include <stdbool.h>

typedef struct {
	int duration;
	int width;
	int height;
	void *data;
} map;

void map_init(map *map, int duration, int width, int height);
void map_free(map *map);

void map_set(map *map, int t, int x, int y, bool status);
bool map_get(map *map, int t, int x, int y);

void map_print(map *map, int t, int x0, int y0, int x1, int y1);


#endif // MAP_H
