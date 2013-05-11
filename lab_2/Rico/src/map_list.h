#ifndef MAP_LIST_H
#define MAP_LIST_H


#include "map.h"
#include "cell_list.h"
#include "intermap.h"


void map_fill_intermap(map *map);
intermap *map_get_intermap(map *map);
void map_list_finalize_border(map *map, int t);
void map_list_finalize_core(map *map, int t);
void map_list_reset_neighbors(map *map);


#endif // MAP_LIST_H
