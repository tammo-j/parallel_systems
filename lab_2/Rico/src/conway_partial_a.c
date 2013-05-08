
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "map.h"
#include "conway_algorithm.h"

int main(int argc, char **argv) {
	int width = 52;
	int height = 52;
	int duration = 20;
	
	map world;
	
	map_init(&world, duration, width, height);
	
	for (int x = 14; x <= 34; ++x) {
		for (int y = 14; y <= 34; ++y) {
			map_set(&world, 0, x, y, true);
		}
	}
	
	map submap[5][5];
	
	// distribute
	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 5; ++j) {
			map *curmap = &submap[i][j];
			
			map_init(curmap, duration, 12, 12);
			
			int x0 = i*10;
			int y0 = j*10;
			
			for (int x = 0; x < 12; ++x) {
				for (int y = 0; y < 12; ++y) {
					bool status = map_get(&world, 0, x0+x, y0+y);
					map_set(curmap, 0, x, y, status);
				}
			}
		}
	}
	
	map_print(&submap[2][2], 0, 0, 0, 11, 11);
	
	// calculate
	for (int t = 1; t < duration; ++t) {
		// calculate border
		for (int i = 0; i < 5; ++i) {
			for (int j = 0; j < 5; ++j) {
				conway_solve_border(&submap[i][j], t);
			}
		}
		
		// exchange information
		for (int i = 0; i < 5; ++i) {
			for (int j = 0; j < 5; ++j) {
				// exchange with right node
				if (i < 4) {
					for (int y = 1; y <= 10; ++y) {
						bool lstatus = map_get(&submap[i  ][j], t, 10, y);
						bool rstatus = map_get(&submap[i+1][j], t,  1, y);
						
						map_set(&submap[i  ][j], t, 11, y, rstatus);
						map_set(&submap[i+1][j], t,  0, y, lstatus);
					}
				}
				
				// exchange with bottom node
				if (j < 4) {
					for (int x = 1; x <= 10; ++x) {
						bool tstatus = map_get(&submap[i][j  ], t, x, 10);
						bool bstatus = map_get(&submap[i][j+1], t, x,  1);
						
						map_set(&submap[i][j  ], t, x, 11, bstatus);
						map_set(&submap[i][j+1], t, x,  0, tstatus);
					}
				}
				
				// exchange with bottom right node
				if (i < 4 && j < 4) {
					bool tlstatus = map_get(&submap[i  ][j  ], t, 10, 10);
					bool brstatus = map_get(&submap[i+1][j+1], t,  1,  1);
					
					map_set(&submap[i  ][j  ], t, 11, 11, brstatus);
					map_set(&submap[i+1][j+1], t,  0,  0, tlstatus);
				}
				
				// exchange with top right node
				if (i < 4 && j > 0) {
					bool blstatus = map_get(&submap[i  ][j  ], t, 10,  1);
					bool trstatus = map_get(&submap[i+1][j-1], t,  1, 10);
					
					map_set(&submap[i  ][j  ], t, 11,  0, trstatus);
					map_set(&submap[i+1][j-1], t,  0, 11, blstatus);
				}
			}
		}
		
		// calculate core
		for (int i = 0; i < 5; ++i) {
			for (int j = 0; j < 5; ++j) {
				conway_solve_core(&submap[i][j], t);
			}
		}
	}
	
	// merge
	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 5; ++j) {
			map *curmap = &submap[i][j];
			
			int x0 = i*10;
			int y0 = j*10;
			
			for (int t = 1; t < duration; ++t) {
				for (int x = 1; x <= 10; ++x) {
					for (int y = 1; y <= 10; ++y) {
						bool status = map_get(curmap, t, x, y);
						map_set(&world, t, x0+x, y0+y, status);
					}
				}
			}
		}
	}
	
	// print
	for (int t = 0; t < duration; ++t)
		map_print(&world, t, 0, 0, width-1, height-1);
	
	// free
	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 5; ++j) {
			map_free(&submap[i][j]);
		}
	}
	
	map_free(&world);
	
	return EXIT_SUCCESS;
}

