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
	map_init(map_1, 8, 8);
	map_init(map_2, 8, 8);
	map_init(map_3, 8, 8);
	map_init(map_4, 8, 8);
	for(cell_t* cell_i = map_get_next(map); cell_i != NULL; cell_i = map_get_next(map))
	{
		int y = cell_i->y;
		int x = cell_i->x;

		// 1|3
		// -+-
		// 2|4
		int segment = 1;
		if(cell_i->y > 7)
			segment = 2;
		if(cell_i->x > 7)
			segment += 2;

		x %= 8;
		y %= 8;

		switch(segment)
		{
			case 1:
				map_add(map_1, y, x);
				break;
			case 2:
				map_add(map_2, y, x);
				break;
			case 3:
				map_add(map_3, y, x);
				break;
			case 4:
				map_add(map_4, y, x);
				break;
			default:
				printf("Error by partitioning!");
				break;
		}
	}
	
	while(true)
	{
		map_print(map_1);
		calc_next_tick(map_1);
		//calc_next_tick(map_2);
		//calc_next_tick(map_3);
		//calc_next_tick(map_4);
		sleep(1);
	}

	return EXIT_SUCCESS;
}

/* Adjust the rules of conway's game-of-life. */
void calc_next_tick(map_t* map)
{
	int  map_count[map->height+2][map->width+2]; // added an extra border around
	memset(map_count, 0, sizeof(map_count));

	// Receive borders from neighbors.
	// TODO:

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
		map_count[y][0] = 0;
		map_count[y][map->width+1] = 0;
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
				if(neighbors < 1)
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
	*map = *map_new;
}
