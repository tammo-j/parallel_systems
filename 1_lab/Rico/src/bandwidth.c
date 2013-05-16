
#include <stdlib.h>
#include <stdio.h>

#include <mpi.h>


#define BUF_SIZE 1024 // 1 KiB
#define ROOT 0
#define SIZE 100.
#define REPEAT SIZE*1024 // SIZE MiB


int main(int argc, char **argv) {
	MPI_Init(&argc, &argv);
	
	int rank;
	int size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	MPI_Status status;
	char msg[BUF_SIZE];
	
	for (int src = 0; src < size; ++src) {
		for (int dst = 0; dst < size; ++dst) if (dst != src) {
			MPI_Barrier(MPI_COMM_WORLD);
			if (rank == src) { // if source
				for (int i = 0; i < REPEAT; ++i)
					MPI_Send(msg, BUF_SIZE, MPI_CHAR, dst, 0, MPI_COMM_WORLD);
			} else if (rank == dst) { // if destination
				double tstart = MPI_Wtime();
				for (int i = 0; i < REPEAT; ++i)
					MPI_Recv(msg, BUF_SIZE, MPI_CHAR, src, 0, MPI_COMM_WORLD, &status);
				double tend = MPI_Wtime();
				double duration = tend - tstart;
			
				printf("from %d to %d duration: %3.1f s (%5.1f MiB/s)\n", src, dst, duration, SIZE / duration);
			}
		}
	}

	MPI_Finalize();

	return EXIT_SUCCESS;
}
