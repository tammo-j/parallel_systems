
/*
 * authors: Tammo Johannes Herbert
 *          Rico Jasper            (319396)
 *          Erik Rudisch           (343930)
 */

#include <stdlib.h>
#include <stdio.h>

#include "mpi.h"

int main(int argc, char **argv) {
	MPI_Init(&argc, &argv);
	
	printf("Hello World\n");
	
	int rank;
	int size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	printf("size = %d, rank = %d\n", size, rank);
	
	MPI_Finalize();

	return EXIT_SUCCESS;
}

