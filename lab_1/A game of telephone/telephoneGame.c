#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char** argv)
{
	int rank, size;
	MPI_Status status;
	char* msg;	
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	if(rank == 0)
	{
		msg = "Hallo!";
	}
	if(rank > 0)
	{
		MPI_Recv(&msg, strlen(msg), MPI_CHAR, rank-1, 0, MPI_COMM_WORLD, &status);
		printf("%u: Received '%s' from %u.\n", rank, msg, rank-1);
	}
	if(rank < size-1)
	{
		MPI_Send(&msg, strlen(msg), MPI_CHAR, rank+1, 0, MPI_COMM_WORLD);
		printf("%u: Sent '%s' to %u.\n", rank, msg, rank+1);
	}

	MPI_Finalize();
	return EXIT_SUCCESS;
}
