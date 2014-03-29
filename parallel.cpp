#include "field.h"
#include "constants.h"
#include <omp.h>

void Field :: parallel()
{
	omp_set_num_threads(NUM_TREADS);
	float buffer;
	int index;
	int i;
	int x_left, x_right, y_top, y_bot;
	int x, y;
	float* swapBuffer;
	float uci;
	#pragma omp parallel for \
	private(buffer)
	for(index = 0; index < Nx*Ny; ++index)
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
	#pragma omp parallel for \
	private(i)
	for(index = 0; index < Nx*Ny; ++index)
		if(cell[index] == ABSORBER || 
		   cell[index] == ISOLATED_SOLID)
			for(i = 0; i < Nc; ++i)
				f[(index)*Nc + i] = w[i] * start_rho;
	#pragma omp parallel for \
	private(x_left, x_right, y_top, y_bot, y)
	for(x = 0; x < Nx; ++x)
	{
		x_left  = (x-1 < 0)    ? (Nx-1) : (x-1);	
		x_right = (x+1 > Nx-1) ? (0)    : (x+1);
		for(y = 0; y < Ny; ++y)
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
	#pragma omp parallel for \
	private(i)
	for(index = 0; index < Nx*Ny; ++index)
	{
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
	#pragma omp parallel for \
	private(uci, i)
	for(index = 0; index < Nx*Ny; ++index)
	{
		u_sqr[index] = uX[index] * uX[index] + uY[index] * uY[index];
		u_sqr[index] /= 2 * csSqd;
		for(i = 0; i < Nc; ++i)
		{
			uci = uX[index] * c[i][0] + uY[index] * c[i][1];
			uci /= csSqd;
			fEq[index*Nc + i] = rho[index] * w[i] * 
				(1 + uci * (1 + uci / 2) - u_sqr[index]); 
		}
	}
	#pragma omp parallel for
	for(index = 0; index < Nx*Ny*Nc; ++index)
		f[index] -= (f[index] - fEq[index]) / tau;
}

