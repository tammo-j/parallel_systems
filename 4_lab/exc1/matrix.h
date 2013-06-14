#ifndef _MATRIX_H
#define _MATRIX_H

#define HEADER		"PP-SS13\0"
#define HEADERLENGTH	8

#define MAXDIFF		0.000001

typedef struct {
	char header[HEADERLENGTH];
	int rows;
	int columns;
	double *data;
} matrix_t;

matrix_t *allocate_matrix(int rows, int columns);
void free_matrix(matrix_t *m);

matrix_t *generate_random_matrix(int rows, int columns);

void show_matrix(matrix_t *m);
void compare_matrix(matrix_t *a, matrix_t *b);

matrix_t *load_matrix(char *path);
void store_matrix(matrix_t *m, char *path);

matrix_t *multiply_matrix(matrix_t *a, matrix_t *b);
void __multiply_matrix(matrix_t *c, matrix_t *a, matrix_t *b);

#endif
