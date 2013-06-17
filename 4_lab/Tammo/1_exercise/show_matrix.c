#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("usage: %s <file>\n"
		       "  loads the matrix in the specified file and displays it\n",
		       argv[0]);
		exit(1);
	}

	matrix_t *m = load_matrix(argv[1]);
	show_matrix(m);

	free_matrix(m);
	return 0;
}
