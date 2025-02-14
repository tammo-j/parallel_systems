#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

int main(int argc, char **argv)
{
	if (argc != 4) {
		printf("usage: %s <file A> <file B> <file C>\n"
		       "  loads both A and B and stores the product in C\n",
		       argv[0]);
		exit(1);
	}

	matrix_t *a = load_matrix(argv[1]);
	matrix_t *b = load_matrix(argv[2]);
	matrix_t *c = multiply_matrix(a, b);
	store_matrix(c, argv[3]);

	free_matrix(a);
	free_matrix(b);
	free_matrix(c);
	return 0;
}
