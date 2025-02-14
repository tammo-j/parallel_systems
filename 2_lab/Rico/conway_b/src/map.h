#ifndef MAP_LIST_H
#define MAP_LIST_H


#include "cell_list.h"


typedef struct {
	int duration;
	int width;
	int height;
	
	cell_list *levels;
} map;

void map_init(map *map, int duration, int width, int height);
void map_free(map *map);

cell_list *map_get_level(map *map, int t);

int map_count(map *map, int t);
void map_append(map *map, int t, int x, int y);
bool map_next(map *map, int t, int *x, int *y);
void map_restart(map *map, int t);

void map_print(map *map, int t, int x0, int y0, int x1, int y1);

//void map_list_fill_intermap(map *map);
//intermap *map_list_get_intermap(map *map);
//void map_list_commit(map *map, int t);
//void map_list_empty_touched_oborder(map *map);
//void map_list_empty_touched_iborder(map *map);
//void map_list_empty_touched_core(map *map);
//void map_list_reset_neighbors(map *map);
//void map_list_reset_neighbors(map *map);


#endif // MAP_LIST_H
