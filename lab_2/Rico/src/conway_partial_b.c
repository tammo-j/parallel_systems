
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "map.h"
#include "conway_algorithm.h"

int main(int argc, char **argv) {
	int width = 50;
	int height = 50;
	int duration = 20;
	
	map world;
	
	map_init(&world, duration, width, height);
	
	for (int x = 14; x <= 34; ++x) {
		for (int y = 14; y <= 34; ++y) {
			map_append(&world, 0, x, y);
		}
	}
	
	map partial_world[5][5];
	
	// distribute
	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 5; ++j) {
			map_init(&partial_world[i][j], duration, 10, 10);
		}
	}
			
	int x, y;
	map_restart(&world, 0);
	while (map_next(&world, 0, &x, &y))
		map_append(&partial_world[x/10][y/10], 0, x%10, y%10);
	
	// calculate
	for (int t = 0; t < duration; ++t) {
		for (int i = 0; i < 5; ++i) {
			for (int j = 0; j < 5; ++j) {
				conway_solve(&partial_world[i][j]);
			}
		}
	}
	
	// merge
	for (int t = 1; t < duration; ++t) {
		for (int i = 0; i < 5; ++i) {
			for (int j = 0; j < 5; ++j) {
				map *cur = &partial_world[i][j];
				
				int x0 = i*10;
				int y0 = j*10;
				
				map_restart(cur, t);
				while (map_next(cur, t, &x, &y))
					map_append(&world, t, x0+x, y0+y);
			}
		}
	}
	
	for (int t = 0; t < duration; ++t)
		map_print(&world, t, 0, 0, width-1, height-1);
		
	map_free(&world);
	
	return EXIT_SUCCESS;
}

