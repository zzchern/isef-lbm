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
#include "spectral_analysis.h"
#define sqr(a) a*a

Spectrogram :: Spectrogram()
{
	is_allocated = false;
}

void Spectrogram :: load_array(const char* filename)
{
	FILE* file;
	file = fopen(filename, "r");
	fscanf(file, "%d", &size);
	if(is_allocated)
	{
		delete [] amp_array;
		delete [] frq_array;
	}
	amp_array = new float[size];
	frq_array = new float[size];
	is_allocated = true;
	for(int i = 0; i < size; ++i)
		fscanf(file, "%f", &amp_array[i]);
	fclose(file);
}

void Spectrogram :: save_results()
{
	FILE* file;
	file = fopen("spectrorgam.txt", "w");
	for(int i = 0; i < size; ++i)
		fprintf(file, "f[%3d] = %0.4f\n", i, frq_array[i]);
	fclose(file);
}

void Spectrogram :: analyze()
{
	FFT(amp_array, frq_array, size);
}

float* Spectrogram :: get_freq_array()
{
	return frq_array;
}


void Spectrogram :: FFT(float* A, float* F, int N) 
{
	int Nvl, Nft;
	int i, j, n, m, Mmax, Istp;
	double Tmpr, Tmpi, Wtmp, Theta;
	double Wpr, Wpi, Wr, Wi;
	double *Tmvl;
	Nvl = Nft = N;
	n = Nvl * 2; 
	Tmvl = new double[n];
	for (i = 0; i < Nvl; i++) 
	{
		j = i * 2; 
		Tmvl[j] = 0; 
		Tmvl[j+1] = A[i];
	}
	i = 1; 
	j = 1;
	while (i < n) 
	{
		if (j > i) 
		{
			Tmpr = Tmvl[i]; 
			Tmvl[i] = Tmvl[j]; 
			Tmvl[j] = Tmpr;
			Tmpr = Tmvl[i+1]; 
			Tmvl[i+1] = Tmvl[j+1]; 
			Tmvl[j+1] = Tmpr;
		}
		i = i + 2; 
		m = Nvl;
		while ((m >= 2) && (j > m)) 
		{
			j = j - m; 
			m = m >> 1;
		}
		j = j + m;
	}
	Mmax = 2;
	while (n > Mmax) 
	{
		Theta = -2 * M_PI / Mmax; 
		Wpi = sin(Theta);
		Wtmp = sin(Theta / 2); 
		Wpr = Wtmp * Wtmp * 2;
		Istp = Mmax * 2; 
		Wr = 1; 
		Wi = 0; 
		m = 1;
		while (m < Mmax) 
		{
			i = m; 
			m = m + 2; 
			Tmpr = Wr; 
			Tmpi = Wi;
			Wr = Wr - Tmpr * Wpr - Tmpi * Wpi;
			Wi = Wi + Tmpr * Wpi - Tmpi * Wpr;
			while (i < n) 
			{
				j = i + Mmax;
				Tmpr = Wr * Tmvl[j] - Wi * Tmvl[j-1];
				Tmpi = Wi * Tmvl[j] + Wr * Tmvl[j-1];
				Tmvl[j] = Tmvl[i] - Tmpr; 
				Tmvl[j-1] = Tmvl[i-1] - Tmpi;
				Tmvl[i] = Tmvl[i] + Tmpr; 
				Tmvl[i-1] = Tmvl[i-1] + Tmpi;
				i = i + Istp;
			}
		}
		Mmax = Istp;
	}
	for (i = 0; i < Nft; i++) 
	{
		j = i * 2; 
		F[Nft - i - 1] = sqrt(sqr(Tmvl[j]) + sqr(Tmvl[j+1]));
	}
	delete []Tmvl;
}
