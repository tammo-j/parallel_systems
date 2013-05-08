#include "conway_algorithm.h"

#include <string.h>


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

	char neighbors[map->width][map->height];
	memset(neighbors, 0, sizeof(neighbors));
	
	int x, y;

	// for all live cells
	// notify yourself as neighbors (minesweeper style)
	map_restart(map, t);
	while (map_next(map, t, &x, &y)) {
		// for all neighboring cells
		for (int dx = -1; dx <= 1; ++dx) {
			for (int dy = -1; dy <= 1; ++dy) {
				if (dx == 0 && dy == 0)
					continue;
					
				int nx = x + dx;
				int ny = y + dy;
				
				if (nx < 0 || nx >= map->width ||
					ny < 0 || ny >= map->height  )
				{
					continue;
				}
			
				++neighbors[nx][ny];
			}
		}
	}
	
	// for all live and neighboring cells
	// check and apply birth and survival rules
	map_restart(map, t);
	while (map_next(map, t, &x, &y)) {
		for (int dx = -1; dx <= 1; ++dx) {
			for (int dy = -1; dy <= 1; ++dy) {
				int nx = x + dx;
				int ny = y + dy;
				int n = neighbors[nx][ny];
				
				// only the current live cell is considered live
				// this exploits that the birth rule is
				// a partial set of the survival rule
				bool now = dx == 0 && dy == 0;
				bool future = conway_future(now, n);
				
				if (future) {
					map_append(map, t+1, nx, ny);
					neighbors[nx][ny] = 0; // avoid that a cell is appended multiple times
				}
			}
		}
	}
}
