/***
    This file is part of 'isef-lbm'. Program for simulation of acoustic
    waves using Lattice Boltzmann Method.
    
    Copyright (C) 2013  Arkadiy Prigojin, Sergey Dukanov

    'isef-lbm' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'isef-lbm' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***/ 

#include "field.h"
#include "constants.h"
#include "timer.h"
#include <stdio.h>
#include <math.h>

void write_cir_ver_results_to_file(int directions, float *dist);
void write_per_ver_results_to_file(int steps, float *val);
int find_period(int n, float* val);

void Field :: constant_frequency_verification()
{
	const int n = Nx / 2;
	const int steps = Nx / 2 / cs;
	const int L = 30;
	int x = Nx/2, y = Ny/2;
	float* val;
	val = new float[n];
	clear_liquids();
	clear_solids();
	set_computing_type(CPU);
	Emitter e(f, Nx, Ny);
	e.set_point_position(x, y);
	e.set_period(L);
	e.create_standart_wave(SIN_EMIT);
	emitters.push_back(e);
	for(int i = 0; i < steps; ++i)
		next_step();
	for(int i = 0; i < n; ++i)
		val[i] = rho[(x+i) * Ny + y];
	
	delete val;
}

void Field :: periodicity_verification()
{
	const int n = 120;
	int x, y, L;
	float* val; 
	val = new float[n];
	x = Nx/2;
	y = Ny/2;
	L = 40;
	clear_liquids();
	clear_solids();
	set_computing_type(CPU);
	Emitter e(f, Nx, Ny);
	e.set_point_position(Nx/2, Ny/2);
	e.set_period(L);
	e.create_standart_wave(SIN_EMIT);
	emitters.push_back(e);
	x += L;
	for(int i = 0; i < n; ++i)
	{
		next_step();
		val[i] = rho[x*Ny + y];
	}
	write_per_ver_results_to_file(n, val);
}

void Field :: wave_damping()
{
	float A = 1.0;
	
}

void Field :: circle_verification()
{
	const int n = 50;
	const int directions = 16;
	int x0 = Nx/2;
	int y0 = Ny/2;
	int x, y;
	float max_val_on_dir, val;
	float dist_to_max_val;
	float angle;
	float* dist;
	dist = new float[directions];
	clear_liquids();
	clear_solids();
	set_computing_type(CPU);
	rho_increase(x0, y0, 1.0);
	for(int i = 0; i < n; ++i)
		next_step();
	for(int i = 0; i < directions; ++i)
	{
		max_val_on_dir = 0.0;
		angle = 2 * M_PI * i / directions;
		for(float R = 0.0; R < n; R += 0.01)
		{
			x = x0 + cos(angle) * R;
			y = y0 + sin(angle) * R;
			val = rho[x*Ny + y];
			if(val > max_val_on_dir)
			{
				max_val_on_dir = val;
				dist_to_max_val = R;
			}
		}
		dist[i] =  dist_to_max_val;
	}
	write_cir_ver_results_to_file(directions, dist);
}

void write_cir_ver_results_to_file(int directions, float *dist)
{
	float average_dist;
	float deviation;
	FILE* file;
	file = fopen("circle_verification.txt", "w");
	fprintf(file, 
		"------------------Circle verification----------------\n");
	fprintf(file, 
		"Distances to the front of the wave from source point.\n");
	fprintf(file, "Number of directions: %d;\n", directions);
	// finding average distance
	average_dist = 0.0;
	for(int i = 0; i < directions; ++i)
		average_dist += dist[i];
	average_dist /= directions;
	fprintf(file, "Average distance: %0.5f;\n", average_dist);
	fprintf(file, "╔====╤==========╤==============================╗\n");
	fprintf(file, "║ №  │ Distance │ Deviation from average value ║\n");
	fprintf(file, "╠====╪==========╪==============================╣\n");
	for(int i = 0; i < directions; ++i)
	{
		deviation = dist[i] - average_dist;
		deviation *= (deviation < 0.0? -1.0 : 1.0);
		fprintf(file, "║ %2d │ %2.5f │ %2.5f                      ║\n", 
			i+1, dist[i], deviation);
	}
	fprintf(file, "╚====╧==========╧==============================╝\n");
	fprintf(file, 
		"-----------------------------------------------------\n");
	fclose(file);
}

void write_per_ver_results_to_file(int n, float *val)
{
	int period = find_period(n, val);
	FILE* file;
	file = fopen("periodicity_verification.txt", "w");
	fprintf(file, 
		"---------------Periodicity verification--------------\n");
	fprintf(file, "period = %d;\n", period);
	fprintf(file, "t = k * period + i;\n");
	fprintf(file, "k ∈ ℕ; i ∈ {0; 1; ..; %d};\n", period-1);
	for(int i = 0; i < period; ++i)
	{
		fprintf(file, "Values: i = %d;", i);
		for(int k = 0; k < n / period; ++k)
		{
			int t = k * period + i;
			if(!(k%3))
				fprintf(file, "\n               ");
			fprintf(file, "%0.5f  ", val[t]);
		}
		fprintf(file, "\n");
	}
	fprintf(file, 
		"-----------------------------------------------------\n");
}

int find_period(int n, float* val)
{
	int period = -1;
	// Finding maximum values
	for(int i = 1; i < n-1; ++i)
	{
		if(val[i] > val[i-1] && 
		   val[i] > val[i+1])
		{
			if(period < 0)
				period = i;
			else 
				return i - period;
		}
	}
	return 0;
}


void Field :: testing_of_speed()
{
	const int n_of_i = 50;
	const int start = 10;
	const int step = 30;
	const int finish = 1000;
	float time;
	Timer timer;
	printf("Speed test.\n");
	printf("CPU:\n");
	set_computing_type(CPU);
	printf("Size  Time\n");
       	for(int i = start; i <= finish; i += step)
	{
		resize(i, i);
		timer.start_timer();
		for(int j = 0; j < n_of_i; ++j)
			next_step();
		time = timer.time_passed() / float(n_of_i);
		printf("%3d %0.8f\n", i, time); 
	}
	printf("CPU_PARALLEL:\n");
	set_computing_type(CPU_PARALLEL);
       	for(int i = start; i <= finish; i += step)
	{
		resize(i, i);
		timer.start_timer();
		for(int j = 0; j < n_of_i; ++j)
			next_step();
		time = timer.time_passed() / float(n_of_i);
		printf("%3d %0.8f\n", i, time); 
	}
	printf("Done.\n");
}
