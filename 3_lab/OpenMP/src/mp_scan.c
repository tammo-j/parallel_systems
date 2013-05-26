#include <omp.h>
#include <stdio.h>
#include <math.h>

main ()  {
	int i, tid, iCalc, chunk, iSerialCount, iParallelCount;

	int iInput[9]= { 0 , 1, 2, 3, 4, 5, 6, 7, 8 };
	chunk = 2;

	omp_set_dynamic(0);
	omp_set_num_threads(omp_get_max_threads());

	for (iSerialCount = 1; iSerialCount <= 3; iSerialCount++)
	{
#pragma omp parallel shared(iInput,iSerialCount,chunk) private(iParallelCount, tid, iCalc)
	{

	#pragma omp for schedule(dynamic,chunk) nowait
		for (iParallelCount=1; iParallelCount <= 8; iParallelCount++)
		{
			iCalc = pow(2.0,iSerialCount-1);
			if ( iParallelCount  >= iCalc )
			{
				tid = omp_get_thread_num();
				printf("ThreadNumber: %d\n", tid);
				printf("SerielCount: %d\n", iSerialCount);
				printf("2^(iSerialCount-1): %d\n", iCalc);
				printf("ParallelCount: %d\n", iParallelCount);
				printf("Input before computation: %d\n", iInput[iParallelCount]);

				iInput[iParallelCount] = iInput[(iParallelCount - iCalc)] + iInput[iParallelCount];

				printf("Input after computation: %d\n", iInput[iParallelCount]);
				printf("\n");
			}
		}
			
	}/* end of parallel section */
	}/* end of serial for */

	scanf("%d",&i);
}