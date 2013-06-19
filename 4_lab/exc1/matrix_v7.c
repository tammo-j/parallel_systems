#include "matrix.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "stopwatch.h"

#define TRANSPOSE_THR 2

static matrix_t *transpose(matrix_t *a);
static void transpose_block(matrix_t *a, matrix_t *b, int il, int ir, int jl, int jr);
static void transpose_iterator(matrix_t *a, matrix_t *b, int il, int ir, int jl, int jr);

matrix_t *multiply_matrix(matrix_t *a, matrix_t *b)
{
	if (a->columns != b->rows) {
		fprintf(stderr, "multiply_matrix(): cannot multiply\n");
		exit(1);
	}

	matrix_t *c = allocate_matrix(a->rows, b->columns);
	

	stopwatch_start();
	matrix_t *b_ = transpose(b);
	__multiply_matrix(c, a, b_);
	stopwatch_stop();

	return c;
}

void __multiply_matrix(matrix_t * restrict c, matrix_t * restrict a, matrix_t * restrict b)
{
	memset(c->data, 0, c->rows * c->columns * sizeof(double));

	#pragma omp parallel for
	for (int i = 0; i < a->rows; i++) {
		for (int j = 0; j < b->columns; j++) {
			int k;
			for (k = 0; k < a->columns - 5; k += 6) {
				c->data[i * b->columns + j] +=
					a->data[i * a->columns + k+0] * b->data[j * b->columns + k+0] +
					a->data[i * a->columns + k+1] * b->data[j * b->columns + k+1] +
					a->data[i * a->columns + k+2] * b->data[j * b->columns + k+2] +
					a->data[i * a->columns + k+3] * b->data[j * b->columns + k+3] +
					a->data[i * a->columns + k+4] * b->data[j * b->columns + k+4] +
					a->data[i * a->columns + k+5] * b->data[j * b->columns + k+5];
			}
			
			for (; k < a->columns; ++k)
				c->data[i * b->columns + j] += a->data[i * a->columns + k] * b->data[j * b->columns + k];
		}
	}
}

matrix_t *transpose(matrix_t *a) {
	matrix_t *b = allocate_matrix(a->columns, a->rows);
	
	transpose_iterator(a, b, 0, a->rows, 0, a->columns);
	
	return b;
}

void transpose_iterator(matrix_t *a, matrix_t *b, int il, int ir, int jl, int jr) {
	if(ir-il < TRANSPOSE_THR) {
		transpose_block(a, b, il, ir, jl, jr);
		return;
	}
	
	int im = il + (ir-il)/2;
	int jm = jl + (jr-jl)/2;

	transpose_iterator(a, b, il, im, jl, jm);
	transpose_iterator(a, b, im, ir, jl, jm);
	transpose_iterator(a, b, il, im, jm, jr);
	transpose_iterator(a, b, im, ir, jm, jr);
}

void transpose_block(matrix_t *a, matrix_t *b, int il, int ir, int jl, int jr) {
	double (*a_)[a->columns] = (double (*)[a->columns]) a->data;
	double (*b_)[b->columns] = (double (*)[b->columns]) b->data;
	
	for (int i = il; i < ir; ++i)
		for (int j = jl; j < jr; ++j)
			b_[j][i] = a_[i][j];
}
