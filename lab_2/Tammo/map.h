#ifndef __MAP_H
#define __MAP_H

// the chanche that a initialized cell is alived is 1:density
#define DENSITY 4

#include <time.h>
#include <stdbool.h>

typedef struct cell
{
	int x;
	int y;
	struct cell* next;
} cell_t;

typedef struct map
{
	cell_t* head;
	cell_t* tail;
	cell_t* curr;
	
	int width;
	int height;
} map_t;

void map_init(map_t* map, int width, int height);
void map_free(map_t* map);
void map_add(map_t* map, int x, int y);
void map_fill_random(map_t* map);
void map_fill_pulsar(map_t* map);
cell_t* map_get_next(map_t* map);
void map_print(map_t* map);
void map_print2(map_t* map, int xoffset);

#endif
