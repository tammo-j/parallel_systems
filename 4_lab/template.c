#include <assert.h>
#include <stdio.h>
#include <time.h>

#include <CL/opencl.h>

#define WIDTH (1024*1024)

const char *source =
""
"\n__kernel void vec_add(__global int *c, __global int *a, __global int *b)"
"\n{"
"\n	int x = get_global_id(0);"
"\n	c[x] = a[x] + b[x];"
"\n}"
;



void printBuildError(cl_program program, cl_device_id device)
{
	size_t required_mem_size;
	char *build_log;
	clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &required_mem_size);
	build_log = malloc(required_mem_size);
	clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, required_mem_size, build_log, NULL);
	fprintf(stderr, "%s\n", build_log);
	free(build_log);
}

int main(int argc, char **argv)
{
	int i;
	cl_int status;
	cl_device_id mydevice;

	/* Query and select device */
	cl_uint num_devices;
	cl_platform_id platform;
	const int max_devices = 8;
	cl_device_id device[max_devices];

	clGetPlatformIDs(1, &platform, NULL);
	clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, max_devices, device, &num_devices);

	if (num_devices > max_devices)
		num_devices = max_devices;

	struct timespec t;
	clock_gettime(CLOCK_REALTIME, &t);
	srand(t.tv_nsec);
	i = (int)((float)num_devices * rand() / (RAND_MAX + 1.0));
	mydevice = device[i];
	printf("Selected device %d of %d\n", i + 1, num_devices);

	/* Setup context + command queue with selected device */
	cl_int errcode;
	cl_context ctx;
	cl_context_properties *ctx_prop;
	ctx_prop = (cl_context_properties *)calloc(3, sizeof(*ctx_prop));
	ctx_prop[0] = CL_CONTEXT_PLATFORM;
	ctx_prop[1] = (cl_context_properties)platform;
	ctx = clCreateContext(ctx_prop, 1, &mydevice, NULL, NULL, &errcode);
	assert(ctx != NULL);

	cl_command_queue cq;
	cq = clCreateCommandQueue(ctx, mydevice, 0, NULL);
	assert(cq != NULL);

	/* Compile + create kernel */
	cl_program program;
	program = clCreateProgramWithSource(ctx, 1, &source, NULL, NULL);
	assert(program != NULL);

	const char *build_opts = NULL;
	status = clBuildProgram(program, 1, &mydevice, build_opts, NULL, NULL);
	if (status != CL_SUCCESS)
		printBuildError(program, mydevice);
	assert(status == CL_SUCCESS);

	cl_kernel kernel = clCreateKernel(program, "vec_add", NULL);
	assert(kernel != NULL);

	/* Create memory objects */
	cl_mem da, db, dc;
	da = clCreateBuffer(ctx, CL_MEM_READ_ONLY, sizeof(cl_int) * WIDTH, NULL, NULL);
	db = clCreateBuffer(ctx, CL_MEM_READ_ONLY, sizeof(cl_int) * WIDTH, NULL, NULL);
	dc = clCreateBuffer(ctx, CL_MEM_WRITE_ONLY, sizeof(cl_int) * WIDTH, NULL, NULL);
	assert(da != NULL);
	assert(db != NULL);
	assert(dc != NULL);


	/* Prepare data on host + transfer to device */
	cl_int *a = (cl_int *)malloc(sizeof(*a) * WIDTH);
	cl_int *b = (cl_int *)malloc(sizeof(*b) * WIDTH);
	cl_int *c = (cl_int *)malloc(sizeof(*c) * WIDTH);
	for (i = 0; i < WIDTH; i++) {
		a[i] = i;
		b[i] = 2 * i + 5;
		c[i] = 0;
	}

	clEnqueueWriteBuffer(cq, da, CL_TRUE, 0, WIDTH * sizeof(*a), a, 0, NULL, NULL);
	clEnqueueWriteBuffer(cq, db, CL_TRUE, 0, WIDTH * sizeof(*b), b, 0, NULL, NULL);

	/* Bind kernel arguments + execute kernel */
	clSetKernelArg(kernel, 0, sizeof(cl_mem), &dc);
	clSetKernelArg(kernel, 1, sizeof(cl_mem), &da);
	clSetKernelArg(kernel, 2, sizeof(cl_mem), &db);

	size_t global_work_sizes[] = { WIDTH };
	const size_t local_work_sizes[] = { 256 };
	status = clEnqueueNDRangeKernel(cq, kernel, 1, NULL, global_work_sizes, local_work_sizes, 0, NULL, NULL);
	assert(status == CL_SUCCESS);
	clFinish(cq);

	/* Transfer results back + verify */
	clEnqueueReadBuffer(cq, dc, CL_TRUE, 0, WIDTH * sizeof(*c), c, 0, NULL, NULL);

	for (i = 0; i < WIDTH; i++) {
		if (c[i] != a[i] + b[i]) {
			printf("Verification failed!\n");
			break;
		}
	}
	if (i == WIDTH)
		printf("Verification successful!\n");

	/* Free everything */
	clReleaseMemObject(da);
	clReleaseMemObject(db);
	clReleaseMemObject(dc);
	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseCommandQueue(cq);
	clReleaseContext(ctx);

	free(a);
	free(b);
	free(c);

	return 0;
}
