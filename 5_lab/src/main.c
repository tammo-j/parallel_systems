#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <omp.h>

#include "fifo_a.h"

// http://gcc.gnu.org/onlinedocs/gcc-4.1.2/gcc/Atomic-Builtins.html

int main( int argc, char **argv ) {
	fifo_queue_t q;
	fifo_init(&q);

	#pragma omp parallel
	#pragma omp single nowait
	{
		int i;
		for(i=1;i<5;++i) {
			#pragma omp task
			{
				int j;
				for(j = 0; j < 1000; ++j) {
					fifo_enqueue(&q, i*1000+j);
				}
			}
			#pragma omp task
			{
				int d, j;
				for(j = 0; j < 1000; ++j) {
					d = fifo_dequeue(&q);
					if (d)
						printf("dequeue %d\n", d);
				}
			}
		}
	}
	
	int d;
	while (true) {
		d = fifo_dequeue(&q);
		if (d == -1)
			break;
		printf("dequeue %d\n", d);
	}
	assert(fifo_empty(&q));
	
	#pragma omp taskwait
	
	fifo_free(&q);
	
	return 0;
}
