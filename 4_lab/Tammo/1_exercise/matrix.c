#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "matrix.h"

matrix_t *allocate_matrix(int rows, int columns)
{
	matrix_t *m = malloc(sizeof(*m));

	if (m == NULL) {
		fprintf(stderr, "allocate_matrix(): out of memory\n");
		exit(1);
	}

	strncpy(m->header, HEADER, HEADERLENGTH);
	m->rows = rows;
	m->columns = columns;
	m->data = malloc(m->rows * m->columns * sizeof(*m->data));

	if (m->data == NULL) {
		fprintf(stderr, "allocate_matrix(): out of memory\n");
		exit(1);
	}

	return m;
}

void free_matrix(matrix_t *m)
{
	free(m->data);
	free(m);
}

matrix_t *generate_random_matrix(int rows, int columns)
{
	matrix_t *m = allocate_matrix(rows, columns);

	int i, j;
	for (i = 0; i < m->rows; i++)
		for (j = 0; j < m->columns; j++)
			m->data[i * m->columns + j] =
				-1.0 + (2.0 * (double)rand() / ((double)RAND_MAX + 1.0));

	return m;
}

void show_matrix(matrix_t *m)
{
	printf("(");

	int i, j;
	for (i = 0; i < m->rows; i++) {
		printf("\n    ");
		for (j = 0; j < m->columns; j++) {
			printf("%f,", m->data[i * m->columns + j]);
		}
	}
	printf("\n)\n");
}

void compare_matrix(matrix_t *a, matrix_t *b)
{
	if (a->columns != b->columns || a->rows != b->rows) {
		printf("Dimensions differ, cannot compare.\n");
		return;
	}

	int i, j, found = 0;
	for (i = 0; i < a->rows; i++) {
		for (j = 0; j < a->columns; j++) {
			double d = fabs(a->data[i * a->columns + j] - b->data[i * a->columns + j]);
			if (d > MAXDIFF) {
				printf("Found difference of %f at [%d,%d].\n", d, i, j);
				found=1;
			}
		}
	}
	if (!found)
		printf("Identical.\n");
}

matrix_t *load_matrix(char *path)
{
	int d = open(path, O_RDONLY);

	if (d == -1) {
		fprintf(stderr, "load_matrix(): could not open %s\n", path);
		exit(1);
	}

	char header[HEADERLENGTH];
	int rows, columns;
	ssize_t r;

	r = read(d, &header, HEADERLENGTH * sizeof(char));
	r += read(d, &rows, sizeof(int));
	r += read(d, &columns, sizeof(int));

	if (r != HEADERLENGTH * sizeof(char) + 2 * sizeof(int)) {
		fprintf(stderr, "load_matrix(): error reading %s\n", path);
		exit(1);
	}

	if (strncmp(header, HEADER, HEADERLENGTH) != 0) {
		fprintf(stderr, "load_matrix(): error reading %s: wrong header\n", path);
		exit(1);
	}

	matrix_t *m = allocate_matrix(rows, columns);

	r = read(d, m->data, m->rows * m->columns * sizeof(double));
	if (r != m->rows * m->columns * sizeof(double)) {
		fprintf(stderr, "load_matrix(): error reading %s\n", path);
		exit(1);
	}

	close(d);

	return m;
}

void store_matrix(matrix_t *m, char *path)
{
	int d = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);

	if (d == -1) {
		fprintf(stderr, "store_matrix(): could not open %s\n", path);
		exit(1);
	}

	ssize_t r;

	r = write(d, &m->header, HEADERLENGTH * sizeof(char));
	r += write(d, &m->rows, sizeof(int));
	r += write(d, &m->columns, sizeof(int));
	r += write(d, m->data, m->rows * m->columns * sizeof(double));

	if (r != HEADERLENGTH * sizeof(char) + 2 * sizeof(int) + m->rows * m->columns * sizeof(double)) {
		fprintf(stderr, "store_matrix(): error writing %s\n", path);
		exit(1);
	}

	close(d);
}

matrix_t *multiply_matrix(matrix_t *a, matrix_t *b)
{
	if (a->columns != b->rows) {
		fprintf(stderr, "multiply_matrix(): cannot multiply\n");
		exit(1);
	}

	matrix_t *c = allocate_matrix(a->rows, b->columns);

	__multiply_matrix(c, a, b);

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
