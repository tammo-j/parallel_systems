#include "algorithm.h"

#include <string.h>

#include "communication.h"


// TODO remove
#include <stdio.h>

static void print_neighbors(intermap *inter) {
	int width = inter->width;
	int height = inter->height;
	char (*neighbors)[height] = (char (*)[height]) inter->neighbors;
	
	for (int j = 0; j < height; ++j) {
		for (int i = 0; i < width; ++i) {
			printf("%d", neighbors[i][j]);
		}
		printf("\n");
	}
}


static void tick(map *map, intermap *inter, int t);
static void touch(intermap *inter, int x, int y);
static void touch_neighbors(intermap *inter, int x, int y);
static void touch_from(intermap *inter, cell_list *from);
static void calculate(intermap *inter, cell_list *live, cell_list *touched, cell_list *future);
static void calculate_core(intermap *inter);
static void calculate_iborder(intermap *inter);
static void send_neighbors(intermap *inter, int t);
static void recv_neighbors(intermap *inter, int t);

static int max(int a, int b) {
	return a > b ? a : b;
}

void conway_solve(map *map, intermap *inter) {
	intermap_fill(inter, map);
	
	// for each tick
	for (int t = 0; t < map->duration-1; ++t)
		tick(map, inter, t);
	
	intermap_commit(inter, map, map->duration-1);
}

void conway_solve_border(intermap *inter, int t) {
	intermap_reset_neighbors(inter);
	
	// touch from inner border and send
	touch_from(inter, &inter->current_iborder);
	send_neighbors(inter, t);
}

void conway_solve_core(map *map, intermap *inter, int t) {
	cell_list_empty(&inter->touched_oborder);
	
	touch_from(inter, &inter->current_core);
	
	calculate_core(inter);
	cell_list_empty(&inter->touched_core);
	
	// recv touched inner border and calculate it
	recv_neighbors(inter, t);
	
	calculate_iborder(inter);
	cell_list_empty(&inter->touched_iborder);
	
	// commit changes
	intermap_commit(inter, map, t);
}

void tick(map *map, intermap *inter, int t) {
	conway_solve_border(inter, t);
	conway_solve_core(map, inter, t);
}

void touch(intermap *inter, int x, int y) {
	int width = inter->width;
	int height = inter->height;
	
	char (*n)[height] = (char (*)[height]) inter->neighbors;
	
	if (++n[x][y] > 1)
		return;
	
	cell_list *touched;
	
	if (x >= 2 && x <= width-3 &&
		y >= 2 && y <= height-3)
	{
		touched = &inter->touched_core;
	} else if (
		x == 0 || x == width-1 ||
		y == 0 || y == height-1)
	{
		touched = &inter->touched_oborder;
	} else {
		touched = &inter->touched_iborder;
	}
	
	cell_list_append(touched, x, y);
}

void touch_neighbors(intermap *inter, int x, int y) {
	for (int dx = -1; dx <= 1; ++dx) {
		for (int dy = -1; dy <= 1; ++dy) {
			if (dx == 0 && dy == 0)
				continue;
				
			touch(inter, x+dx, y+dy);
		}
	}
}

void touch_from(intermap *inter, cell_list *from) {
	int x, y;
	
	cell_list_restart(from);
	while (cell_list_next(from, &x, &y))
		touch_neighbors(inter, x, y);
}

void calculate(intermap *inter, cell_list *live, cell_list *touched, cell_list *future) {
	int height = inter->height;
	char (*n)[height] = (char (*)[height]) inter->neighbors;
	
	int x, y;
	
	// for all touched cells
	// apply birth rule
	cell_list_restart(touched);
	while (cell_list_next(touched, &x, &y)) {
		// consider touched cells dead
		// this exploits that the birth rule is
		// a partial set of the survival rule
		if (n[x][y] == 3) {
			cell_list_append(future, x, y);
			n[x][y] = 0; // avoid that a cell is appended multiple times
		}
	}
	
	// for all live cells
	// apply survival rule
	cell_list_restart(live);
	while (cell_list_next(live, &x, &y)) {
		// couldn't be 3 since it was checked above
		if (n[x][y] == 2)
			cell_list_append(future, x, y);
	}
}

void calculate_core(intermap *inter) {
	cell_list *now     = &inter->current_core;
	cell_list *touched = &inter->touched_core;
	cell_list *future  = &inter->future_core;
	
	calculate(inter, now, touched, future);
}

