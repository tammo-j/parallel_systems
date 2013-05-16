#ifndef CONWAY_ALGORITHM_H
#define CONWAY_ALGORITHM_H

#include "map.h"

// interface
void conway_solve(map *map);

void conway_solve_border(map *map, int t);
void conway_solve_core(map *map, int t);

// implemented in conway_algorith_common
bool conway_future(bool status, int neighbors);


#endif // CONWAY_ALGORITHM_H
