#include "conway_algorithm.h"

#include <string.h>

#include "map_list.h"
#include "conway_neighbor_communication.h"


// TODO remove
#include <stdio.h>

static void print_neighbors(map *map) {
	intermap *inter = map_list_get_intermap(map);
	int width = map->width;
	int height = map->height;
	char (*neighbors)[height] = (char (*)[height]) inter->neighbors;
	
	for (int j = 0; j < height; ++j) {
		for (int i = 0; i < width; ++i) {
			printf("%d", neighbors[i][j]);
		}
		printf("\n");
	}
}


// static void tick(map *map, int t);

static void touch(map *map, int x, int y);
static void touch_neighbors(map *map, int x, int y);
static void touch_from_iborder(map *map);
static void touch_from_core(map *map);
static void calculate_core(map *map);
static void calculate_iborder(map *map);
static void send_oborder_neighbors(map *map, int t);
static void recv_iborder_neighbors(map *map, int t);
static void apply_birth_rule(int width, int height, char neighbors[width][height], cell_list *touched, cell_list *future);
static void apply_survival_rule(int width, int height, char neighbors[width][height], cell_list *live, cell_list *future);

static int max(int a, int b) {
	return a > b ? a : b;
}

void conway_solve(map *map) {
	map_list_fill_intermap(map);
	
	// for each tick
	for (int t = 0; t < map->duration-1; ++t)
		tick(map, t);
	
	map_list_commit(map, map->duration-1);
}

void conway_solve_border(map *map, int t) {
	intermap *inter = map_list_get_intermap(map);
	map_list_reset_neighbors(map);
	
	// touch from inner border and send
	touch_from_iborder(map);
	send_oborder_neighbors(map, t);
	map_list_empty_touched_oborder(map);
	
/*	// touch from core and calculate it*/
/*	printf("before touched by core\n");*/
/*	print_neighbors(map);*/
	
	touch_from_core(map);
	
/*	printf("after touched by core\n");*/
/*	print_neighbors(map);*/
	
	if (t == 3) {
		printf("touched by iborder and core\n");
		print_neighbors(map);
	}
	
	calculate_core(map);
	map_list_empty_touched_core(map);
}

void conway_solve_core(map *map, int t) {
	// recv touched inner border and calculate it
	recv_iborder_neighbors(map, t);
	
	if (t == 3) {
		printf("touched by recv\n");
		print_neighbors(map);
	}
	
	calculate_iborder(map);
	map_list_empty_touched_iborder(map);
	
	// commit changes
	map_list_commit(map, t);
}

void tick(map *map, int t) {
	conway_solve_border(map, t);
	conway_solve_core(map, t);
}

void touch(map *map, int x, int y) {
	intermap *inter = map_list_get_intermap(map);
	char (*n)[map->height] = (char (*)[map->height]) inter->neighbors;
	
	if (++n[x][y] > 1)
		return;
	
	cell_list *touched;
	
	if (x >= 2 && x <= map->width-3 &&
		y >= 2 && y <= map->height-3)
	{
		touched = &inter->touched_core;
	} else if (
		x == 0 || x == map->width-1 ||
		y == 0 || y == map->height-1)
	{
		touched = &inter->touched_oborder;
	} else {
		touched = &inter->touched_iborder;
	}
	
	cell_list_append(touched, x, y);
}

void touch_neighbors(map *map, int x, int y) {
	for (int dx = -1; dx <= 1; ++dx) {
		for (int dy = -1; dy <= 1; ++dy) {
			if (dx == 0 && dy == 0)
				continue;
				
			touch(map, x+dx, y+dy);
		}
	}
}

void apply_birth_rule(int width, int height, char neighbors[width][height], cell_list *touched, cell_list *future) {
	int x, y;
	
	// for all touched cells
	// apply birth rule
	cell_list_restart(touched);
	while (cell_list_next(touched, &x, &y)) {
		// consider touched cells dead
		// this exploits that the birth rule is
		// a partial set of the survival rule
		bool status = conway_future(false, neighbors[x][y]);
		
		if (status) {
			cell_list_append(future, x, y);
			neighbors[x][y] = 0; // avoid that a cell is appended multiple times
		}
	}
}

void apply_survival_rule(int width, int height, char neighbors[width][height], cell_list *live, cell_list *future) {
	int x, y;
	
	// for all live cells
	// apply survival rule
	cell_list_restart(live);
	while (cell_list_next(live, &x, &y)) {
		bool status = conway_future(true, neighbors[x][y]);
		
		if (status)
			cell_list_append(future, x, y);
	}
}

void touch_from_iborder(map *map) {
	intermap *inter = map_list_get_intermap(map);
	cell_list *iborder = &inter->current_iborder;
	int x, y;
	
	cell_list_restart(iborder);
	while (cell_list_next(iborder, &x, &y))
		touch_neighbors(map, x, y);
}

void touch_from_core(map *map) {
	intermap *inter = map_list_get_intermap(map);
	cell_list *core = &inter->current_core;
	int x, y;
	
	cell_list_restart(core);
	while (cell_list_next(core, &x, &y))
		touch_neighbors(map, x, y);
}

