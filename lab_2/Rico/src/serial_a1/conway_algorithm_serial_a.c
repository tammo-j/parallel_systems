#include "conway_algorithm.h"


static void tick(map *map, int t);


void conway_solve(map *map) {
	// for each tick
	for (int t = 0; t < map->duration-1; ++t) {
		tick(map, t);
		
		map_print(map, t, 0, 0, map->width-1, map->height-1);
	}
}

void tick(map *map, int t) {
/*	printf("tick %d\n", t); */
	
	// for each cell
	for (int x = 0; x < map->width; ++x) {
		for (int y = 0; y < map->height; ++y) {
			bool status = conway_future(map, t, x, y);

			map_set(map, t+1, x, y, status);
		}
	}
}

