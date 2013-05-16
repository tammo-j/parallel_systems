#include "grid.h"

#include <mpi.h>


static int grid_width;
static int grid_height;


void grid_init(int width, int height) {
	grid_width = width;
	grid_height = height;
}

void grid_get(int *x, int *y) {
	int id;
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	
	grid_id2grid(id, x, y);
}


int grid_neighbors() {
	int x, y;
	grid_get(&x, &y);
	
	int n = -1;
	for (int dx = -1; dx <= 1; ++dx) {
		for (int dy = -1; dy <= 1; ++dy) {
			if (x+dx >= 0 && x+dx < grid_width &&
				y+dy >= 0 && y+dy < grid_height)
			{
				++n;
			}
		}
	}
	
	return n;
}

void grid_id2grid(int id, int *x, int *y) {
	*x = id % grid_width;
	*y = id / grid_width;
}

int grid_grid2id(int x, int y) {
	return x + y*grid_width;
}

int grid_dir2id(int dir) {
	int x, y;
	grid_get(&x, &y);
	
	switch (dir) {
		case GRID_TOP:                  y -= 1; break;
		case GRID_BOTTOM:               y += 1; break;
		case GRID_LEFT:         x -= 1;         break;
		case GRID_RIGHT:        x += 1;         break;
		case GRID_TOP_LEFT:     x -= 1; y -= 1; break;
		case GRID_TOP_RIGHT:    x += 1; y -= 1; break;
		case GRID_BOTTOM_LEFT:  x -= 1; y += 1; break;
		case GRID_BOTTOM_RIGHT: x += 1; y += 1; break;
	}
	
	if (x < 0 || x >= grid_width ||
		y < 0 || y >= grid_height)
	{
		return -1;
	}
	
	return grid_grid2id(x, y);
}

int grid_swap_dir(int dir) {
	switch (dir) {
		case GRID_TOP:          return GRID_BOTTOM;
		case GRID_BOTTOM:       return GRID_TOP;
		case GRID_LEFT:         return GRID_RIGHT;
		case GRID_RIGHT:        return GRID_LEFT;
		case GRID_TOP_LEFT:     return GRID_BOTTOM_RIGHT;
		case GRID_TOP_RIGHT:    return GRID_BOTTOM_LEFT;
		case GRID_BOTTOM_LEFT:  return GRID_TOP_RIGHT;
		case GRID_BOTTOM_RIGHT: return GRID_TOP_LEFT;
		default:                return -1;
	}
}

int grid_id2dir(int id) {
	int x1, y1, x2, y2;
	grid_get(&x1, &y1);
	grid_id2grid(id, &x2, &y2);
	
	int dx = x2 - x1;
	int dy = y2 - y1;
	
	if (dx == -1) {        // left with corners
		if (dy == -1)      // top left corner
			return GRID_TOP_LEFT;
		else if (dy == 1)  // bottom left corner
			return GRID_BOTTOM_LEFT;
		else               // left
			return GRID_LEFT;
	} else if (dx == 1) {  // right with corners
		if (dy == -1)      // top right corner
			return GRID_TOP_RIGHT;
		else if (dy == 1)  // bottom right corner
			return GRID_BOTTOM_RIGHT;
		else               // right
			return GRID_RIGHT;
	} else if (dy == -1) { // top without corners
		return GRID_TOP;
	} else {               // bottom without corners
		return GRID_BOTTOM;
	}
}

