#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include <CL/opencl.h>

#define NUM_GROUPS          3
#define NUM_ITEMS_PER_GROUP 3
#define NUM_ELEMS_PER_ITEM  10
#define NUM_TOTAL_ITEMS     NUM_GROUPS * NUM_ITEMS_PER_GROUP
#define NUM_TOTAL_ELEMS     NUM_ELEMS_PER_ITEM * NUM_TOTAL_ITEMS

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
	
	cl_kernel kernel = clCreateKernel(program, "cumulate", NULL);
	assert(kernel != NULL);

	// Create memory objects
	cl_mem cl_arr;
	cl_arr = clCreateBuffer(ctx, CL_MEM_READ_WRITE, NUM_TOTAL_ELEMS * sizeof(cl_int), NULL, NULL);
	
	// Prepare data on host + transfer to device
	cl_int arr[NUM_TOTAL_ELEMS];

	for (i = 0; i < NUM_TOTAL_ELEMS; ++i)
		arr[i] = 1;
	
	clEnqueueWriteBuffer(cq, cl_arr, CL_TRUE, 0, NUM_TOTAL_ELEMS * sizeof(cl_int), arr, 0, NULL, NULL);

	cl_int elems_per_item = NUM_ELEMS_PER_ITEM;

	// Bind kernel arguments + execute kernel
	clSetKernelArg(kernel, 0, sizeof(cl_mem), &cl_arr);
	
	size_t global_work_sizes[] = { NUM_TOTAL_ITEMS };
	const size_t local_work_sizes[] = { NUM_ITEMS_PER_GROUP };
	status = clEnqueueNDRangeKernel(cq, kernel, 1, NULL, global_work_sizes, local_work_sizes, 0, NULL, NULL);
	assert(status == CL_SUCCESS);
	clFinish(cq);
	
	// Transfer results back + verify
	clEnqueueReadBuffer(cq, cl_arr, CL_TRUE, 0, NUM_TOTAL_ELEMS * sizeof(*arr), arr, 0, NULL, NULL);
	
	for (i = 0; i < NUM_TOTAL_ELEMS; i++) {
		printf("%d ", arr[i]);
	}
	
	// Free everything
	clReleaseMemObject(cl_arr);
	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseCommandQueue(cq);
	clReleaseContext(ctx);
	
	free(source);
	
	return 0;
}

