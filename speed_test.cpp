/***
    This file is part of 'isef-lbm'. Program for simulation of acoustic
    waves using Lattice Boltzmann Method.
    
    Copyright (C) 2013  Arkadiy Prigojin, Sergey Dukanov

    'isef-lbm' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***/ 

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
