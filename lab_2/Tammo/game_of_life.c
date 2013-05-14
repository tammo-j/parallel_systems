#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <mpi.h>
#include <math.h>
#include "map.h"

void calc_next_tick(map_t* map, int rank, int size);

int main(int argc, char** argv)
{
	int rank, size;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if(size < 2)
	{
		
		map_t* map = calloc(1, sizeof(map_t));
		map_init(map, 16, 16);
		map_fill_pulsar(map);
		while(true)
		{
			map_print(map);
			calc_next_tick(map, rank, size);
			sleep(1);
			
		}
	} else
	{
		int map_width = (int) ceil((float)16/(float)(size-1));

		if(rank == 0)
		{
			// Initialize the globale map.
			map_t* map = calloc(1, sizeof(map_t));
			map_init(map, map_width*(size-1), 16);
			map_fill_pulsar(map);

			// Distribute the globale map to working processes.
			for(cell_t* cell_i = map_get_next(map); cell_i != NULL; cell_i = map_get_next(map))
			{
				int dot[2] = {cell_i->x, cell_i->y};
				int segment = ((cell_i->x)/map_width)+1;
				MPI_Send(dot, 2, MPI_INT, segment, 0, MPI_COMM_WORLD);
			}
			int dot[2] = {-1, -1};
			for(int i = 1; i < size; i++)
				MPI_Send(dot, 2, MPI_INT, i, 0, MPI_COMM_WORLD);
			
			// Reveive the composed map and print it.
			int count;
			while(true)
			{
				count = 0;
				while(count < size-1)
				{
					MPI_Recv(dot, 2, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					if(dot[0] == -1 && dot[1] == -1)
						count++;
					else
						map_add(map, dot[0], dot[1]);
				}

				map_print(map);
				
				map_free(map);
			}
		} else
		{
			map_t* map = calloc(1, sizeof(map_t));
			map_init(map, map_width, 16);

			// Receive the disributed map from root.
			int dot[2] = {0, 0};
			while(dot[0] != -1 && dot[1] != -1)
			{
				MPI_Recv(dot, 2, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				if(dot[0] != -1 && dot[1] != -1)
					map_add(map, dot[0]%map_width, dot[1]);
			}
			
			// Calculate the map and send it to root.
			while(true)
			{
				int offset = (rank-1)*map_width;
				for(cell_t* cell_i = map_get_next(map); cell_i != NULL; cell_i = map_get_next(map))
				{
					int dot[2] = {offset+cell_i->x, cell_i->y};
					MPI_Send(dot, 2, MPI_INT, 0, 0, MPI_COMM_WORLD);
				}
				int dot[2] = {-1, -1};
				MPI_Send(dot, 2, MPI_INT, 0, 0, MPI_COMM_WORLD);
				
				calc_next_tick(map, rank, size);
				sleep(1);
			}
		}
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

	// Send to left neighbor.
	if(rank > 1)
		MPI_Send(map_border_left,  map->height, MPI_INT, rank-1, 0, MPI_COMM_WORLD);

	// Send to right neighbor.
	if(rank < size-1)
		MPI_Send(map_border_right, map->height, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
}

// Receive borders from neighbors.
void recvBorders(map_t* map, int rank, int size)
{
	int map_border_left[map->height];
	int map_border_right[map->height];

	// Receive from left neighbor.
	if(rank > 1)
	{
		MPI_Recv(map_border_left,  map->height, MPI_INT, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		for(int y = 0; y < map->height; y++)
		{
			if(map_border_left[y] == 1)
				map_add(map, -1 ,y);
		}
	}

	// Receive from right neighbor.
	if(rank < size-1)
	{
		MPI_Recv(map_border_right, map->height, MPI_INT, rank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		for(int y = 0; y < map->height; y++)
		{
			if(map_border_right[y] == 1)
				map_add(map, map->width ,y);
		}
	}
}

// Apply the rules of conway's game-of-life.
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

	// Count the neighboring cells. Added an extra border around.
	int  map_count[map->width+4][map->height+2];
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

	map_free(map);

	*map = *map_new;
}