void calculate_iborder(intermap *inter) {
	cell_list *now     = &inter->current_iborder;
	cell_list *touched = &inter->touched_iborder;
	cell_list *future  = &inter->future_iborder;
	
	calculate(inter, now, touched, future);
}

// TODO worst routine ever :/
void send_neighbors(intermap *inter, int t) {
	int width  = inter->width;
	int height = inter->height;
	
	char (*n)[height] = (char (*)[height]) inter->neighbors;
	cell_list *touched = &inter->touched_oborder;
	
	neighbor_info *info;
	conway_msg *msg, msgs[8];
	neighbor_info
		top   [width] , top_left    ,
		bottom[width] , top_right   ,
		left  [height], bottom_left ,
		right [height], bottom_right;
	
	// initialize infos
	for (int i = 0; i < 8; ++i) {
		msgs[i].size = 0;
		
		switch (i) {
			case COMM_TOP:          msgs[i].neighbors = top;           break;
			case COMM_BOTTOM:       msgs[i].neighbors = bottom;        break;
			case COMM_LEFT:         msgs[i].neighbors = left;          break;
			case COMM_RIGHT:        msgs[i].neighbors = right;         break;
			case COMM_TOP_LEFT:     msgs[i].neighbors = &top_left;     break;
			case COMM_TOP_RIGHT:    msgs[i].neighbors = &top_right;    break;
			case COMM_BOTTOM_LEFT:  msgs[i].neighbors = &bottom_left;  break;
			case COMM_BOTTOM_RIGHT: msgs[i].neighbors = &bottom_right; break;
		}
	}
	
	// append at appropriate message container
	int x, y;
	cell_list_restart(touched);
	while (cell_list_next(touched, &x, &y)) {
		if (x == 0) {                 // left with corners
			if (y == 0) {             // top left corner
				msg = &msgs[COMM_TOP_LEFT];
			} else if (y == height-1) // bottom left corner
				msg = &msgs[COMM_BOTTOM_LEFT];
			else                      // top border
				msg = &msgs[COMM_LEFT];
		} else if (x == height-1) {   // right with corners
			if (y == 0)               // top right corner
				msg = &msgs[COMM_TOP_RIGHT];
			else if (y == height-1)   // bottom right corner
				msg = &msgs[COMM_BOTTOM_RIGHT];
			else                      // bottom border
				msg = &msgs[COMM_RIGHT];
		} else if (y == 0) {          // top without corners
			msg = &msgs[COMM_TOP];
		} else {                      // bottom without corners
			msg = &msgs[COMM_BOTTOM];
		}
		
		info = &msg->neighbors[msg->size++];
		info->x = x;
		info->y = y;
		info->neighbors = n[x][y];
	}
	
	// send to every neighbor
	for (int i = 0; i < 8; ++i)
		comm_send(i, t, &msgs[i]);
}

// TODO also pretty bad
void recv_neighbors(intermap *inter, int t) {
	int width = inter->width;
	int height = inter->height;
	
	char (*n)[height] = (char (*)[height]) inter->neighbors;
	cell_list *touched = &inter->touched_iborder;
	
	int src;
	
	// reserve storage
	neighbor_info infos[max(width, height)];
	conway_msg msg;
	msg.neighbors = infos;
	
	// TODO consider border cases (remove dirty solution from comm~.c)
	// receive from each neighbor
	for (int i = 0; i < 8; ++i) {
		comm_recv(&src, t, &msg);
		
		for (int j = 0; j < msg.size; ++j) {
			int x = infos[j].x;
			int y = infos[j].y;
			
			int tx, ty;
			
			// translate coordinates
			switch (src) {
				case COMM_TOP:          tx = x;       ty = 1;        break;
				case COMM_BOTTOM:       tx = x;       ty = height-2; break;
				case COMM_LEFT:         tx = 1;       ty = y;        break;
				case COMM_RIGHT:        tx = width-2; ty = y;        break;
				case COMM_TOP_LEFT:     tx = 1;       ty = 1;        break;
				case COMM_TOP_RIGHT:    tx = width-2; ty = 1;        break;
				case COMM_BOTTOM_LEFT:  tx = 1;       ty = height-2; break;
				case COMM_BOTTOM_RIGHT: tx = width-2; ty = height-2; break;
			}
			
			if (n[tx][ty] == 0)
				cell_list_append(touched, tx, ty);
			
			n[tx][ty] += infos[j].neighbors;
		}
	}
}


