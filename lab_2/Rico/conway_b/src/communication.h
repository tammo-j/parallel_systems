#ifndef CONWAY_NEIGHBOR_COMMUNICATION_H
#define CONWAY_NEIGHBOR_COMMUNICATION_H


#include "cell_list.h"


#define COMM_TOP 0
#define COMM_BOTTOM 1
#define COMM_LEFT 2
#define COMM_RIGHT 3
#define COMM_TOP_LEFT 4
#define COMM_TOP_RIGHT 5
#define COMM_BOTTOM_LEFT 6
#define COMM_BOTTOM_RIGHT 7


typedef struct {
	int x;
	int y;
	int neighbors;
} neighbor_info;

typedef struct {
	int size;
	neighbor_info *neighbors;
} conway_msg;


void comm_init(int duration, int xsize, int ysize);
void comm_free();

void comm_send(int dst, int t, conway_msg *msg);
void comm_recv(int *src, int t, conway_msg *msg);

void comm_set_dimensions(int xsize, int ysize);
// TODO should be obsolet with MPI
void comm_set_current_node(int x, int y);

#endif // CONWAY_LIST_COMMUNICATION_H
