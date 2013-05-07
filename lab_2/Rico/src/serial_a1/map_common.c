#include "map.h"

#include <stdio.h>

void map_print(map *map, int t, int x0, int y0, int x1, int y1) {
	printf("t=%d x=%d-%d y=%d-%d:\n", t, x0, x1, y0, y1);

	for (int x = x0; x <= x1; ++x) {
		for (int y = y0; y <= y1; ++y) {
			if (map_get(map, t, x, y))
				printf("X");
			else
				printf(" ");
		}
		
		printf("\n");
	}
}

