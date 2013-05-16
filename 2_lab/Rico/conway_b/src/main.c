
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <mpi.h>

#include "grid.h"
#include "map.h"
#include "intermap.h"
#include "algorithm.h"
#include "communication.h"


#define ROOT 0 // don't change because of merge offset


static void print_world(map *map);
static void copy_map(map *m1, map *m2, int x0, int y0);

int main(int argc, char **argv) {
	MPI_Init(&argc, &argv);
	
	int rank;
	int size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	// grid size
	int gwidth = 2;
	int gheight = 3;
	
	// individual size
	int nwidth = 25;
	int nheight = 20;
	
	// world size
	int wwidth  = gwidth  * nwidth;
	int wheight = gheight * nheight;
	int wduration = 20;
	
	grid_init(gwidth, gheight);
	comm_init();
	
	int nx, ny;
	grid_get(&nx, &ny);
	
	// create whole field
	map initial;
	map_init(&initial, 1, wwidth+2, wheight+2);
	for (int x = 14; x <= 34; ++x) {
		for (int y = 14; y <= 34; ++y) {
			map_append(&initial, 0, x, y);
		}
	}
	
	map partial;
	intermap inter;
	
	// distribute
	map_init(&partial, wduration, nwidth+2, nheight+2);
	intermap_init(&inter, nwidth+2, nheight+2);
			
	int x, y;
	map_restart(&initial, 0);
	while (map_next(&initial, 0, &x, &y)) {
		int x0 = nx*nwidth;
		int y0 = ny*nheight;
		int x1 = x0 + nwidth  + 1;
		int y1 = y0 + nheight + 1;
		
		if (x > x0 && x < x1 &&
			y > y0 && y < y1)
		{
			map_append(&partial, 0, x-x0, y-y0);
		}
	}
	
	map_free(&initial);
	
	// calculate
	conway_solve(&partial, &inter);
	
	printf("calculation done! %d\n", rank);
	
	MPI_Barrier(MPI_COMM_WORLD);
	
	// share & merge
	if (rank == ROOT) {
		map world;
		map_init(&world, wduration, wwidth, wheight);
		
		// note that the offset is fixed here (no particular reason)
		copy_map(&world, &partial, 0, 0);
		map_free(&partial);
		
		// recv from each node except yourself
		for (int i = 0; i < gwidth*gheight-1 ; ++i) {
			map_init(&partial, wduration, nwidth+2, nheight+2);
			
			int sx, sy;
			comm_recv_partial(&partial, MPI_ANY_SOURCE, &sx, &sy);
			copy_map(&world, &partial, sx*nwidth, sy*nheight);
			map_free(&partial);
		}
		
		print_world(&world);
		
		map_free(&world);
	} else {
		comm_send_partial(&partial, ROOT);
	}
	
	MPI_Finalize();
	
	return EXIT_SUCCESS;
}

void print_world(map *map) {
	for (int t = 0; t < map->duration; ++t)
		map_print(map, t, 1, 1, map->width, map->height);
}

void copy_map(map *dst, map *src, int x0, int y0) {
	for (int t = 0; t < src->duration; ++t) {
		int x, y;
		map_restart(src, t);
		while (map_next(src, t, &x, &y))
			map_append(dst, t, x0+x, y0+y);
	}
}

