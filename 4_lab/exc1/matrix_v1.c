#include "matrix.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

void __multiply_matrix(matrix_t * restrict c, matrix_t * restrict a, matrix_t * restrict b)
{
	int i,j,k;
	memset(c->data, 0, c->rows * c->columns * sizeof(double));

	for (i = 0; i < a->rows; i++) {
		for (j = 0; j < b->columns; j++) {
			for (k = 0; k < a->columns; k++) {
				c->data[i * b->columns + j] +=
					a->data[i * a->columns + k] * b->data[k * b->columns + j];
			}
		}
	}
}

