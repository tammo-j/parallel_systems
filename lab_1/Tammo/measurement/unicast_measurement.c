#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char** argv)
{
	int rank, size;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	char* msg;	
	double starttime, endtime;
	
	if(rank == 0)
	{
		starttime = MPI_Wtime();
		msg = "Hallo!";
	}
	if(rank > 0)
	{
		MPI_Recv(&msg, strlen(msg), MPI_CHAR, rank-1, 0, MPI_COMM_WORLD, &status);
	}
	if(rank < size-1)
	{
		MPI_Send(&msg, strlen(msg), MPI_CHAR, rank+1, 0, MPI_COMM_WORLD);
	}

	MPI_Barrier(MPI_COMM_WORLD);
	if(rank == 0)
	{
		endtime = MPI_Wtime();
		printf("time of awareness = %5.3f ms (by sending %u unicasts)\n", (endtime-starttime)*1000, size);
	}

	MPI_Finalize();
	return EXIT_SUCCESS;
}
