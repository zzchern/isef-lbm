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
Proportional conversion:
	Основная идея заключается в том, чтобы 1 секунда моделируемого реального
	времени моделировалась за N итераций программы. Где N - максимальная 
	частота дискретизации для данного поля. (частота дискретизации файла, 
	который будет моделироваться)
	1 [секунда реального времени] = N [итераций]
	1 [секунда реального времени] * sound_speed [метров/секунда] = 
					N [итераций] * 1/sqrt(3) [lattice units / итерация]  
	dx / sound_speed
	1 * sound_speed [м] = N / sqrt(3) [клеток] 
*/
#include <math.h>
#include <stdio.h>
#include "unit_conversion.h"

UnitConverter :: UnitConverter()
{
	// Initialization of constants.
	// Visosity for material. [Pa*s]
	Viscosity[Air] = 1.53e-5;
	Viscosity[O2] =  1.40e-5;
	Viscosity[CO2] = 7.32e-6;
	Viscosity[H2] =  9.78e-5;
	Viscosity[H2O] = 1.00e-6;
	Viscosity[Oil] = 1.01e-3;
	// Speed of sound in materials. [m/s]
	SoundSpeedPhys[Air] = 343; 
	SoundSpeedPhys[O2] = 317;
	SoundSpeedPhys[CO2] = 258;
	SoundSpeedPhys[H2] = 1270;
	SoundSpeedPhys[H2O] = 1481;
	SoundSpeedPhys[Oil] = 1540;
	lSoundSpeed = 1 / sqrt(3);		
	// default 
	material = Air;
	sample_rate = 8000;
	Tau = 1.0;
}	

void UnitConverter :: set_material(int m)
{
	m = (m < 0 ? 0 : m);
	m = (m > 5 ? 5 : m);
	material = m;
}

void UnitConverter :: set_tau(float _Tau)
{
	Tau = _Tau;
	Tau = (Tau <= 0.5 ? 1 : Tau);
	Tau = (Tau > 5 ? 1 : Tau);
}
	
float UnitConverter :: get_dx(int type)
{
	float dx;
	switch(type)
	{
	case DirectConversion:
		dx = Viscosity[material] / 
			(lSoundSpeed * 
			 SoundSpeedPhys[material] * 
			 (Tau - 0.5));
		break;
	case ProportionalConversion:
		dx = sqrt(3) * SoundSpeedPhys[material] / sample_rate; 
		break;
	}
	return dx;
}

float UnitConverter :: get_dt(int type)
{
	float dt;
	switch(type)
	{
	case DirectConversion:
		dt = Viscosity[material] / 
			(SoundSpeedPhys[material] * 
			 SoundSpeedPhys[material] * 
			 (Tau - 0.5));
		break;
	case ProportionalConversion:
		dt = sqrt(3) / sample_rate;
		break;
	}
	return dt;
}

void UnitConverter :: set_max_sample_rate(int _sample_rate)
{
	sample_rate = _sample_rate;
	sample_rate = (sample_rate < 10 ? 10 : sample_rate);
	sample_rate = (sample_rate > 100000 ? 100000 : sample_rate);	
}

int UnitConverter :: get_max_sample_rate()
{
	return sample_rate;
}

void UnitConverter :: display(int nx, int ny)
{
	float dx, dt;
	printf("----------------Coversion----------------\n");
	dx = get_dx(DirectConversion);
	dt = get_dt(DirectConversion);
	printf("Type      : %s\n", "DirectConversion"); 	
	printf("Field size: %.7f * %.7f [m*m]\n", dx*nx, dx*ny);
	printf("Delta time: %.10f [sec]\n", dt);
	dx = get_dx(ProportionalConversion);
	dt = get_dt(ProportionalConversion);
	printf("Type      : %s\n", "ProportionalConversion"); 	
	printf("Field size: %.7f * %.7f [m*m]\n", dx*nx, dx*ny);
	printf("Delta time: %.10f [sec]\n", dt);	
	printf("-----------------------------------------\n");
}


