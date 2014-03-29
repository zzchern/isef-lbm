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

/*
  Класс исследования структуры поля на наличие 
  наиболее удачного расположения источника звука.
  Функции:
  - Сохранение в отдельную папку изображений 
  sound-mp для каждого источника
   
  Опции:
  - Проводить анализ по различным частотам
  - В таблице поля: 
  * max и min
  * процентное содержание "глухих" областей
 */

#include <stdio.h>
#include <math.h>
#include "constants.h"
#include "research_acoustic_source.h"

Research_AS :: Research_AS(Field *f) :
	field(f)
{
	int nx, ny, n;
	f -> get_lattice_size(nx, ny);
	n = sqrt(nx*nx + ny*ny);
	set_number_of_iterations(n);
	rated = false;
}

void Research_AS :: add_acoustic_system(int number_of_sources, Point* positions)
{
	Acoustic_System AS;
	Acoustic_Source source;
	int *cell;
	int nx, ny;
	cell = field -> get_lattice_object();
	field -> get_lattice_size(nx, ny);
	for(int i = 0; i < number_of_sources; ++i)
	{
		source.x = positions[i].x;
		source.y = positions[i].y;
	       	if(cell[source.x * ny + source.y] == LIQUID)
			AS.sources.push_back(source);
	}
	AS.mean_val = 0.0;
	AS.max_val = 0.0;
	AS.min_val = 0.0;
	AS.dev_val = 0.0;
	AS.R1 = 0;
	AS.R2 = 0;
	systems.push_back(AS);
}

void Research_AS :: add_default_acoustic_system(int grid_size)
{
	Point* positions;
	int indent_x, distance_x;
	int indent_y, distance_y;
	int nx, ny;
	int x, y, index;
	field -> get_lattice_size(nx, ny);
	distance_x = nx / grid_size;
	distance_y = ny / grid_size;
	indent_x = distance_x / 2;
	indent_y = distance_y / 2;
	positions = new Point[grid_size * grid_size];
	for(int i = 0; i < grid_size; ++i)
	for(int j = 0; j < grid_size; ++j)
	{
		index = i * grid_size + j;
		x = indent_x + i * distance_x;
		y = indent_y + j * distance_y;
		positions[index].x = x;
		positions[index].y = y;
	}
	add_acoustic_system(grid_size * grid_size, positions);
	delete positions;
}

void Research_AS :: set_number_of_iterations(int n)
{
	n = (n <= 0 ? 10 : n);
	number_of_iterations = n;
}

void Research_AS :: analyze_acoustic_systems()
{
	int x, y;
	char buf[40];
	field -> set_computing_type(CPU_PARALLEL);
	field -> sound_map -> work(true);
	printf("Acoustic sourse research.\n");
	printf("Wait, please.\n");
	for(int i = 0; i < systems.size(); ++i)
	{
		field -> clear_liquids();
		field -> sound_map -> clear_map();
		for(int j = 0; j < systems[i].sources.size(); ++j)
		{
			x = systems[i].sources[j].x;
			y = systems[i].sources[j].y;
			field -> rho_increase(x, y, 1.0);
		}
		for(int k = 0; k < number_of_iterations; ++k)
			field -> next_step();
		systems[i].mean_val = field -> sound_map -> 
			find_arithmetic_mean();
	       	systems[i].dev_val = field -> sound_map -> 
		find_average_deviation_from_mean();
		sprintf(buf, "sound_density_map_acoustic_system_%d.bmp", i);
		field -> sound_map -> save_density_map_bmp(buf);
		sprintf(buf, "sound_max_val_map_acoustic_system_%d.bmp", i);
		field -> sound_map -> save_max_val_map_bmp(buf);
		field -> emitters.clear();
	}
	rate();
	printf("Done.\n");
}

// Rating = R1 + R2
// R1 - rating for highest mean value [0; 50]
// R2 - rating for lowest deviation value [0; 50]
void Research_AS :: rate()
{
	float max_mean_val = 0.0;
	float min_mean_val = 1.0;
	float max_dev_val = 0.0;
	float min_dev_val = 1.0;
	for(unsigned int i = 0; i < systems.size(); ++i)
	{
		
		if(systems[i].mean_val > max_mean_val)
			max_mean_val = systems[i].mean_val;
		if(systems[i].mean_val < min_mean_val)
			min_mean_val = systems[i].mean_val;
		if(systems[i].dev_val < min_dev_val)
			min_dev_val = systems[i].dev_val;
		if(systems[i].dev_val > max_dev_val)
			max_dev_val = systems[i].dev_val;
	}
	for(unsigned int i = 0; i < systems.size(); ++i)
	{
		systems[i].R1 = int(
			(systems[i].mean_val - min_mean_val) / 
			(max_mean_val - min_mean_val) * 50
			);
		systems[i].R2 = int(
			(1.0 - (systems[i].dev_val - min_dev_val) /
			 (max_dev_val - min_dev_val)) * 50
			);
	}
	rated = true;
}

void Research_AS :: save_results()
{
}/*	FILE* file;
	file = fopen("research_acoustic_source_result.txt", "w");
	fprintf(file,
"╔================╤====================================╤=================╗\n");
	fprintf(file, 
"║   Emitter:     │ Result: number of iterations = %3d │     Rating      ║\n",
		number_of_iterations);
	fprintf(file, 
"╠====╤=====╤=====╪==============╤=====================╪====╤====╤=======╣\n");
	fprintf(file, 
"║ №  │  X  │  Y  │  Mean value  │ Deviation from mean │ R1 │ R2 │ Total ║\n");
	fprintf(file, 
"╠====╪=====╪=====╪==============╪=====================╪====╪====╪=======╣\n");
	for(unsigned int i = 0; i < as.size(); ++i)
	{
		fprintf(file, 
"║ %2d │ %3d │ %3d │   %0.6f   │  %0.6f           │ %2d │ %2d │ %3d   ║\n", 
			i+1, as[i].x, as[i].y, as[i].mean_val, 
			as[i].dev_val, as[i].R1, as[i].R2, as[i].R1 + as[i].R2);
	}
	fprintf(file, 
"╚====╧=====╧=====╧==============╧=====================╧====╧====╧=======╝\n");
	fclose(file);
}
 */
