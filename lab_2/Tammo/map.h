#ifndef __MAP_H
#define __MAP_H

// the chanche that a initialized cell is alived is 1:density
#define DENSITY 4

#include <time.h>
#include <stdbool.h>

typedef struct cell
{
	int y;
	int x;
	struct cell* next;
} cell_t;

typedef struct map
{
	cell_t* head;
	cell_t* tail;
	cell_t* curr;
	
	int height;
	int width;
} map_t;

void map_init(map_t* map, int height, int width);
void map_add(map_t* map, int y, int x);
void map_fill_random(map_t* map);
cell_t* map_get_next(map_t* map);
void map_print(map_t* map);

#endif
