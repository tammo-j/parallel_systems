#ifndef CONWAY_NEIGHBOR_COMMUNICATION_H
#define CONWAY_NEIGHBOR_COMMUNICATION_H


#include "conway_communication.h"
#include "cell_list.h"

typedef struct {
	int x;
	int y;
	int neighbors;
} neighbor_info;

struct conway_msg_t {
	int size;
	neighbor_info *neighbors;
};

void comm_set_dimensions(int xsize, int ysize);
void comm_set_current_node(int x, int y);

#endif // CONWAY_LIST_COMMUNICATION_H
