#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

int main(int argc, char **argv)
{
	if (argc != 3) {
		printf("usage: %s <file A> <file B>\n"
		       "  loads both A and B and compares them (thereby allowing a small difference per value)\n",
		       argv[0]);
		exit(1);
	}

	matrix_t *a = load_matrix(argv[1]);
	matrix_t *b = load_matrix(argv[2]);
	compare_matrix(a, b);

	free_matrix(a);
	free_matrix(b);
	return 0;
}
