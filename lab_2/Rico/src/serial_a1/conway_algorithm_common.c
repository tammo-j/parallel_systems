#include "conway_algorithm.h"

bool conway_future(map *map, int t, int x, int y) {
	int neighbors = 0;
	
	// for all neighboring cells
	for (int dx = -1; dx <= 1; ++dx) {
		for (int dy = -1; dy <= 1; ++dy) {
			if (dx == 0 && dy == 0)
				continue;
			
			if (map_get(map, t, x+dx, y+dy))
				++neighbors;
		}
	}
	
	// if cell is live
	if (map_get(map, t, x, y)) {
		// stay live if their are two to three neighbors
		return neighbors >= 2 && neighbors <= 3;
	} else {
		// be born when there are exactly three neighbors
		return neighbors == 3;
	}
}

