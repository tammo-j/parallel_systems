
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "matrix.h"
#include "stopwatch.h"


#define ARG_SEED 1
#define ARG_WORK 2
#define ARG_DIMS 3

#define MAX(A,B) ((A)>(B)?(A):(B))

typedef struct {
	int m, n, k;
} dim_t;

bool verify_matrix(matrix_t *a, matrix_t *b, matrix_t *d);
bool __verify_matrix(matrix_t *c, matrix_t *a, matrix_t *b, matrix_t *d);
bool __compare_matrix(matrix_t *a, matrix_t *b);

bool verify_matrix(matrix_t *a, matrix_t *b, matrix_t *d) {
	if (a->columns != b->rows) {
		fprintf(stderr, "multiply_matrix(): cannot multiply\n");
		exit(1);
	}

	matrix_t *c = allocate_matrix(a->rows, b->columns);

	return __verify_matrix(c, a, b, d);
}

bool __verify_matrix(matrix_t *c, matrix_t *a, matrix_t *b, matrix_t *d) {
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
	
	return __compare_matrix(c, d);
}

bool __compare_matrix(matrix_t *a, matrix_t *b) {
	int i, j;
	for (i = 0; i < a->rows; i++) {
		for (j = 0; j < a->columns; j++) {
			double d = fabs(a->data[i * a->columns + j] - b->data[i * a->columns + j]);
			if (d > MAXDIFF)
				return false;
		}
	}
	
	return true;
}

int main(int argc, char **argv) {
	if (argc < 3) {
		printf("usage: %s <random seed> <minimum work in G> <mxnxk> [<mxnxk>, ...]\n",
		       argv[0]);
		exit(EXIT_FAILURE);
	}
	
	int seed = atoi(argv[ARG_SEED]);
	double min_work_G = 1e9 * atof(argv[ARG_WORK]);
	
	int dims_len = argc - ARG_DIMS;
	dim_t dims[dims_len];
	
	for (int i = 0; i < dims_len; ++i) {
		char *dim_str = argv[ARG_DIMS+i];
		
		int num = sscanf(dim_str, "%dx%dx%d", &dims[i].m, &dims[i].n, &dims[i].k);
		
		if (num != 3) {
			printf("bad dimension format \"%s\" at %d\n", dim_str, ARG_DIMS+i);
			
			exit(EXIT_FAILURE);
		}
	}
	
	srand(seed);
	
	for (int i = 0; i < dims_len; ++i) {
		int m = dims[i].m;
		int n = dims[i].n;
		int k = dims[i].k;
	
		int matrix_work = m * n * k;
		long long repeat = ceil( (double) min_work_G / (double) matrix_work );
		repeat = MAX(1, repeat);

		printf("%d/%d: Multiplying %dx%d and %dx%d matrices %lld times ...\n", i+1, dims_len, m, n, n, k, repeat);
		
		stopwatch_reset();
	
		bool error = false;
		for (int j = 0; j < repeat; ++j) {
			matrix_t *A = generate_random_matrix(m, n);
			matrix_t *B = generate_random_matrix(n, k);
			matrix_t *C = multiply_matrix(A, B);
			
			//error |= !verify_matrix(A, B, C);

			free_matrix(A);
			free_matrix(B);
			free_matrix(C);
		}
		
		double time = stopwatch_get();
		
		double wps = repeat*matrix_work / time / 1e6;
		
		if (error)
			printf("\terror found!\n");
		printf("\t%.2f seconds needed. WPS = %.1f\n", time, wps);
	}
	
	return EXIT_SUCCESS;
}

