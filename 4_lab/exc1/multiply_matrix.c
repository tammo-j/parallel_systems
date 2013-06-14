#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"
#include "stopwatch.h"

int main(int argc, char **argv)
{
	int i;
	
	if (argc != 4) {
		printf("usage: %s <file A> <file B> <file C>\n"
		       "  loads both A and B and stores the product in C\n",
		       argv[0]);
		exit(1);
	}

	matrix_t *a = load_matrix(argv[1]);
	matrix_t *b = load_matrix(argv[2]);
	matrix_t *c;
	
	stopwatch_reset();
	for (i = 0; i < 1000; ++i)
		c = multiply_matrix(a, b);
	store_matrix(c, argv[3]);
	
	printf("%.2e seconds needed\n", stopwatch_get());

	free_matrix(a);
	free_matrix(b);
	free_matrix(c);
	return 0;
}
