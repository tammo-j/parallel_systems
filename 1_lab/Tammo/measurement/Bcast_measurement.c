#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char** argv)
{
	int rank, size;
	char* msg;
	double starttime, endtime;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	if(rank == 0)
	{
		msg = "Hello!";
		starttime = MPI_Wtime();
	}
	MPI_Bcast(&msg, 10, MPI_CHAR, 0, MPI_COMM_WORLD); //FIXME: Warum nicht mit strlen(msg) wie bei telephoneGame.c?
	
	MPI_Barrier(MPI_COMM_WORLD);
	if(rank == 0)
	{
		endtime = MPI_Wtime();
		printf("time of awareness = %5.3f ms (by broadcasting to %u processes)\n", (endtime-starttime)*1000, size);
	}

	MPI_Finalize();
	return EXIT_SUCCESS;
}
