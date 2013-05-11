#include "conway_algorithm.h"

#include <string.h>


static void tick(map *map, int t);

void conway_solve(map *map) {
	// for each tick
	for (int t = 0; t < map->duration-1; ++t)
		tick(map, t);
}

void conway_solve_border(map *map, int t) {
}

void conway_solve_core(map *map, int t) {
}

void tick(map *world, int t) {
/*	printf("tick %d\n", t);*/

	char neighbors[world->width][world->height];
	memset(neighbors, 0, sizeof(neighbors));
	
	// remembers live cells' neighbors
	map touched;
	map_init(&touched, 1, world->width, world->height);
	
	int x, y;

	// for all live cells
	// notify yourself as neighbors (minesweeper style)
	map_restart(world, t);
	while (map_next(world, t, &x, &y)) {
		// for all neighboring cells
		for (int dx = -1; dx <= 1; ++dx) {
			for (int dy = -1; dy <= 1; ++dy) {
				if (dx == 0 && dy == 0)
					continue;
					
				int nx = x + dx;
				int ny = y + dy;
				
				// don't look beyond the world
				if (nx < 0 || nx >= world->width ||
					ny < 0 || ny >= world->height  )
				{
					continue;
				}
				
				// if untouched
				if (neighbors[nx][ny] == 0)
					map_append(&touched, 0, nx, ny);
			
				++neighbors[nx][ny];
			}
		}
	}
	
	// for all touched cells
	// apply birth rule
	while (map_next(&touched, 0, &x, &y)) {
		// consider touched cells dead
		// this exploits that the birth rule is
		// a partial set of the survival rule
		bool status = conway_future(false, neighbors[x][y]);
		
		if (status) {
			map_append(world, t+1, x, y);
			neighbors[x][y] = 0; // avoid that a cell is appended multiple times
		}
	}
	
	map_free(&touched);
	
	// for all live cells
	// apply survival rule
	map_restart(world, t);
	while (map_next(world, t, &x, &y)) {
		int n = neighbors[x][y];
		
		// if already revived by birth rule
		if (n == 0)
			continue;
		
		bool status = conway_future(true, n);
		
		if (status)
			map_append(world, t+1, x, y);
	}
}
