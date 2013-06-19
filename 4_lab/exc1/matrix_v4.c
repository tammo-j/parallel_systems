#include "matrix.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>

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
	memset(c->data, 0, c->rows * c->columns * sizeof(double));
	
	#pragma omp parallel for
	for (int i = 0; i < a->rows; ++i) {
		for (int k = 0; k < a->columns; ++k) {
			int j;
			for (j = 0; j < b->columns - 5; j += 6) {
				double a_ik = a->data[i * a->columns + k];
				
				c->data[i * b->columns + (j+0)] += a_ik * b->data[k * b->columns + (j+0)];
				c->data[i * b->columns + (j+1)] += a_ik * b->data[k * b->columns + (j+1)];
				c->data[i * b->columns + (j+2)] += a_ik * b->data[k * b->columns + (j+2)];
				c->data[i * b->columns + (j+3)] += a_ik * b->data[k * b->columns + (j+3)];
				c->data[i * b->columns + (j+4)] += a_ik * b->data[k * b->columns + (j+4)];
				c->data[i * b->columns + (j+5)] += a_ik * b->data[k * b->columns + (j+5)];
			}
			
			for (; j < b->columns; ++j)
				c->data[i * b->columns + j] += a->data[i * a->columns + k] * b->data[k * b->columns + j];
		}
	}
}

