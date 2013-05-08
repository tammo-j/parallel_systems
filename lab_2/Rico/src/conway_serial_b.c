
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
	
	conway_solve(&world);
	
	for (int t = 0; t < duration; ++t)
		map_print(&world, t, 0, 0, width-1, height-1);
		
	map_free(&world);
	
	return EXIT_SUCCESS;
}