void calculate(int width, int height, char neighbors[width][height], cell_list *now, cell_list *touched, cell_list *future) {
	apply_birth_rule(width, height, neighbors, touched, future);
	apply_survival_rule(width, height, neighbors, now, future);
}

void calculate_core(map *map) {
	intermap *inter = map_list_get_intermap(map);
	
	char (*n)[map->height] = (char (*)[map->height]) inter->neighbors;
	cell_list *now = &inter->current_core;
	cell_list *touched = &inter->touched_core;
	cell_list *future = &inter->future_core;
	
	calculate(map->width, map->height, n, now, touched, future);
}

void calculate_iborder(map *map) {
	intermap *inter = map_list_get_intermap(map);
	
	char (*n)[map->height] = (char (*)[map->height]) inter->neighbors;
	cell_list *now = &inter->current_iborder;
	cell_list *touched = &inter->touched_iborder;
	cell_list *future = &inter->future_iborder;
	
	calculate(map->width, map->height, n, now, touched, future);
}

// TODO worst routine ever :/
void send_oborder_neighbors(map *map, int t) {
	intermap *inter = map_list_get_intermap(map);
	char (*neighbors)[map->height] = (char (*)[map->height]) inter->neighbors;
	cell_list *touched = &inter->touched_oborder;
	
	neighbor_info *info;
	conway_msg *msg;
	
	conway_msg msgs[8];
	neighbor_info
		top[map->width],
		bottom[map->width],
		left[map->height],
		right[map->height],
		top_left,
		top_right,
		bottom_left,
		bottom_right;
	
	// initialize infos
	for (int i = 0; i < 8; ++i) {
		msgs[i].size = 0;
		
		switch (i) {
		case COMM_TOP:
			msgs[i].neighbors = top;
			break;
		case COMM_BOTTOM:
			msgs[i].neighbors = bottom;
			break;
		case COMM_LEFT:
			msgs[i].neighbors = left;
			break;
		case COMM_RIGHT:
			msgs[i].neighbors = right;
			break;
		case COMM_TOP_LEFT:
			msgs[i].neighbors = &top_left;
			break;
		case COMM_TOP_RIGHT:
			msgs[i].neighbors = &top_right;
			break;
		case COMM_BOTTOM_LEFT:
			msgs[i].neighbors = &bottom_left;
			break;
		case COMM_BOTTOM_RIGHT:
			msgs[i].neighbors = &bottom_right;
			break;
		}
	}
	
	int x, y;
	cell_list_restart(touched);
	while (cell_list_next(touched, &x, &y)) {
		int tx, ty;
		
		if (x == 0) { // left with corners
			if (y == 0) {
				msg = &msgs[COMM_TOP_LEFT];
			} else if (y == map->height-1)
				msg = &msgs[COMM_BOTTOM_LEFT];
			else
				msg = &msgs[COMM_LEFT];
		} else if (x == map->height-1) { // right with corners
			if (y == 0)
				msg = &msgs[COMM_TOP_RIGHT];
			else if (y == map->height-1)
				msg = &msgs[COMM_BOTTOM_RIGHT];
			else
				msg = &msgs[COMM_RIGHT];
		} else if (y == 0) { // top without corners
			msg = &msgs[COMM_TOP];
		} else { // bottom without corners
			msg = &msgs[COMM_BOTTOM];
		}
		
		info = &msg->neighbors[msg->size++];
		info->x = x;
		info->y = y;
		info->neighbors = neighbors[x][y];
	}
	
	for (int i = 0; i < 8; ++i)
		conway_send(i, t, &msgs[i]);
}

// TODO also pretty bad
void recv_iborder_neighbors(map *map, int t) {
	intermap *inter = map_list_get_intermap(map);
	char (*neighbors)[map->height] = (char (*)[map->height]) inter->neighbors;
	cell_list *touched = &inter->touched_iborder;
	
	int src;
	
	// reserve storage
	neighbor_info infos[max(map->width, map->height)];
	conway_msg msg;
	msg.neighbors = infos;
	
	
	for (int i = 0; i < 8; ++i) {
		conway_recv(&src, t, &msg);
		
		for (int j = 0; j < msg.size; ++j) {
			int x = infos[j].x;
			int y = infos[j].y;
			
			int tx, ty;
			
			// translate coordinates
			switch (src) {
				case COMM_TOP:          tx = x;            ty = 1;             break;
				case COMM_BOTTOM:       tx = x;            ty = map->height-2; break;
				case COMM_LEFT:         tx = 1;            ty = y;             break;
				case COMM_RIGHT:        tx = map->width-2; ty = y;             break;
				case COMM_TOP_LEFT:     tx = 1;            ty = 1;             break;
				case COMM_TOP_RIGHT:    tx = map->width-2; ty = 1;             break;
				case COMM_BOTTOM_LEFT:  tx = 1;            ty = map->height-2; break;
				case COMM_BOTTOM_RIGHT: tx = map->width-2; ty = map->height-2; break;
			}
			
			int n = infos[j].neighbors;
			
			if (neighbors[tx][ty] == 0)
				cell_list_append(touched, tx, ty);
			
			neighbors[tx][ty] += n;
		}
	}
}


