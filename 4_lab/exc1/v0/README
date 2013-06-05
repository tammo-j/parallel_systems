Core functionality is provided by matrix.c.

The other c-files provide examples of the usage:

generate_random_matrix.c	Generate a matrix and store it to disk.
multiply_matrix.c		Load two matrices from disk, multiply them, write result to disk.
show_matrix.c			Load matrix from disk and display it.
compare_matrix.c		Load two matrices from disk and compare them.


The used disk file layout is binary:
	Header (16 bytes):
		Offset 0: Null-terminated identifier (8 characters) "PP-SS13"
		Offset 8: number of rows (int, 4 bytes)
		Offset 12: number of columns (int, 4 bytes)
	Data:
		Offset 16: double precision data in row-major order (double, rows*columns*8 bytes)

Memory layout of the data is identical.
