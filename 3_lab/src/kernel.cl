
#define NUM_ELEMS_PER_ITEM  15
#define NUM_ITEMS_PER_GROUP 4
#define NUM_GROUPS          5

__kernel void cumulate(__global int *arr, __global int *item_offsets) {
	// local cumulated sum of this work group
	__local int group_sum[NUM_ITEMS_PER_GROUP];
	
	// some ids
	int global_id = get_global_id(0);
	int local_id = get_local_id(0);
	int group_id = get_group_id(0);
	
	// starting point of the input array for this work item
	int arr_offset = global_id * NUM_ELEMS_PER_ITEM;
	
	// STEP 1: scan on work item level

	// cumulate sum on work item level
	int acc = 0;
	for (int i = 0; i < NUM_ELEMS_PER_ITEM; ++i)
		acc = arr[arr_offset + i] += acc;
	
	// save total sum of this work item
	group_sum[local_id] = acc;
	
	barrier(CLK_LOCAL_MEM_FENCE);
	
	// STEP 2: scan on work group level

	// for one item of each work group
	if (local_id == 0) {
		// cumulate the total sum of each work item
		acc = 0;
		for (int i = 0; i < NUM_ITEMS_PER_GROUP; ++i) {
			acc += group_sum[i];
			item_offsets[group_id*NUM_ITEMS_PER_GROUP + i] = acc;
		}
	}
}

__kernel void add_offset(__global int *arr, __global int *item_offsets, __global int *group_offsets) {
	int global_id  = get_global_id(0);
	int group_id   = get_group_id(0);
	int local_id   = get_local_id(0);
	int arr_offset = global_id * NUM_ELEMS_PER_ITEM;
	
	// STEP 3: merge results

	int offset = 0;

	if (local_id > 0)
		offset += item_offsets[global_id-1];
	if (group_id > 0)
		offset += group_offsets[group_id-1];

	for (int i = 0; i < NUM_ELEMS_PER_ITEM; ++i)
		arr[arr_offset + i] += offset;
}
