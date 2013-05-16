#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char** argv)
{
	int rank;
	char* msg;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	if(rank == 0)
	{
		msg = "Hello!";
	}
	MPI_Bcast(&msg, 10, MPI_CHAR, 0, MPI_COMM_WORLD); //FIXME: Warum nicht mit strlen(msg) wie bei telephoneGame.c?
	printf("%u: my msg is '%s'.\n", rank, msg);

	MPI_Finalize();
	return EXIT_SUCCESS;
}
