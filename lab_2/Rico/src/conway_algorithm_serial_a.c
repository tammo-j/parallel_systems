#include "conway_algorithm.h"


static void tick(map *map, int t);
static int count_neighbors(map *map, int t, int x, int y);


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
	// apply rules of birth and survival
	for (int x = 0; x < map->width; ++x) {
		for (int y = 0; y < map->height; ++y) {
			int  n      = count_neighbors(map, t, x, y);
			bool now    = map_get(map, t, x, y);
			bool future = conway_future(now, n);

			map_set(map, t+1, x, y, future);
		}
	}
}

int count_neighbors(map *map, int t, int x, int y) {
	int neighbors = 0;
	
	// for all neighboring cells
	for (int dx = -1; dx <= 1; ++dx) {
		for (int dy = -1; dy <= 1; ++dy) {
			if (dx == 0 && dy == 0)
				continue;
			
			if (map_get(map, t, x+dx, y+dy))
				++neighbors;
		}
	}
	
	return neighbors;
}

