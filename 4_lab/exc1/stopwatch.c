#include "stopwatch.h"

#include <omp.h>

static double g_total_time, g_start_time, g_lap_time;

void stopwatch_reset() {
	g_total_time = 0.;
}

void stopwatch_start() {
	g_start_time = omp_get_wtime();
}

void stopwatch_stop() {
	g_lap_time = omp_get_wtime() - g_start_time;
	g_total_time += g_lap_time;
}

double stopwatch_get() {
	return g_total_time;
}

double stopwatch_lap() {
	return g_lap_time;
}

