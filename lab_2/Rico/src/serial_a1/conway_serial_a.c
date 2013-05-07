
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>


typedef struct {
	int width;
	int height;
	int duration;
	void *data;
} map;


void init_map(map *map, int duration, int width, int height);
void free_map(map *map);

void set(map *map, int t, int x, int y, bool status);
bool lookup(map *map, int t, int x, int y);
bool future(map *map, int t, int x, int y);

void tick(map *map, int t);
void loop(map *map);

void print_map(map *map, int t, int x0, int y0, int x1, int y1);

int main(int argc, char **argv) {
	int width = 50;
	int height = 50;
	int duration = 20;
	
	map map;
	
	init_map(&map, duration, width, height);
	
	for (int x = 14; x <= 34; ++x) {
		for (int y = 14; y <= 34; ++y) {
			set(&map, 0, x, y, true);
		}
	}
	
	loop(&map);
	
	return EXIT_SUCCESS;
}

void init_map(map *map, int duration, int width, int height) {
	map->duration = duration;
	map->width = width;
	map->height = height;

	size_t num = duration*width*height;
	
	map->data = calloc(num, sizeof(bool));
	memset(map->data, false, num * sizeof(bool));
}

void free_map(map *map) {
	free(map->data);
}

void set(map *map, int t, int x, int y, bool status) {
	bool (*arr)[map->width][map->height] = map->data;
	
	arr[t][x][y] = status;
}

bool lookup(map *map, int t, int x, int y) {
	if (t < 0 || t >= map->duration ||
		x < 0 || x >= map->width    ||
		y < 0 || y >= map->height     )
	{
		return false;
	}
	
	bool (*arr)[map->width][map->height] = map->data;
	
	return arr[t][x][y];
}

bool future(map *map, int t, int x, int y) {
	int neighbors = 0;
	
	for (int dx = -1; dx <= 1; ++dx) {
		for (int dy = -1; dy <= 1; ++dy) {
			if (dx == 0 && dy == 0)
				continue;
			
			if (lookup(map, t, x+dx, y+dy))
				++neighbors;
		}
	}
	
	// if cell is live
	if (lookup(map, t, x, y)) {
		// stay live if their are two to three neighbors
		return neighbors >= 2 && neighbors <= 3;
	} else {
		// be born when there are exactly three neighbors
		return neighbors == 3;
	}
}

void print_map(map *map, int t, int x0, int y0, int x1, int y1) {
	printf("t=%d x=%d-%d y=%d-%d:\n", t, x0, x1, y0, y1);

	for (int x = x0; x <= x1; ++x) {
		for (int y = y0; y <= y1; ++y) {
			if (lookup(map, t, x, y))
				printf("X");
			else
				printf(" ");
		}
		
		printf("\n");
	}
}

void tick(map *map, int t) {
	printf("tick %d\n", t);
	
	for (int x = 0; x < map->width; ++x) {
		for (int y = 0; y < map->height; ++y) {
			bool status = future(map, t, x, y);
			
			set(map, t+1, x, y, status);
		}
	}
}

void loop(map *map) {
	for (int t = 0; t < map->duration-1; ++t) {
		tick(map, t);
		
		print_map(map, t, 0, 0, map->width-1, map->height-1);
	}
}

