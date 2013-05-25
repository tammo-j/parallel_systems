#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include <CL/opencl.h>

#define NUM_ELEMS_PER_ITEM  15
#define NUM_ITEMS_PER_GROUP 4
#define NUM_GROUPS          5
#define NUM_ELEMS_PER_GROUP NUM_ELEMS_PER_ITEM  * NUM_ITEMS_PER_GROUP
#define NUM_ITEMS     NUM_ITEMS_PER_GROUP * NUM_GROUPS 
#define NUM_ELEMS     NUM_ELEMS_PER_GROUP * NUM_GROUPS

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
	
	// Query and select device
	cl_uint num_devices;
	cl_platform_id platform;
	const int max_devices = 8;
	cl_device_id device[max_devices];
	
	clGetPlatformIDs(1, &platform, NULL);
	clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, max_devices, device, &num_devices);
	
	if (num_devices > max_devices)
		num_devices = max_devices;
	
	time_t t = time(NULL);
	srand(t);
	i = (int)((float)num_devices * rand() / (RAND_MAX + 1.0));
	mydevice = device[i];
	printf("Selected device %d of %d\n", i + 1, num_devices);
	
	// Setup context + command queue with selected device
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
	
	// Get Kernel from File
	FILE *kernel_file = fopen("./kernel.cl","r"); 
	assert(kernel_file != NULL); 
	fseek(kernel_file, 0L, SEEK_END); 
	size_t kernel_size = ftell(kernel_file); 
	rewind(kernel_file); 
	char *source = (char*) calloc(sizeof(char), kernel_size+1); 
	fread(source, kernel_size, 1, kernel_file); 
	assert(!ferror(kernel_file)); 
	fclose(kernel_file);
	
	// Compile + create kernel
	cl_program program;
	program = clCreateProgramWithSource(ctx, 1, (const char**)&source, NULL, NULL);
	assert(program != NULL);
	
	const char *build_opts = NULL;
	status = clBuildProgram(program, 1, &mydevice, build_opts, NULL, NULL);
	if (status != CL_SUCCESS)
		printBuildError(program, mydevice);
	assert(status == CL_SUCCESS);
	
	cl_kernel cumulate = clCreateKernel(program, "cumulate", NULL);
	cl_kernel add_offset = clCreateKernel(program, "add_offset", NULL);
	assert(cumulate != NULL);
	assert(add_offset != NULL);

	// Create memory objects
	cl_mem cl_arr, cl_item_offsets, cl_group_offsets;
	cl_arr     = clCreateBuffer(ctx, CL_MEM_READ_WRITE, NUM_ELEMS * sizeof(cl_int), NULL, NULL);
	cl_item_offsets = clCreateBuffer(ctx, CL_MEM_READ_WRITE, NUM_ITEMS * sizeof(cl_int), NULL, NULL);
	cl_group_offsets = clCreateBuffer(ctx, CL_MEM_READ_ONLY, NUM_ITEMS * sizeof(cl_int), NULL, NULL);
	
	
	// Prepare data on host + transfer to device
	cl_int arr[NUM_ELEMS];
	cl_int item_offsets[NUM_ITEMS];
	cl_int group_offsets[NUM_GROUPS];

	for (i = 0; i < NUM_ELEMS; ++i)
		arr[i] = 1;
	
	clEnqueueWriteBuffer(cq, cl_arr, CL_TRUE, 0, NUM_ELEMS * sizeof(*arr), arr, 0, NULL, NULL);

	cl_int elems_per_item = NUM_ELEMS_PER_ITEM;

	// Bind kernel arguments + execute kernel
	clSetKernelArg(cumulate, 0, sizeof(cl_mem), &cl_arr);
	clSetKernelArg(cumulate, 1, sizeof(cl_mem), &cl_item_offsets);
	clSetKernelArg(add_offset, 0, sizeof(cl_mem), &cl_arr);
	clSetKernelArg(add_offset, 1, sizeof(cl_mem), &cl_item_offsets);
	clSetKernelArg(add_offset, 2, sizeof(cl_mem), &cl_group_offsets);
	
	size_t global_work_sizes[] = { NUM_ITEMS };
	const size_t local_work_sizes[] = { NUM_ITEMS_PER_GROUP };
	status = clEnqueueNDRangeKernel(cq, cumulate, 1, NULL, global_work_sizes, local_work_sizes, 0, NULL, NULL);
	assert(status == CL_SUCCESS);
	clFinish(cq);

	clEnqueueReadBuffer(cq, cl_item_offsets, CL_TRUE, 0, NUM_ITEMS * sizeof(*item_offsets), item_offsets, 0, NULL, NULL);
	int acc = 0;
	for (int i = 0; i < NUM_GROUPS; ++i) {
		acc += item_offsets[(i+1)*NUM_ITEMS_PER_GROUP-1];
		group_offsets[i] = acc;

		printf("group %d: %d\n", i, acc);
	}

	clEnqueueWriteBuffer(cq, cl_group_offsets, CL_TRUE, 0, NUM_GROUPS * sizeof(*group_offsets), group_offsets, 0, NULL, NULL);

	status = clEnqueueNDRangeKernel(cq, add_offset, 1, NULL, global_work_sizes, local_work_sizes, 0, NULL, NULL);
	assert(status == CL_SUCCESS);
	clFinish(cq);
	
	// Transfer results back + verify
	clEnqueueReadBuffer(cq, cl_arr, CL_TRUE, 0, NUM_ELEMS * sizeof(*arr), arr, 0, NULL, NULL);
	
	for (i = 0; i < NUM_ELEMS; i++) {
		printf("%d: %d\n", i, arr[i]);
	}
	
	// Free everything
	clReleaseMemObject(cl_arr);
	clReleaseMemObject(cl_item_offsets);
	clReleaseMemObject(cl_group_offsets);
	clReleaseKernel(cumulate);
	clReleaseKernel(add_offset);
	clReleaseProgram(program);
	clReleaseCommandQueue(cq);
	clReleaseContext(ctx);
	
	free(source);
	
	return 0;
}

