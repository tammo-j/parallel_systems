
#define NUM_ELEMS_PER_ITEM  10
#define NUM_ITEMS_PER_GROUP 3
#define NUM_GROUPS          3
#define NUM_ELEMS_PER_GROUP NUM_ELEMS_PER_ITEM  * NUM_ITEMS_PER_GROUP
#define NUM_TOTAL_ITEMS     NUM_ITEMS_PER_GROUP * NUM_GROUPS 
#define NUM_TOTAL_ELEMS     NUM_ELEMS_PER_GROUP * NUM_GROUPS

__kernel void cumulate(__global int *arr) {
	// private cumulated sum of this work item
	__private int item_sum [NUM_ELEMS_PER_ITEM ];
	// local cumulated sum of this work group
	__local   int group_sum[NUM_ITEMS_PER_GROUP];
	
	// some ids
	int global_id = get_global_id(0);
	int local_id = get_local_id(0);
	int group_id = get_group_id(0);
	
	// starting point of the input array for this work item
	int offset = global_id * NUM_ELEMS_PER_ITEM;
	
	// accumulator
	int acc;
	
	// STEP 1: scan on work item level

	// cumulate sum on work item level
	acc = 0;
	for (int i = 0; i < NUM_ELEMS_PER_ITEM; ++i) {
		acc += arr[offset + i];
		item_sum[i] = acc;
	}
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
			group_sum[i] = acc;
		}
		// save the total sum of this work group

		// Use the arr array as storage so
		// we don't need to get extra global memory.
		// Write at the first position of each group.
		arr[group_id * NUM_ELEMS_PER_GROUP] = acc;
	}
	
	barrier(CLK_GLOBAL_MEM_FENCE);

	// STEP 3: scan on device level

	// for one item on this device
	if (global_id == 0) {
		// cumulate the total sum of each work group
		acc = 0;
		for (int i = 0; i < NUM_TOTAL_ELEMS; i += NUM_ELEMS_PER_GROUP) {
			acc += arr[i];
			arr[i] = acc;
		}
	}
	
	barrier(CLK_GLOBAL_MEM_FENCE);

	// STEP 4: merging results
	
	// offset of the sum of this work item to be added
	int sum_offset = 0;
	
	// add offset of this group
	if (group_id > 0)
		sum_offset += arr[(group_id-1) * NUM_ELEMS_PER_GROUP];
	// add the offset of this work item
	if (local_id > 0)
		sum_offset += group_sum[local_id-1];
	
	// add the overall offset to the private sum
	// and write back to input array
	for (int i = 0; i < NUM_ELEMS_PER_ITEM; ++i)
		arr[offset + i] = sum_offset + item_sum[i];
}

