#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>
#include <math.h>

int main (int argc, char **argv)  {
	int arr[] = {0, 1, 1, 1, 1, 1, 1, 1, 1};
	int len = sizeof(arr) / sizeof(*arr);
	
	int temp[len];
	int *src = arr;
	int *dst = temp;
	
	memset(temp, 0, sizeof(temp));
	
	int n = len-1;
	int m = floor( log(n) / log(2.) );
	
	printf("n=%d, m=%d\n", n, m);
	
	for (int i = 1; i <= m; ++i) {
		int offset = floor( pow(2., i-1) );
		
		#pragma omp parallel for
		for (int j = 1; j <= n; ++j) {
			if (j < offset)
				dst[j] = src[j];
			else
				dst[j] = src[j-offset] + src[j];
			
//			printf("%d: dst[%d] = src[%d] + src[%d] == %d = %d + %d\n", omp_get_thread_num(), j, j-offset, j, dst[j], src[j-offset], src[j]);
		}
		
		int *swap_temp = src;
		src = dst;
		dst = swap_temp;
	}
	
	for (int i = 0; i < len; ++i)
		printf("arr[%d] = %d\n", i, src[i]);
	
	return EXIT_SUCCESS;
}
