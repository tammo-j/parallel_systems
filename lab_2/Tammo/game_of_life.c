#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
//#include <mpi.h>

#include "map.h"

void calc_next_tick(map_t* map);

int main(int argc, char** argv)
{
	/* init map */

	map_t* map = calloc(1, sizeof(map_t));
	map_init(map,16, 16);
	map_fill_random(map);

	
	/* distribute map */


	map_t* map_1 = calloc(1, sizeof(map_t));
	map_t* map_2 = calloc(1, sizeof(map_t));
	map_t* map_3 = calloc(1, sizeof(map_t));
	map_t* map_4 = calloc(1, sizeof(map_t));
	map_init(map_1, 4, 4);
	map_init(map_2, 4, 4);
	map_init(map_3, 4, 4);
	map_init(map_4, 4, 4);
	for(cell_t* cell_i = map_get_next(map); cell_i != NULL; cell_i = map_get_next(map))
	{
		int segment = 1;
		if(cell_i->y > 8)
			segment = 3;
		if(cell_i->y < 8)
			segment += 1;

		switch(segment)
		{
			case 1:
				map_add(map_1, cell_i->y, cell_i->x);
				break;
			case 2:
				map_add(map_2, cell_i->y, cell_i->x);
				break;
			case 3:
				map_add(map_3, cell_i->y, cell_i->x);
				break;
			case 4:
				map_add(map_4, cell_i->y, cell_i->x);
				break;
			default:
				printf("Error by partitioning!");
				break;
		}
	}
	
	while(true)
	{
		map_print(map);
		calc_next_tick(map);
		sleep(1);
	}

	return EXIT_SUCCESS;
}

/* Adjust the rules of conway's game-of-life. */
void calc_next_tick(map_t* map)
{
	int  map_count[map->height+2][map->width+2]; // added a border
	memset(map_count, 0, sizeof(map_count));
	
	// Count the neighboring cells.
	for(cell_t* cell_i = map_get_next(map); cell_i != NULL; cell_i = map_get_next(map))
	{
		int y = cell_i->y+1;
		int x = cell_i->x+1;

		map_count[y][x+1] += 1;
		map_count[y][x-1] += 1;
		map_count[y+1][x] += 1;
		map_count[y+1][x+1] += 1;
		map_count[y+1][x-1] += 1;
		map_count[y-1][x] += 1;
		map_count[y-1][x+1] += 1;
		map_count[y-1][x-1] += 1;
	}
	// Don't consider the extra border around!
	memset(map_count[0], 0, sizeof(map_count[0]));
	for(int y = 1; y < map->height+2; y++)
	{
		map_count[y][0] =  0;
		map_count[y][map->width+1] =  0;
	}
	memset(map_count[map->height+1], 0, sizeof(map_count[map->height+1]));
		
	map_t* map_new = calloc(1, sizeof(map_t));
	map_init(map_new, map->height, map->width);

	for(cell_t* cell_i = map_get_next(map); cell_i != NULL; cell_i = map_get_next(map))
	{
		int y = cell_i->y+1;
		int x = cell_i->x+1;

		for(int dy = -1; dy < 2; dy++)
		{
			for(int dx = -1; dx < 2; dx++)
			{
				int neighbors = map_count[y+dy][x+dx];
				
				// Is this cell considered already?
				if(neighbors < 0)
					continue;
				
				// Is this cell newborned or alived?
				if((dy == 0 && dx == 0 && neighbors == 2) || (neighbors == 3))
				{
					map_add(map_new, y+dy-1, x+dx-1);
					map_count[y+dy][x+dx] = -1;
				}
			}
		}
	}
	// TODO: free map first?
	*map = *map_new;
}
