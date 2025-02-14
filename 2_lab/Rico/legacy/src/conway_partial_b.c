
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "map.h"
#include "map_list.h"
#include "conway_algorithm.h"
#include "conway_neighbor_communication.h"

int main(int argc, char **argv) {
	int width = 52;
	int height = 52;
	int duration = 20;
	
	comm_init(duration, 5, 5);
	
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
			map_init(&partial_world[i][j], duration, 12, 12);
		}
	}
			
	int x, y;
	map_restart(&world, 0);
	while (map_next(&world, 0, &x, &y))
		map_append(&partial_world[(x-1)/10][(y-1)/10], 0, (x-1)%10+1, (y-1)%10+1);
	
	
	
	// calculate
	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 5; ++j) {
			map_list_fill_intermap(&partial_world[i][j]);
		}
	}
	for (int t = 0; t < duration-1; ++t) {
		for (int i = 0; i < 5; ++i) {
			for (int j = 0; j < 5; ++j) {
				comm_set_current_node(i, j);
				printf("now %d:%d:%d\n", t, i, j);
				conway_solve_border(&partial_world[i][j], t);
			}
		}
		for (int i = 0; i < 5; ++i) {
			for (int j = 0; j < 5; ++j) {
				comm_set_current_node(i, j);
				printf("now %d:%d:%d\n", t, i, j);
				conway_solve_core(&partial_world[i][j], t);
			}
		}
	}
	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 5; ++j) {
			map_list_commit(&partial_world[i][j], duration-1);
		}
	}
	
	map merge_world;
	map_init(&merge_world, duration, width, height);
	
	// merge
	for (int t = 0; t < duration; ++t) {
		for (int i = 0; i < 5; ++i) {
			for (int j = 0; j < 5; ++j) {
				map *cur = &partial_world[i][j];
				
				int x0 = i*10;
				int y0 = j*10;
				
				map_restart(cur, t);
				while (map_next(cur, t, &x, &y))
					map_append(&merge_world, t, x0+x, y0+y);
			}
		}
	}
	
	// print
	for (int t = 0; t < duration; ++t)
		map_print(&merge_world, t, 0, 0, width-1, height-1);
	
	map_free(&merge_world);
		
	map_free(&world);
	
	return EXIT_SUCCESS;
}

