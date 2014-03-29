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

// p = rho * cs^2 / 3;
#include <math.h>
#include <stdio.h>
#include "constants.h"
#include "emitter.h"
#define sqr(a) (a)*(a)

Emitter :: Emitter(float* field_f, int nx, int ny) : 
	f(field_f), field_nx(nx), field_ny(ny) 
{
	step = 0;
	set_default_params();
}

void Emitter :: next_step()
{
	movement();
	emission();	
}

void Emitter :: set_point_position(int x, int y)
{
	Point p(x, y);
	set_point_position(p);
}

void Emitter :: set_point_position(Point p)
{
	position.clear();
	p.x = (p.x < 0 ? 0 : p.x);
	p.x = (p.x >= field_nx ? field_nx-1 : p.x);
	p.y = (p.y < 0 ? 0 : p.y);
	p.y = (p.y >= field_ny ? field_ny-1 : p.y);
	state = true;
	position.push_back(p);
}

void Emitter :: set_line_position(int x1, int y1, int x2, int y2)
{
	Point p1(x1, y1);
	Point p2(x2, y2);
	set_line_position(p1, p2);
}

void Emitter :: set_line_position(Point p1, Point p2)
{
	position.clear();
	int L;
	int x, y;
	L = int( sqrt( sqr(p1.x - p2.x) + sqr(p1.y - p2.y) ) );
	for(int i = 0; i < L; ++i)
	{
		x = p1.x + (p2.x - p1.x) / L * i;
		y = p1.y + (p2.y - p1.y) / L * i;
		x = (x < 0 ? 0 : x);
		x = (x >= field_nx ? field_nx-1 : x);
		y = (y < 0 ? 0 : y);
		y = (y >= field_ny ? field_ny-1 : y);
		Point p(x, y);
		position.push_back(p);
	}
}
	
void Emitter :: set_period(unsigned long value)
{
	value = (value < 5 ? 5 : value);
	T = value;
	freq = 1/float(T);
}

void Emitter :: create_standart_wave(int type)
{
	wave = new float[T];
	for(int i = 0; i < T; ++i)
	{
		switch(type)
		{
		case SIN_EMIT:
			wave[i] = (sin(2.0 * M_PI * freq * i) + 1.0) / 2.0;
			break;
		case SQR_EMIT:
			if(i < T/2)
				wave[i] = 1.0;
			else
				wave[i] = 0.0;
			break;
		case SAW_EMIT:
			wave[i] = 2.0 * float(i) / float(T);
			break;
		case TRI_EMIT:
			if(i < T/2)
				wave[i] = 2.0 / float(T/2) * float(i);
			else
				wave[i] = 2.0 - 2.0 / float(T/2) * float(i);
			break;
		default:
			wave[i] = 0.0;
			break;
		}
	}
}

void Emitter :: create_special_wave(float* data, unsigned long size)
{
	set_period(size);
	delete wave;
	wave = new float[T];
	for(int i = 0; i < T; ++i)
		wave[i] = float(data[i]);
	float max_value = -1;
	for(int i = 0; i < T; ++i)
		if(wave[i] > max_value)
			max_value = wave[i];
	for(int i = 0; i < T; ++i)
		wave[i] /= max_value;
}

void Emitter :: load_wave(char* file_name)
{
	FILE *file;
	file = fopen(file_name, "r");
	if(file == NULL)
	{
		printf("File not found.\n");
		return;
	}
	fscanf(file, "%d", &T);
	delete wave;
	wave = new float[T];
	for(int i = 0; i < T; ++i)
		fscanf(file, "%f", &wave[i]);
	fclose(file);
}

void Emitter :: save_wave(char* file_name)
{
	FILE *file;
	file = fopen(file_name, "w");
	fprintf(file, "%d\n", T);
	for(int i = 0; i < T; ++i)
		fprintf(file, "%f\n", wave[i]);
	fclose(file);
	
}

void Emitter :: set_frequency(float value)
{
	set_period(int(1.0/value));
}

void Emitter :: set_speed(float new_vx, float new_vy)
{
	vx = new_vx;
	vy = new_vy;
	vx = (vx < -0.5 ? -0.5 : vx);
	vx = (vx > 0.5 ? 0.5 : vx);
	vy = (vy < -0.5 ? -0.5 : vy);
	vy = (vy > 0.5 ? 0.5 : vy);
}

void Emitter :: switch_emitter(bool new_state)
{
	state = new_state;
}

void Emitter :: movement()
{
	if(state)
	{
		for(int i = 0; i < position.size(); ++i)
		{
			position[i].x += vx;
			position[i].y += vy;
		}
	}
	for(int i = 0; i < position.size(); ++i)
		if( position[i].x < 0 || position[i].x >= field_nx || 
		    position[i].y < 0 || position[i].y >= field_ny)
		{
			position[i].x = field_nx / 2;
			position[i].y = field_ny / 2;
		}
}

void Emitter :: emission()
{
	if(state)
	{
		step++;
		step %= T;
		rho_increase(wave[step]);
	}
}

void Emitter :: rho_increase(float k)
{
	int index;
	k = (k > 1.0 ? 1.0 : k);
	k = (k < 0.0 ? 0.0 : k);
	for(int j = 0; j < position.size(); ++j)
	{
		index = position[j].x * field_ny + position[j].y;
		for(int i = 0; i < Nc; ++i)
			f[index * Nc + i] += k * rho_inc / Nc;
	}
}

void Emitter :: set_default_params()
{
	set_point_position(field_nx/2, field_ny/2);
	set_speed(0.0, 0.0);
	set_period(256/16);
	create_standart_wave(SIN_EMIT);
	switch_emitter(true);
}
