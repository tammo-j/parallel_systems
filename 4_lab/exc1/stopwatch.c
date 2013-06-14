#include "stopwatch.h"

#include <time.h>

static clock_t g_total_time, g_start_time;

void stopwatch_reset() {
	g_total_time = 0;
}

void stopwatch_start() {
	g_start_time = clock();
}

void stopwatch_stop() {
	g_total_time += clock() - g_start_time;
}

double stopwatch_get() {
	return (double) g_total_time / CLOCKS_PER_SEC;
}

