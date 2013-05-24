
__kernel void vec_add(__global int *c, __global int *a, __global int *b) {
	int x = get_global_id(0);
	c[x] = a[x] + b[x];
}

