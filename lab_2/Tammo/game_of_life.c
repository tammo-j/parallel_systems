#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <mpi.h>

void print_map(map_t* map);

int main(int argc, char** argv)
{
	/* init map */
	
	map_t* map = calloc(1, sizeof(map_t));
	cell_t cell = {0, 0};
	map->cell = cell;
	map->next = NULL;
	
	map_t* last = map;

	srand(time(NULL));
	for(int x = 1; x < WIDTH; x++)
	{
		for(int y = 0; y < HEIGHT; y++)
		{
			if(0 < (rand() % DENSITY))
				continue;
			
			map_t* map_i = calloc(1, sizeof(map_t));
			cell_t cell = {x, y};
			map_i->cell = cell;
			map_i->next = NULL;
			
			last->next = map_i;
			last = map_i;
		}
	}

	print_map(map);

	/* calculate next tick */

	int  map_count[WIDTH+2][HEIGHT+2]; // added a border
	memset(map_count, 0, sizeof(map_count));
	
	map_t* map_i = NULL;
	
	// count neighboring cells
	map_i = map;
	while(map_i)
	{
		int x = map_i->cell.x;
		int y = map_i->cell.y;

		map_count[x+1][y] += 1;
		map_count[x-1][y] += 1;
		map_count[x][y+1] += 1;
		map_count[x][y-1] += 1;
		map_count[x+1][y+1] += 1;
		map_count[x+1][y-1] += 1;
		map_count[x-1][y+1] += 1;
		map_count[x-1][y-1] += 1;

		map_i = map_i->next;
	}
	
	// Adjust the rules of conway's game-of-life.
	
	map_t* map_new = calloc(1, sizeof(map_t));
	map_t* map_new_last = map_new;

	// foreach cell in current map
	map_t* map_cur = map;
	while(map_cur)
	{
		for(int dx = -1; dx < 2; dx++)
		{
			for(int dy = -1; dy < 2; dy++)
			{
				int x = map_cur->cell.x;
				int y = map_cur->cell.y;

				int neighbors = map_count[x+dx][y+dy];
				
				// Is this cell considered already?
				if(neighbors < 0)
					continue;

				map_count[x+dx][y+dy] = -1;
				
				// Is this cell newborned or alived?
				if(neighbors == 3 || (dx == 0 && dy == 0 && neighbors == 2))
				{
					map_t* map_new_i = calloc(1, sizeof(map_t));
					cell_t cell = {x, y};
					map_new_i->cell = cell;
					map_new_i->next = NULL;

					map_new_last->next = map_new_i;

					//printf("New alived in (%i, %i).\n", x+dx, y+dy);
				}
			}
		}
		map_cur = map_cur->next;
	}

	// The first element was a dummy.
	map_new = map_new->next;

	print_map(map_new);

	return EXIT_SUCCESS;
}

void print_map(map_t* map)
{
	/*
	for(int x = 0; x < 10; x++)
		printf("%i", x);
	for(int x = 10; x < WIDTH; x++)
		printf("%i", x/10);
	printf("\n");
	for(int x = 0; x < 10; x++)
		printf(" ");
	for(int x = 10; x < WIDTH; x++)
		printf("%i", x%10);
	printf("\n");
	for(int x = 0; x < WIDTH; x++)
		printf("-");
	printf("\n");
	*/
	
	bool cli_arr[WIDTH][HEIGHT]; 
	memset(cli_arr, false, sizeof(cli_arr));

	map_t* map_i = map;
	while(map_i)
	{
		cli_arr[map_i->cell.x][map_i->cell.y] = true;
		map_i = map_i->next;
	}

	for(int y = 0; y < HEIGHT; y++)
	{
		for(int x = 0; x < WIDTH; x++)
		{
			if(cli_arr[x][y] == true)
				printf("X");
			else
				printf(" ");
		}
		printf("\n");
	}
}
