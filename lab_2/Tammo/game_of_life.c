#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <mpi.h>

#define HEIGHT 30
#define WIDTH  50
// the chanche that a initialized cell is alived is 1:density
#define DENSITY 6

typedef struct
{
	int x;
	int y;
	bool alived;
} cell_t;

typedef struct map
{
	cell_t cell;
	struct map* next;
} map_t;

int main(int argc, char** argv)
{
	/* init map */
	
	map_t* map = calloc(1, sizeof(map_t));
	cell_t cell = {0, 0, true};
	map->cell = cell;
	map->next = NULL;
	
	map_t* last = map;

	for(int y = 0; y < HEIGHT; y++)
	{
		for(int x = 1; x < WIDTH; x++)
		{
			if(0 < (rand() % DENSITY))
				continue;
			
			map_t* map_i = calloc(1, sizeof(map_t));
			cell_t cell = {x, y, true};
			map_i->cell = cell;
			map_i->next = NULL;
			
			last->next = map_i;
			last = map_i;
		}
	}
	
	/* manage suitable map section */
	
	map_t* map_i = map;

	// y axis
	for(int y = 0; y < HEIGHT; y++)
	{
		if(map_i->cell.y != y)
		{
			printf("\n");
			continue;
		}

		// x axis
		for(int x = 0; x < WIDTH; x++)
		{
			if(map_i->cell.x != x)
			{
				printf(" ");
				continue;
			}
			printf("X");

			map_i = map_i->next;
			if(map_i == NULL || map_i->cell.y != y)
				break;
		}

		printf("\n");
	}

	return EXIT_SUCCESS;
}
