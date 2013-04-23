
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <mpi.h>


#define BUF_SIZE 100
#define ROOT 0


int main(int argc, char **argv) {
	MPI_Init(&argc, &argv);
	
	int rank;
	int size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	char msg[BUF_SIZE];
	strcpy(msg, "Some message.");
	
	MPI_Status status;
	double times[size];
	
	for (int n = 0; n < size; ++n) {
		if (rank == n) { // if current reference node
			MPI_Barrier(MPI_COMM_WORLD);
			double tstart = MPI_Wtime();
		
			for (int i = 0; i < size - 1; ++i) {
				MPI_Recv(msg, sizeof(msg), MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
				double tend = MPI_Wtime();
			
				int src = status.MPI_SOURCE;
				times[src] = tend - tstart;
			}
		
			for (int i = 0; i < size; ++i) if (i != rank)
				printf("Round trip time between %d and %d: %5.3f ms.\n", rank, i, times[i] * 1000.);
		} else {
			MPI_Barrier(MPI_COMM_WORLD);
			MPI_Send(msg, sizeof(msg), MPI_CHAR, n, 0, MPI_COMM_WORLD);
		}
	}
	
	MPI_Finalize();
	return EXIT_SUCCESS;
}

