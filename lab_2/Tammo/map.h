// map sizes
#define HEIGHT 10
#define WIDTH  30
// the chanche that a initialized cell is alived is 1:density
#define DENSITY 4

typedef struct
{
	int x;
	int y;
} cell_t;

typedef struct map
{
	cell_t cell;
	struct map* next;
} map_t;

typedef struct map_list
{
	struct map_list* head;
	struct map_list* tail;
	struct map_list* curr;
} map_list_t;

int append(map_t map, int x, int y);
