#ifndef GRID_H
#define GRID_H


#define GRID_TOP          0
#define GRID_BOTTOM       1
#define GRID_LEFT         2
#define GRID_RIGHT        3
#define GRID_TOP_LEFT     4
#define GRID_TOP_RIGHT    5
#define GRID_BOTTOM_LEFT  6
#define GRID_BOTTOM_RIGHT 7

void grid_init(int width, int height);
void grid_get(int *x, int *y);

int  grid_neighbors();
int  grid_swap_dir(int dir);

int  grid_grid2id(int x, int y);
void grid_id2grid(int id, int *x, int *y);

int  grid_dir2id(int dir);
int  grid_id2dir(int id);


#endif // GRID_H
