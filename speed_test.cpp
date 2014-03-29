#include "field.h"
#include "timer.h"
#include "constants.h"
#include <stdio.h>

void Field :: speed_test()
{
	printf("Speed test:\n");
	Timer t;
	int n = 60;
	sound_map -> work(false);
	set_step(15);
	for(int size = 100; size < 401; size += 50)
	{
		printf("Field: %d * %d: ", size, size);
		resize(size, size);
		
		t.start_timer();
		for(int i = 0; i < n; ++i)
			next_step_GPU();
		printf("GPU  : %2.4f ms. ", 
		       t.time_passed() / float(n*compute_n_steps) * 1000.0);

		t.start_timer();
		for(int i = 0; i < n; ++i)
			next_step_CPU();
		printf("CPU  : %2.4f ms. ", 
		       t.time_passed() / float(n*compute_n_steps) * 1000.0);

		t.start_timer();
		for(int i = 0; i < n; ++i)
			next_step_CPU_parallel();
		printf("CPU+ : %2.4f ms.\n", 
		       t.time_passed() / float(n*compute_n_steps) * 1000.0);
	}
}
