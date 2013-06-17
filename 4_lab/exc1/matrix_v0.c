#include "matrix.h"

#include <stdlib.h>
#include <stdio.h>


#include "stopwatch.h"

matrix_t *multiply_matrix(matrix_t *a, matrix_t *b)
{
	if (a->columns != b->rows) {
		fprintf(stderr, "multiply_matrix(): cannot multiply\n");
		exit(1);
	}

	matrix_t *c = allocate_matrix(a->rows, b->columns);

	stopwatch_start();
	__multiply_matrix(c, a, b);
	stopwatch_stop();

	return c;
}

void __multiply_matrix(matrix_t *c, matrix_t *a, matrix_t *b)
{
	int i,j,k;

	for (i = 0; i < a->rows; i++) {
		for (j = 0; j < b->columns; j++) {
			c->data[i * b->columns + j] = 0.0;
			for (k = 0; k < a->columns; k++) {
				c->data[i * b->columns + j] +=
					a->data[i * a->columns + k] * b->data[k * b->columns + j];
			}
		}
	}
}

