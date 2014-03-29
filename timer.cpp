/*
	В этом файле содержатся функции работы о временем.
*/
#include "timer.h"
#include <cstddef>
#include <omp.h>

void Timer :: start_timer()
{
	start = omp_get_wtime();
}

double Timer :: time_passed()
{
	return omp_get_wtime() - start;
}
