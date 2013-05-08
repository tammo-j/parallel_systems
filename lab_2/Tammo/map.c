int append(map_t map, int x, int y)
{

	map_t* map_i = calloc(1, sizeof(map_t));
	cell_t cell = {x, y};
	map_i->cell = cell;
	map_i->next = NULL;
}
