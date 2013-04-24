
/*
 * authors: Tammo Johannes Herbert (319391)
 *          Rico Jasper            (319396)
 *          Erik Rudisch           (343930)
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <mpi.h>


#define REQ_ARGC 2
#define MSG_ARG 1
#define BUF_SIZE 100
#define ROOT 0

bool check_msg_len(int argc, char **argv, int size);
void concat_msg(char *msg, int argc, char **argv);

int main(int argc, char **argv) {
	MPI_Init(&argc, &argv);
	
	int rank;
	int size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (check_msg_len(argc, argv, size)) {
		if (rank == 0)
			fprintf(stderr, "message too long\n");
			
		MPI_Finalize();
		exit(EXIT_FAILURE);
	}
	
	// message buffer
	char msg[BUF_SIZE];
	
	// if original source
	if (rank == ROOT) {
		printf("There are %d ranks.\n", size);
		
		// concat message from arguments
		concat_msg(msg, argc, argv);
		
		printf("Original message is \"%s\"\n", msg);
	}
	
	// share message
	MPI_Bcast(msg, BUF_SIZE, MPI_CHAR, ROOT, MPI_COMM_WORLD);
	
	printf("Message is \"%s\" from rank %d\n", msg, rank);

	MPI_Finalize();

	return EXIT_SUCCESS;
}

bool check_msg_len(int argc, char **argv, int size) {
	int total_len = 0;
	for (int i = 1; i < argc; ++i)
		total_len += strlen(argv[i]) + 1;
	
	return BUF_SIZE < total_len + size;
}

void concat_msg(char *msg, int argc, char **argv) {
	if (argc > 1) {
		strcpy(msg, argv[1]);

		for (int i = 2; i < argc; ++i) {
			strcat(msg, " ");
			strcat(msg, argv[i]);
		}
	}
}

