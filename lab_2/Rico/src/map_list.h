#ifndef MAP_LIST_H
#define MAP_LIST_H


#include "map.h"
#include "cell_list.h"
#include "intermap.h"


void map_list_fill_intermap(map *map);
intermap *map_list_get_intermap(map *map);
void map_list_commit(map *map, int t);
void map_list_empty_touched_oborder(map *map);
void map_list_empty_touched_iborder(map *map);
void map_list_empty_touched_core(map *map);
void map_list_reset_neighbors(map *map);
void map_list_reset_neighbors(map *map);


#endif // MAP_LIST_H
