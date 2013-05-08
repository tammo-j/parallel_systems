#ifndef CONWAY_ALGORITHM_H
#define CONWAY_ALGORITHM_H

#include "map.h"

// interface
void conway_solve(map *map);

// implemented in conway_algorith_common
bool conway_future(map *map, int t, int x, int y);


#endif // CONWAY_ALGORITHM_H
