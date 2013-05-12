#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void map_init(map_t* map, int height, int width)
{
	map->head = NULL;
	map->tail = NULL;
	map->curr = NULL;

	map->height = height;
	map->width = width;
}

void map_add(map_t* map, int y, int x)
{	
	// create new cell
	cell_t* cell_i = calloc(1, sizeof(cell_t));
	cell_i->y = y;
	cell_i->x = x;
	cell_i->next = NULL;

	// append to list
	if(map->head == NULL)
	{
		map->head = cell_i;
		map->tail = map->head;
	} else
	{
		map->tail->next = cell_i;
		map->tail = cell_i;
	}
}

void map_fill_random(map_t* map)
{
	map_add(map,1, 3);
	map_add(map,1, 4);
	map_add(map,1, 5);
	map_add(map,1, 9);
	map_add(map,1, 10);
	map_add(map,1, 11);
	map_add(map,6, 3);
	map_add(map,6, 4);
	map_add(map,6, 5);
	map_add(map,6, 9);
	map_add(map,6, 10);
	map_add(map,6, 11);
	map_add(map,8, 3);
	map_add(map,8, 4);
	map_add(map,8, 5);
	map_add(map,8, 9);
	map_add(map,8, 10);
	map_add(map,8, 11);
	map_add(map,13, 3);
	map_add(map,13, 4);
	map_add(map,13, 5);
	map_add(map,13, 9);
	map_add(map,13, 10);
	map_add(map,13, 11);

	map_add(map, 3, 1);
	map_add(map, 4, 1);
	map_add(map, 5, 1);
	map_add(map, 9, 1);
	map_add(map, 10, 1);
	map_add(map, 11, 1);
	map_add(map, 3, 6);
	map_add(map, 4, 6);
	map_add(map, 5, 6);
	map_add(map, 9, 6);
	map_add(map, 10, 6);
	map_add(map, 11, 6);
	map_add(map, 3, 8);
	map_add(map, 4, 8);
	map_add(map, 5, 8);
	map_add(map, 9, 8);
	map_add(map, 10, 8);
	map_add(map, 11, 8);
	map_add(map, 3, 13);
	map_add(map, 4, 13);
	map_add(map, 5, 13);
	map_add(map, 9, 13);
	map_add(map, 10, 13);
	map_add(map, 11, 13);
	return; 

	srand(time(NULL));
	for(int y = 0; y < map->height; y++)
	{
		for(int x = 0; x < map->width; x++)
		{
			if(0 < (rand() % DENSITY))
				continue;
			
			map_add(map, y, x);
		}
	}
}

cell_t* map_get_next(map_t* map)
{
	if(map->curr == NULL)
		map->curr = map->head;
	else
		map->curr = map->curr->next;
	
	return map->curr;
}

void map_print(map_t* map)
{
	for(int x = 0; x < 10; x++)
		printf("%i", x);
	for(int x = 10; x < map->width; x++)
		printf("%i", x/10);
	printf("\n");
	for(int x = 0; x < 10; x++)
		printf(" ");
	for(int x = 10; x < map->width; x++)
		printf("%i", x%10);
	printf("\n");
	for(int x = 0; x < map->width; x++)
		printf("-");
	printf("\n");
	
	bool cli_arr[map->width][map->height]; 
	memset(cli_arr, false, sizeof(cli_arr));

	for(cell_t* cell_i = map_get_next(map); cell_i != NULL; cell_i = map_get_next(map))
	{
		cli_arr[cell_i->x][cell_i->y] = true;
	}

	for(int y = 0; y < map->height; y++)
	{
		for(int x = 0; x < map->width; x++)
		{
			if(cli_arr[x][y] == true)
				printf("%c", 999);
			else
				printf(" ");
		}
		printf("\n");
	}
}
