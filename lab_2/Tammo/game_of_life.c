#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <mpi.h>
#include "map.h"

void calc_next_tick(map_t* map, int rank, int size);

int main(int argc, char** argv)
{
	int rank, size;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	map_t* map;

	// init map
	map = calloc(1, sizeof(map_t));
	map_init(map, 16/size, 16);
	map_fill_random(map);
	
	// distribute map
	while(true)
	{
		printf("RANK: %i\n", rank);
		map_print(map);
		calc_next_tick(map, rank, size);
		sleep(1);
	}

	MPI_Finalize();

	return EXIT_SUCCESS;
}

// Send borders to neighbors.
void sendBorders(map_t* map, int rank, int size)
{
	int map_border_left[map->height];
	int map_border_right[map->height];
	memset(map_border_left,  0, sizeof(map_border_left));
	memset(map_border_right, 0, sizeof(map_border_right));

	for(cell_t* cell_i = map_get_next(map); cell_i != NULL; cell_i = map_get_next(map))
	{
		int y = cell_i->y;
		int x = cell_i->x;

		if(x == 0)
			map_border_left[y] = 1;
		if(x == map->width-1)
			map_border_right[y] = 1;
	}

	if(rank > 0)
		MPI_Send(map_border_left,  map->height, MPI_INT, rank-1, 0, MPI_COMM_WORLD);

	if(rank < size-2)
		MPI_Send(map_border_right, map->height, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
}

// Receive borders from neighbors.
void recvBorders(map_t* map, int rank, int size)
{
	int map_border_left[map->height];
	int map_border_right[map->height];

	if(rank > 0)
	{
		MPI_Recv(map_border_left,  map->height, MPI_INT, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		for(int y = 0; y < map->height; y++)
		{
			if(map_border_left[y] == 1)
				map_add(map, -1 ,y);
		}
	}
	if(rank < size-2)
	{
		MPI_Recv(map_border_right, map->height, MPI_INT, rank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		for(int y = 0; y < map->height; y++)
		{
			if(map_border_left[y] == 1)
				map_add(map, map->width ,y);
		}
	}
}

// Adjust the rules of conway's game-of-life.
void calc_next_tick(map_t* map, int rank, int size)
{

	if(rank % 2 == 0)
	{
		sendBorders(map, rank, size);
		recvBorders(map, rank, size);
	} else
	{
		recvBorders(map, rank, size);
		sendBorders(map, rank, size);
	}

	// Count the neighboring cells.
	int  map_count[map->width+4][map->height+2]; // added an extra border around
	memset(map_count, 0, sizeof(map_count));

	for(cell_t* cell_i = map_get_next(map); cell_i != NULL; cell_i = map_get_next(map))
	{
		int x = cell_i->x+2;
		int y = cell_i->y+1;

		map_count[x][y+1]   += 1;
		map_count[x][y-1]   += 1;
		map_count[x+1][y]   += 1;
		map_count[x+1][y+1] += 1;
		map_count[x+1][y-1] += 1;
		map_count[x-1][y]   += 1;
		map_count[x-1][y+1] += 1;
		map_count[x-1][y-1] += 1;
	}

	// Don't consider the extra border around!
	memset(map_count[0], 0, sizeof(map_count[0]));
	memset(map_count[1], 0, sizeof(map_count[1]));
	memset(map_count[map->width+2], 0, sizeof(map_count[map->width+2]));
	memset(map_count[map->width+3], 0, sizeof(map_count[map->width+3]));
	for(int x = 0; x < map->width+4; x++)
	{
		map_count[x][0] = 0;
		map_count[x][map->height+1] = 0;
	}
		
	map_t* map_new = calloc(1, sizeof(map_t));
	map_init(map_new, map->width, map->height);

	for(cell_t* cell_i = map_get_next(map); cell_i != NULL; cell_i = map_get_next(map))
	{
		int x = cell_i->x+2;
		int y = cell_i->y+1;

		for(int dx = -1; dx < 2; dx++)
		{
			for(int dy = -1; dy < 2; dy++)
			{
				int neighbors = map_count[x+dx][y+dy];
				
				// Is this cell considered already?
				if(neighbors < 1)
					continue;
				
				// Is this cell newborned or alived?
				if((dx == 0 && dy == 0 && neighbors == 2) || (neighbors == 3))
				{
					map_add(map_new, x+dx-2, y+dy-1);
					map_count[x+dx][y+dy] = -1;
				}
			}
		}
	}
	// FIXME: free(map)
	*map = *map_new;
}
