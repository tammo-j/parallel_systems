
#define NUM_ELEMS_PER_ITEM 10
#define NUM_ITEMS_PER_GROUP 3
#define NUM_GROUPS 3
#define NUM_ELEMS_PER_GROUP NUM_ELEMS_PER_ITEM * NUM_ITEMS_PER_GROUP
#define NUM_TOTAL_ITEMS NUM_GROUPS * NUM_ITEMS_PER_GROUP
#define NUM_TOTAL_ELEMS NUM_ELEMS_PER_ITEM * NUM_ITEMS_PER_GROUP

__kernel void cumulate(__global int *global_sum) {
	int global_id = get_global_id(0);
	int local_id = get_local_id(0);
	int group_id = get_group_id(0);
	int offset = global_id * NUM_ELEMS_PER_ITEM;
	
	int acc;

	__private int item_sum[NUM_ELEMS_PER_ITEM];
	__local   int group_sum[NUM_ITEMS_PER_GROUP];

	acc = 0;
	for (int i = 0; i < NUM_ELEMS_PER_ITEM; ++i) {
		acc += global_sum[offset + i];
		item_sum[i] = acc;
	}
	group_sum[local_id] = acc;

	barrier(CLK_LOCAL_MEM_FENCE);

	if (local_id == 0) {
		acc = 0;
		for (int i = 0; i < NUM_ITEMS_PER_GROUP; ++i) {
			acc += group_sum[i];
			group_sum[i] = acc;
		}
		global_sum[(group_id+1) * NUM_ELEMS_PER_GROUP - 1] = acc;
	}

	barrier(CLK_GLOBAL_MEM_FENCE);

	if (global_id == 0) {
		acc = 0;
		for (int i = NUM_ELEMS_PER_GROUP-1; i < NUM_TOTAL_ELEMS; i += NUM_ELEMS_PER_GROUP) {
			acc += global_sum[i];
			global_sum[i] = acc;
		}
	}

	barrier(CLK_GLOBAL_MEM_FENCE);

	int sum_offset = 0;
	
	if (group_id > 0)
		sum_offset += global_sum[(group_id+1) * NUM_ELEMS_PER_GROUP - 1];
	if (local_id > 0)
		sum_offset += group_sum[local_id-1];
	
	for (int i = 0; i < NUM_ELEMS_PER_ITEM; ++i)
		global_sum[offset + i] = sum_offset + item_sum[i];
}

