#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

int main(int argc, char **argv)
{
	if (argc != 5) {
		printf("usage: %s <random seed> <rows> <columns> <file>\n"
		       "  generates a matrix filled with random values and stores\n"
		       "  it in the specified file\n",
		       argv[0]);
		exit(1);
	}

	matrix_t *m;

	int seed = atoi(argv[1]);
	int rows = atoi(argv[2]);
	int columns = atoi(argv[3]);

	srand(seed);
	m = generate_random_matrix(rows, columns);
	store_matrix(m, argv[4]);

	free_matrix(m);
	return 0;
}
