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

void Field :: initialization_parameters()
{
	float w0 = 16 / 36.0;
	float w1 = 4 / 36.0;
	float w2 = 1 / 36.0;
	w[0] = w0;
	w[1] = w[2] = w[3] = w[4] = w1;
	w[5] = w[6] = w[7] = w[8] = w2;
	c[0][0] = 0;		c[0][1] = 0;
	c[1][0] = 1;		c[1][1] = 0;
	c[2][0] = 0;		c[2][1] = 1;
	c[3][0] = -1;		c[3][1] = 0;
	c[4][0] = 0;		c[4][1] = -1;
	c[5][0] = 1;		c[5][1] = 1;
	c[6][0] = -1;		c[6][1] = 1;	
	c[7][0] = -1;		c[7][1] = -1;
	c[8][0] = 1;		c[8][1] = -1;
}

void Field :: initialization_speed_rho()
{
	for(int x = 0; x < Nx; ++x)
	for(int y = 0; y < Ny; ++y)
	{
		uX[x*Ny + y] = 0.0f;
		uY[x*Ny + y] = 0.0f;
		rho[x*Ny + y] = start_rho;
	}
}

void Field :: compute_u_sqr()
{
	for(int x = 0; x < Nx; ++x)
	for(int y = 0; y < Ny; ++y)
		u_sqr[x*Ny + y] =
			uX[x*Ny + y] * uX[x*Ny + y] 
			+ uY[x*Ny + y] * uY[x*Ny + y];
}

void Field :: compute_f_equilibrium()
{
	float uci;
	int x, y, i, index;
	compute_u_sqr();
	for(x = 0; x < Nx; ++x)
	for(y = 0; y < Ny; ++y)
	{
		index = x*Ny + y;
		u_sqr[index] /= 2 * csSqd;
		for(i = 0; i < Nc; ++i)
		{
			uci = uX[index] * c[i][0] + uY[index] * c[i][1];
			uci /= csSqd;
			fEq[index*Nc + i] = rho[index] * w[i] * 
				(1 + uci * (1 + uci / 2) - u_sqr[index]); 
		}
	}
}

void Field :: initialize_f()
{
	for(int x = 0; x < Nx; ++x)
	for(int y = 0; y < Ny; ++y)
	for(int i = 0; i < Nc; ++i)
		f[x*Ny*Nc + y*Nc + i] = fEq[x*Ny*Nc + y*Nc + i];
}

void Field :: streaming()
{
	float *swapBuffer;
	int x_left, x_right;
	int y_top, y_bot;
	for(int x = 0; x < Nx; ++x)
	{
		x_left  = (x-1 < 0)    ? (Nx-1) : (x-1);	
		x_right = (x+1 > Nx-1) ? (0)    : (x+1);
		for(int y = 0; y < Ny; ++y)
		if(cell[x*Ny + y] != ISOLATED_SOLID)
		{
			y_bot   = (y-1 < 0)    ? (Ny-1) : (y-1);
			y_top   = (y+1 > Ny-1) ? (0)    : (y+1);
			fTemp[x*Ny*Nc + y*Nc + 0] = 
				f[x * Ny*Nc + y * Nc + 0];
			fTemp[x*Ny*Nc + y*Nc + North]     = 
				f[x * Ny*Nc + y_top * Nc + North];
			fTemp[x*Ny*Nc + y*Nc + South]     = 
				f[x * Ny*Nc + y_bot * Nc + South];
			fTemp[x*Ny*Nc + y*Nc + East]      = 
				f[x_left * Ny*Nc + y * Nc + East];
			fTemp[x*Ny*Nc + y*Nc + West]      = 
				f[x_right * Ny*Nc + y * Nc + West];
			fTemp[x*Ny*Nc + y*Nc + NorthEast] = 
				f[x_left  * Ny*Nc + y_top * Nc + NorthEast];
			fTemp[x*Ny*Nc + y*Nc + NorthWest] = 
				f[x_right * Ny*Nc + y_top * Nc + NorthWest];
			fTemp[x*Ny*Nc + y*Nc + SouthEast] = 
				f[x_left  * Ny*Nc + y_bot * Nc + SouthEast];
			fTemp[x*Ny*Nc + y*Nc + SouthWest] = 
				f[x_right * Ny*Nc + y_bot * Nc + SouthWest];
		}
	}
	swapBuffer = f;
	f = fTemp;
	fTemp = swapBuffer;		
}

void Field :: solids()
{
	simple_BC();
}


void Field :: simple_BC()
{
	float buffer;
	int index;
	for(int x = 0; x < Nx; ++x)
	for(int y = 0; y < Ny; ++y)
	{
		index = x*Ny + y;
		if(cell[index] == SOLID)
		{
		 	buffer = f[index*Nc + North];	
			f[index*Nc + North] = f[index*Nc + South];	        
			f[index*Nc + South] = buffer;
			buffer = f[index*Nc + East];	
			f[index*Nc + East] = f[index*Nc + West];		
			f[index*Nc + West] = buffer;
			buffer = f[index*Nc + NorthEast];	
			f[index*Nc + NorthEast] = f[index*Nc + SouthWest];	
			f[index*Nc + SouthWest] = buffer;
			buffer = f[index*Nc + NorthWest];	
			f[index*Nc + NorthWest] = f[index*Nc + SouthEast];	
			f[index*Nc + SouthEast] = buffer;
		}
	}
}

void Field :: absorbers()
{
	int x, y, i;
	for(x = 0; x < Nx; ++x)
	for(y = 0; y < Ny; ++y)
	if(cell[x*Ny + y] == ABSORBER || 
	   cell[x*Ny + y] == ISOLATED_SOLID)
		for(i = 0; i < Nc; ++i)
			f[(x*Ny + y)*Nc + i] = w[i] * start_rho;
}

void Field :: compute_speed_rho()
{
	int index;
	int x, y, i;
	for(x = 0; x < Nx; ++x)
	for(y = 0; y < Ny; ++y)
	{
		index = x*Ny + y;
		rho[index] = uX[index] = uY[index] = 0;
		for(i = 0; i < Nc; ++i)
		{
			uX[index] += f[index*Nc + i] * c[i][0];
			uY[index] += f[index*Nc + i] * c[i][1];
			rho[index] += f[index*Nc + i];
		}
		uX[index] /= rho[index];
		uY[index] /= rho[index];
	}
}

void Field :: collide()
{
	int index;
	for(int x = 0; x < Nx; ++x)
	for(int y = 0; y < Ny; ++y)
	for(int i = 0; i < Nc; ++i)
	{
		index = x*Ny*Nc + y*Nc + i;
		f[index] -= (f[index] - fEq[index]) / tau;
	}
}
