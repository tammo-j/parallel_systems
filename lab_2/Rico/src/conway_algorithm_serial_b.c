#include "conway_algorithm.h"

#include <string.h>


static void tick(map *map, int t);


void conway_solve(map *map) {
	// for each tick
	for (int t = 0; t < map->duration-1; ++t) {
//	for (int t = 0; t < 3; ++t) {
		tick(map, t);
		
		map_print(map, t, 0, 0, map->width-1, map->height-1);
	}
}

void tick(map *map, int t) {
/*	printf("tick %d\n", t); */

	int neighbors[map->width][map->height];
	memset(neighbors, 0, sizeof(neighbors));
	
	int x, y;

	map_restart(map, t);
	while (map_next(map, t, &x, &y)) {
		// for all neighboring cells
		for (int dx = -1; dx <= 1; ++dx) {
			for (int dy = -1; dy <= 1; ++dy) {
				if (dx == 0 && dy == 0)
					continue;
			
				++neighbors[x+dx][y+dy];
			}
		}
	}
	
	// for all live cells
	// check and apply survival rule
	map_restart(map, t);
	while (map_next(map, t, &x, &y)) {
		int n = neighbors[x][y];
		
		if (n >= 2 && n <= 3)
			map_append(map, t+1, x, y);
		
		neighbors[x][y] = 0;
	}
	
	// for all live cells
	map_restart(map, t);
	while (map_next(map, t, &x, &y)) {
		// for dead neighboring cells
		// check and apply birth rule
		for (int dx = -1; dx <= 1; ++dx) {
			for (int dy = -1; dy <= 1; ++dy) {
				if (dx == 0 && dy == 0)
					continue;
				
				int nx = x + dx;
				int ny = y + dy;
			
				int n = neighbors[nx][ny];
		
				if (n == 3)
					map_append(map, t+1, nx, ny);
				
				neighbors[nx][ny] = 0;
			}
		}
	}
		
//	// check condition
//	for (int ix = 0; ix < map->width; ++ix) {
//		for (int iy = 0; iy < map->height; ++iy) {
//			int n = neighbors[ix][iy];
//			
//			if (n == 3 || (cells[ix][iy] && n == 2))
//				map_append(map, t+1, ix, iy);
//		}
//	}
}

