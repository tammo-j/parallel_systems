#include "communication.h"

#include <stdlib.h>
#include <mpi.h>

#include "grid.h"

static void define_types();

static MPI_Datatype mpi_neighbor_info;
static MPI_Datatype mpi_cell_info;

void define_types() {
	// define mpi_cell_info
	
    int cell_info_count = 3;
    int cell_info_lengths[] = {1, 1, 1};
    MPI_Aint cell_info_offsets[] = {0, sizeof(int), 2*sizeof(int)};
    MPI_Datatype cell_info_types[] = {MPI_INT, MPI_INT, MPI_INT};
    
    MPI_Type_create_struct(cell_info_count, cell_info_lengths, cell_info_offsets, cell_info_types, &mpi_cell_info);
    MPI_Type_commit(&mpi_cell_info);
    
	// define mpi_neighbor_info
	
    int neighbor_info_count = 3;
    int neighbor_info_lengths[] = {1, 1, 1};
    MPI_Aint neighbor_info_offsets[] = {0, sizeof(int), 2*sizeof(int)};
    MPI_Datatype neighbor_info_types[] = {MPI_INT, MPI_INT, MPI_INT};
    
    MPI_Type_create_struct(neighbor_info_count, neighbor_info_lengths, neighbor_info_offsets, neighbor_info_types, &mpi_neighbor_info);
    MPI_Type_commit(&mpi_neighbor_info);
    
}

void comm_init() {
	define_types();
}

void comm_send_neighbors(int dst_dir, int t, conway_msg *msg) {
	int id = grid_dir2id(dst_dir);
	
	// no neighbor
	if (id == -1)
		return;
		
	MPI_Send(msg->neighbors, msg->size, mpi_neighbor_info, id, t, MPI_COMM_WORLD);
}

void comm_recv_neighbors(int *src_dir, int t, conway_msg *msg) {
	int len;
	MPI_Status status;

	MPI_Probe(MPI_ANY_SOURCE, t, MPI_COMM_WORLD, &status);
	MPI_Get_count(&status, mpi_neighbor_info, &len);
	
	MPI_Recv(msg->neighbors, len, mpi_neighbor_info, MPI_ANY_SOURCE, t, MPI_COMM_WORLD, &status);
	msg->size = len;
	
	*src_dir = grid_id2dir(status.MPI_SOURCE);
}

		
void comm_recv_partial(map *map, int src_id, int *sx, int *sy) {
	int len;
	MPI_Status status;

	MPI_Probe(src_id, 0, MPI_COMM_WORLD, &status);
	MPI_Get_count(&status, mpi_cell_info, &len);
	
	cell_info cells[len];
		
	MPI_Recv(cells, len, mpi_cell_info, src_id, 0, MPI_COMM_WORLD, &status);
	
	grid_id2grid(status.MPI_SOURCE, sx, sy);
	
	for (int i = 0; i < len; ++i) {
		int x = cells[i].x;
		int y = cells[i].y;
		int t = cells[i].t;
		
		map_append(map, t, x, y);
	}
}

void comm_send_partial(map *map, int dst_id) {
	// count elements
	int size = 0;
	for (int t = 0; t < map->duration; ++t)
		size += map_count(map, t);
	
	cell_info cells[size];
	int i = 0;
	
	// fill elements
	for (int t = 0; t < map->duration; ++t) {
		int x, y;
		map_restart(map, t);
		while (map_next(map, t, &x, &y)) {
			cell_info *cell = &cells[i++];
			
			cell->x = x;
			cell->y = y;
			cell->t = t;
		}
	}
	
	MPI_Send(cells, size, mpi_cell_info, dst_id, 0, MPI_COMM_WORLD);
}

