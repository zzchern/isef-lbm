/*
  Contains bodies of functions of class "Wrapper".
 */
#include "wrapper.h"
#include "constants.h"
#include <stdio.h>
#include <SDL/SDL.h>

// RGBA masks
const Uint32 Rmask = 0xff000000;
const Uint32 Gmask = 0x00ff0000;
const Uint32 Bmask = 0x0000ff00;
const Uint32 Amask = 0x000000ff;
// Colors for saving field structure
#define liquid_color         0x00000000
#define solid_color                  Gmask | Bmask | Amask
#define isolated_solid_color                 Bmask | Amask
#define absorber_color       Rmask |         Bmask | Amask

Uint32 getpixel(SDL_Surface *surface, int x, int y);

void Wrapper :: save(char* path, Field* field)
{ 
	FILE *file;
	int nx, ny;
	int* cell;
	file = fopen(path, "w");
	cell = field -> get_lattice_object();
  	field -> get_lattice_size(nx, ny);
  	fprintf(file, "%d\n", nx);
	fprintf(file, "%d\n", ny);
	for(int x = 0; x < nx; ++x)
  	for(int y = 0; y < ny; ++y)
		fprintf(file, "%d\n", cell[x*ny + y]);
 	fclose(file);
}

void Wrapper :: load(char* path, Field* field)
{ 
	FILE *file;
	size_t result;
	int nx, ny;
	int* cell;
	file = fopen(path, "r");
	result = fscanf(file, "%d", &nx);
	result = fscanf(file, "%d", &ny);
	field -> resize(nx, ny);
	cell = field -> get_lattice_object();
	for(int x = 0; x < nx; ++x)
	for(int y = 0; y < ny; ++y)
		result = fscanf(file, "%d", &cell[x*ny + y]);
	fclose(file);
}

void Wrapper :: save_field_structure_bmp(char* bmp_path, Field* field)
{
	Uint32 *pixels;
	int* cell;
	int nx, ny;
	field -> get_lattice_size(nx, ny);
	cell = field -> get_lattice_object();
	pixels = new Uint32[nx*ny];
	for(int x = 0; x < nx; ++x)
	for(int y = 0; y < ny; ++y)
		switch(cell[x*ny + y])
		{
		case LIQUID:
			pixels[y*nx + x] = liquid_color;         
			break;
		case SOLID:
			pixels[y*nx + x] = solid_color;
			break;
		case ISOLATED_SOLID: 
			pixels[y*nx + x] = isolated_solid_color; 
			break;
		case ABSORBER:       
			pixels[y*nx + x] = absorber_color;       
			break;
		}
	SDL_Surface* image;
	SDL_Init( SDL_INIT_EVERYTHING );
	image = SDL_CreateRGBSurface(SDL_SWSURFACE, nx, ny, 32, 
				     Rmask, Gmask, Bmask, Amask);
	memcpy( (Uint32*)(image->pixels), pixels, nx*ny*sizeof(Uint32));
	SDL_SaveBMP(image, bmp_path);
	SDL_FreeSurface(image);
	SDL_Quit();
}

void Wrapper :: load_field_structure_bmp(char* bmp_path, Field* field)
{
	SDL_Surface* image;
	int nx, ny;
	int* cell;
	Uint32 color;
	SDL_Init( SDL_INIT_EVERYTHING );
	image = SDL_LoadBMP(bmp_path);
	nx = image -> w;
	ny = image -> h;
	field -> resize(nx, ny);
	cell = field -> get_lattice_object();
	for(int x = 0; x < nx; ++x)
	for(int y = 0; y < ny; ++y)
	{
		color = getpixel(image, x, y);
		if(!(color & Amask))
			cell[x*ny + y] = LIQUID;
		if(color & Gmask)
			cell[x*ny + y] = SOLID;
		if(color & Rmask) 
			cell[x*ny + y] = ABSORBER;
		if((color & Bmask) && !(color & Gmask) && !(color & Rmask))
			cell[x*ny + y] = ISOLATED_SOLID;
	}
	SDL_FreeSurface(image);
	SDL_Quit();
}

void Wrapper :: get_pixel_array_from_bmp(char* bmp_file, 
					 int& width, int& height, 
					 unsigned int* array)
{
	SDL_Surface* image;
	SDL_Init( SDL_INIT_EVERYTHING );
	image = SDL_LoadBMP(bmp_file);
	width = image -> w;
	height = image -> h;
	array = new unsigned int[width*height];
	for(int x = 0; x < width; ++x)
	for(int y = 0; y < height; ++y)
		array[x*height + y] = getpixel(image, x, y);
	SDL_FreeSurface(image);
	SDL_Quit();
}

Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
	int bpp = surface -> format -> BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + 
		y * surface->pitch + 
		x * bpp;
	switch(bpp)
	{
	case 1:
		return *p;
	case 2:
		return *(Uint16 *)p;
	case 3: 
		if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 16 | p[1] << 8 | p[2];
		else
			return p[0] | p[1] << 8 | p[2] << 16;
	case 4:
		return *(Uint32 *)p;
	}
	return 0;
}
