#include "conway_algorithm.h"


static void tick(map *map, int t);
static int count_neighbors(map *map, int t, int x, int y);
static void calc_cell(map *map, int t, int x, int y);


void conway_solve(map *map) {
	// for each tick
	for (int t = 0; t < map->duration-1; ++t)
		tick(map, t);
}

void tick(map *map, int t) {
/*	printf("tick %d\n", t); */

	conway_solve_border(map, t+1);
	conway_solve_core(map, t+1);
}

void conway_solve_core(map *map, int t) {
	for (int x = 2; x < map->width-2; ++x) {
		for (int y = 2; y < map->height-2; ++y) {
			calc_cell(map, t, x, y);
		}
	}
}

void conway_solve_border(map *map, int t) {
	// inner border area
	int top    = 1;
	int bottom = map->height - 2;
	int left   = 1;
	int right  = map->width - 2;
	
	// horizontal borders with corners
	for (int x = left; x <= right; ++x) {
		calc_cell(map, t, x, top   ); // top border
		calc_cell(map, t, x, bottom); // bottom border
	}
	
	// vertical borders without corners
	for (int y = top+1; y <= bottom-1; ++y) {
		calc_cell(map, t, left , y); // left border
		calc_cell(map, t, right, y); // right border
	}
}

void calc_cell(map *map, int t, int x, int y) {
	int  n      = count_neighbors(map, t-1, x, y);
	bool now    = map_get(map, t-1, x, y);
	bool future = conway_future(now, n);

	map_set(map, t, x, y, future);
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
