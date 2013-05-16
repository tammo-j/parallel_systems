#ifndef CONWAY_NEIGHBOR_COMMUNICATION_H
#define CONWAY_NEIGHBOR_COMMUNICATION_H


#include "map.h"


typedef struct {
	int x;
	int y;
	int t;
} cell_info;

typedef struct {
	int x;
	int y;
	int neighbors;
} neighbor_info;

typedef struct {
	int size;
	neighbor_info *neighbors;
} conway_msg;


void comm_init();

void comm_send_neighbors(int dst, int t, conway_msg *msg);
void comm_recv_neighbors(int *src, int t, conway_msg *msg);

void comm_recv_partial(map *map, int src_id, int *sx, int *sy);
void comm_send_partial(map *map, int dst_id);

#endif // CONWAY_LIST_COMMUNICATION_H
