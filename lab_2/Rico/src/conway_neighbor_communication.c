#include "conway_neighbor_communication.h"

#include <stdlib.h>


struct msg_stack_t {
	int src;
	conway_msg *msg;
	struct msg_stack_t *next;
};

typedef struct msg_stack_t msg_stack;

static int comm_dir2id(int direction);
static int comm_id2dir(int id);
static int comm_dst2src(int src);

static int g_xsize = -1;
static int g_ysize = -1;
static int g_duration = -1;
static int g_nqueues = -1;
static int g_x = -1;
static int g_y = -1;
static msg_stack *g_msg_queues = NULL;

void conway_send(int dst, int t, conway_msg *msg) {
	int id = comm_dir2id(dst);
	
	msg_stack *stack = &g_msg_queues[id*g_duration + t];
	
	// allocations
	msg_stack *new = malloc(sizeof(*new));
	new->msg = malloc(sizeof(*new->msg));
	new->msg->neighbors = calloc(msg->size, sizeof(*new->msg->neighbors));
	
	// copy message
	new->msg->size = msg->size;
	new->src = comm_dst2src(dst);
	
	for (int i = 0; i < msg->size; ++i)
		new->msg->neighbors[i] = msg->neighbors[i];
		
	// push on stack
	new->next = stack->next;
	stack->next = new;
}

void conway_recv(int *src, int t, conway_msg *msg) {
	int dst_id = g_x*g_ysize + g_y;
	int stack_id = dst_id*g_duration + t;
	
	msg_stack *stack = &g_msg_queues[stack_id];
	msg_stack *top = stack->next;
	
	*src = top->src;
	
	// copy message
	msg->size = top->msg->size;
	for (int i = 0; i < msg->size; ++i)
		msg->neighbors[i] = top->msg->neighbors[i];
	
	// pop from stack
	stack->next = top->next;
	
	// deallocations
	free(top->msg->neighbors);
	free(top->msg);
	free(top);
}

void comm_init(int duration, int xsize, int ysize) {
	g_duration = duration;
	g_xsize = xsize;
	g_ysize = ysize;
	g_nqueues = duration*xsize*ysize;
	
	g_msg_queues = calloc(g_nqueues, sizeof(*g_msg_queues));
	
	for (int i = 0; i < g_nqueues; ++i) {
		g_msg_queues[i].msg = NULL;
		g_msg_queues[i].next = NULL;
	}
}

void comm_free() {
	msg_stack *cur, *next;
	
	for (int i = 0; i < g_nqueues; ++i) {
		cur = g_msg_queues[i].next;
		
		while (cur) {
			next = cur->next;
			free(cur);
		}
	}
	
	free(g_msg_queues);
}

void comm_set_current_node(int x, int y) {
	g_x = x;
	g_y = y;
}

int comm_dst2src(int dst) {
	switch (dst) {
		case COMM_TOP:          return COMM_BOTTOM;
		case COMM_BOTTOM:       return COMM_TOP;
		case COMM_LEFT:         return COMM_RIGHT;
		case COMM_RIGHT:        return COMM_LEFT;
		case COMM_TOP_LEFT:     return COMM_BOTTOM_RIGHT;
		case COMM_TOP_RIGHT:    return COMM_BOTTOM_LEFT;
		case COMM_BOTTOM_LEFT:  return COMM_TOP_RIGHT;
		case COMM_BOTTOM_RIGHT: return COMM_TOP_LEFT;
	}
}

int comm_dir2id(int direction) {
	int x = g_x;
	int y = g_y;
	
	switch (direction) {
		case COMM_TOP:                  y -= 1; break;
		case COMM_BOTTOM:               y += 1; break;
		case COMM_LEFT:         x -= 1;         break;
		case COMM_RIGHT:        x += 1;         break;
		case COMM_TOP_LEFT:     x -= 1; y -= 1; break;
		case COMM_TOP_RIGHT:    x += 1; y -= 1; break;
		case COMM_BOTTOM_LEFT:  x -= 1; y += 1; break;
		case COMM_BOTTOM_RIGHT: x += 1; y += 1; break;
	}
	
	if (x < 0 || x >= g_xsize ||
		y < 0 || y >= g_ysize)
	{
		return -1;
	}
	
	return x*g_ysize + y;
}

int comm_id2dir(int id) {
	int dx = id / g_ysize - g_x;
	int dy = id % g_ysize - g_y;
	
	if (dx == -1) {        // left with corners
		if (dy == -1)      // top left corner
			return COMM_TOP_LEFT;
		else if (dy == 1)  // bottom left corner
			return COMM_BOTTOM_LEFT;
		else               // left
			return COMM_LEFT;
	} else if (dx == 1) {  // right with corners
		if (dy == -1)      // top right corner
			return COMM_TOP_RIGHT;
		else if (dy == 1)  // bottom right corner
			return COMM_BOTTOM_RIGHT;
		else               // right
			return COMM_RIGHT;
	} else if (dy == -1) { // top without corners
		return COMM_TOP;
	} else {               // bottom without corners
		return COMM_BOTTOM;
	}
}

