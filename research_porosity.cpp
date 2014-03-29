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

#include <stdio.h>
#include <math.h>
#include "research_porosity.h"
#include "constants.h"

Research_Porosity :: Research_Porosity(Field* f) :
	field(f)
{
	int nx, ny;
	field -> get_lattice_size(nx, ny);
	cell = field -> get_lattice_object();
	x_left = 0; // Default
	x_right = nx-1;
	y_top = ny-1;
	y_bot = 0;
	L = 10;
}

void Research_Porosity :: set_area(int x_l, int y_t, int x_r, int y_b)
{
	int nx, ny;
	field -> get_lattice_size(nx, ny);
	x_l = (x_l < x_r) ? (x_l) : (x_r);
	x_r = (x_r > x_l) ? (x_r) : (x_l);
	y_t = (y_t > y_b) ? (y_t) : (y_b);
	y_b = (y_b < y_t) ? (y_b) : (y_t);
	x_l = (x_l >= 0 && x_l < nx) ? (x_l) : (0); 
	x_r = (x_r >= 0 && x_r < nx) ? (x_r) : (nx-1); 
	y_t = (y_t >= 0 && y_t < ny) ? (y_t) : (ny-1); 
	y_b = (y_b >= 0 && y_b < ny) ? (y_b) : (0); 
	printf("Area: A(%d;%d) B(%d;%d);\n", x_l, y_t, x_r, y_b);
	x_left = x_l;
	x_right = x_r;
	y_top = y_t;
	y_bot = y_b;
}

float Research_Porosity :: calculate_porosity_of_area()
{
	int nx, ny;
	int number_of_solids;
	int number_of_objects;
	field -> get_lattice_size(nx, ny);
	number_of_objects = (x_right - x_left) * (y_top - y_bot);
	number_of_solids = 0;
	for(int x = x_left; x <= x_right; ++x)
	for(int y = y_bot; y <= y_top; ++y)
		if(cell[x*ny+y] != LIQUID)
			number_of_solids++;
	return 1.0 - float(number_of_solids) / float(number_of_objects);
	
}

int Research_Porosity :: size_of_surface()
{
	int size = 0;
	int nx, ny;
	field -> get_lattice_size(nx, ny);
	for(int x = x_left; x <= x_right; ++x)
	for(int y = y_bot; y <= y_top; ++y)
		if(cell[x*ny+y] == SOLID)
			size++;
	return size;
}

void Research_Porosity :: experiment()
{
	direction_of_wave_motion = East;
	create_exp_field();
	copy_sample_of_investigation();
	create_wave();
	iterations();
	delete_exp_field();
}

void Research_Porosity :: create_exp_field()
{
	int nx_exp, ny_exp;
	nx_exp = x_right - x_left; // For sample of investigation
	ny_exp = y_top - y_bot;
	nx_exp += 2; // For borders
	ny_exp += 2; 
	if(direction_of_wave_motion == East || 
	   direction_of_wave_motion == West)
		nx_exp += L;
	else 
		ny_exp += L;
	field_exp = new Field(nx_exp, ny_exp);
}

void Research_Porosity :: delete_exp_field()
{
	delete field_exp;
}

void Research_Porosity :: create_wave()
{
	field_exp -> clear_liquids();
	field_exp -> sound_map -> clear_map();
	int nx, ny;
	field_exp -> get_lattice_size(nx, ny);
	switch(direction_of_wave_motion)
	{
	case East:
		for(int y = 0; y < ny; ++y)
			field_exp -> rho_increase(1, y, 1.0);
		break;
	case South:
		for(int x = 0; x < nx; ++x)
			field_exp -> rho_increase(x, ny-2, 1.0);
		break;
	case West:
		for(int y = 0; y < ny; ++y)
			field_exp -> rho_increase(nx-2, y, 1.0);
		break;
	case North:
		for(int x = 0; x < nx; ++x)
			field_exp -> rho_increase(x, 1, 1.0);
		break;
	}
}

void Research_Porosity :: copy_sample_of_investigation()
{
	int x0, y0;
	int nx, ny;
	field -> get_lattice_size(nx, ny);
	x0 = y0 = 1;
	field_exp -> create_fence_solids(ABSORBER); 
	if(direction_of_wave_motion == East)
		x0 += L;
	if(direction_of_wave_motion == South)
		y0 += L;
	for(int x = x_left; x <= x_right; ++x)
	for(int y = y_bot; y <= y_top; ++y)
		field_exp -> set_obj(x0 + x - x_left,
				     y0 + y - y_bot, 
				     cell[x*ny+y]);
}

void Research_Porosity :: iterations()
{
	int n;
	int nx, ny;
	field_exp -> get_lattice_size(nx, ny);
	n = sqrt(nx*nx + ny*ny);
	for(int i = 0; i < n; ++i)
		field_exp -> next_step();
	field_exp -> sound_map -> 
		save_density_map_bmp("sound_map_propagation.bmp");
}
