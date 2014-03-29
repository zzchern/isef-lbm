/*
  This idea is based on creationd "sound maps".
  Sound Map [SM] - type of displaing of information, which shows, what points of fieled
  had the most pressure changing. It is shown by gradation.
  Also there is function of saving SM into "*.bmp".
*/
#include <SDL/SDL.h>
#include <math.h>
#include "constants.h"
#include "sound_map.h"
#define NORMAL_POINT 0
#define SILENT_POINT 1 // Silent points on objects map
#define LOUD_POINT 2   // Loud points
const Uint32 Rmask = 0xff000000; // RGBA маски
const Uint32 Gmask = 0x00ff0000;
const Uint32 Bmask = 0x0000ff00;
const Uint32 Amask = 0x000000ff;
	

SoundMap :: SoundMap(int size_nx, int size_ny, float* ptr_rho, float koef)
{
	nx = size_nx;
	ny = size_ny;
	rho = ptr_rho;
	k = koef;
	density_map = new float[nx*ny];
	max_val_map = new float[nx*ny];
	obj = new short[nx*ny];
	iteration = 0;
	clear_map();
	work(false);
}

SoundMap :: ~SoundMap()
{
	delete density_map;
	delete max_val_map;
	delete obj;
}

void SoundMap :: refresh_map()
{
	float maxRho;
	float minRho;
	float deltaRho;
	float val;
	if(state) 
	{
		// density map
		iteration++;
		minRho = start_rho * (1.0);	
		maxRho = start_rho * (1.0 + k);
		deltaRho = maxRho - minRho;
		for(int x = 0; x < nx; ++x)
		for(int y = 0; y < ny; ++y)
		{
			val = rho[x*ny+y];
			val = (val > maxRho ? maxRho : val);
			val = (val < minRho ? minRho : val);
			val = (val - minRho) / deltaRho;
			val = val*val;
			density_map[x*ny+y] = ((density_map[x*ny+y] * (iteration-1)) 
					       + val)	/ iteration;
		}
		// max_val map
		for(int x = 0; x < nx; ++x)
		for(int y = 0; y < ny; ++y)
			if((rho[x*ny+y]-start_rho) > max_val_map[x*ny+y])
				max_val_map[x*ny+y] = rho[x*ny+y] - start_rho;
	}
}

float* SoundMap :: get_density_map()
{
	return density_map;
}

float* SoundMap :: get_max_val_map()
{
	return max_val_map;
}

inline
Uint32 value_to_RGBA(float value, short type)
{
	Uint32 color;
	value = pow(value, 1.0/3.0);
	color = color = ( (Uint32(value * 255.0)<<24)
			  | (Uint32(value * 255.0)<<16) 
			  | (Uint32(value * 255.0)<<8)
			  | (0x000000ff));
	return color;
}

inline
float abs(float a)
{
	if(a >= 0)
		return a;
	else
		return (-1.0)*a;
}

void SoundMap :: save_density_map_bmp(const char* path)
{
	Uint32 *pixels;
 	SDL_Surface* image;
	pixels = new Uint32[nx*ny];
	SDL_Init( SDL_INIT_EVERYTHING );
	find_max_min_vals();
	find_arithmetic_mean();
	find_loud_areas();
	find_silent_areas();
	find_average_deviation_from_mean();
	for(int x = 0; x < nx; ++x)
	for(int y = 0; y < ny; ++y)
	{
		pixels[y*nx + x] = value_to_RGBA(sqrt(density_map[x*ny + y]), 
						 obj[x*ny + y]);
	}
	image = SDL_CreateRGBSurfaceFrom(pixels, nx, ny, 32, nx*4, 
					 Rmask, Gmask, Bmask, Amask);
	SDL_SaveBMP(image, path);
	SDL_FreeSurface(image);
	SDL_Quit();
	delete pixels;
}

void SoundMap :: save_max_val_map_bmp(const char* path)
{
	Uint32 *pixels;
 	SDL_Surface* image;
	pixels = new Uint32[nx*ny];
	SDL_Init( SDL_INIT_EVERYTHING );
	for(int x = 0; x < nx; ++x)
	for(int y = 0; y < ny; ++y)
	{
		pixels[y*nx + x] = value_to_RGBA(sqrt(max_val_map[x*ny + y]), 
						 obj[x*ny + y]);
	}
	image = SDL_CreateRGBSurfaceFrom(pixels, nx, ny, 32, nx*4, 
					 Rmask, Gmask, Bmask, Amask);
	SDL_SaveBMP(image, path);
	SDL_FreeSurface(image);
	SDL_Quit();
	delete pixels;
}

void SoundMap :: clear_map()
{
	for(int x = 0; x < nx; ++x)
	for(int y = 0; y < ny; ++y)
	{
		density_map[x*ny + y] = 0.0;
		max_val_map[x*ny + y] = 0.0;
		obj[x*ny + y] = NORMAL_POINT;
	}
	iteration = 0;
}

void SoundMap :: work(bool new_state)
{
	state = new_state;
}

void SoundMap :: find_silent_areas()
{
	for(int x = 0; x < nx; ++x)
	for(int y = 0; y < ny; ++y)
		if(density_map[x*ny + y] <= max_value/3.0)
			obj[x*ny + y] = SILENT_POINT;
}

void SoundMap :: find_loud_areas()
{
	for(int x = 0; x < nx; ++x)
	for(int y = 0; y < ny; ++y)
		if(density_map[x*ny + y] >= max_value*2.0/3.0)
			obj[x*ny + y] = LOUD_POINT;
}

void SoundMap :: find_max_min_vals()
{
	max_value = -1.0;
	min_value = 10.0;
	for(int x = 0; x < nx; ++x)
	for(int y = 0; y < ny; ++y)
	{
		if(density_map[x*ny + y] > max_value)
			max_value = density_map[x*ny + y];
		if(density_map[x*ny + y] < min_value)
			min_value = density_map[x*ny + y];
	}
}

float SoundMap :: find_arithmetic_mean()
{
	float sum = 0;
	for(int x = 0; x < nx; ++x)
	for(int y = 0; y < ny; ++y)
		sum += density_map[x*ny + y];
	mean_value = sum / (nx*ny);
	return mean_value;
}

float SoundMap :: find_average_deviation_from_mean()
{
	float current_deviation;
	float deviation_sum = 0;
	for(int x = 0; x < nx; ++x)
	for(int y = 0; y < ny; ++y)
	{
		current_deviation = abs(density_map[x*ny + y] - mean_value);
		deviation_sum += current_deviation;
	}
	deviation_value = deviation_sum / (nx*ny);
	return deviation_value;
}

void SoundMap :: save_bmp_from_pixels(unsigned int *pixels, char* path)
{
	SDL_Surface* image;
	SDL_Init( SDL_INIT_EVERYTHING );
	image = SDL_CreateRGBSurfaceFrom(pixels, nx, ny, 32, nx*4, 
					 Rmask, Gmask, Bmask, Amask);
	SDL_SaveBMP(image, path);
	SDL_FreeSurface(image);
	SDL_Quit();
	delete pixels;
}
