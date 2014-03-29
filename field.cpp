/*
  File contains bodies of functions of class "Field".
 */
#include "field.h"
#include "constants.h"
#include "stdio.h"
#include "unit_conversion.h"res
#include "wav_codec.h"
#include <math.h>

Field :: Field(int nx, int ny)
{
	Nx = nx;
	Ny = ny;
	initialization();
	prelude();
}

Field :: ~Field()
{	
	clear_memory();
}

void Field :: resize(int new_nx, int new_ny)
{
	clear_memory();  
	emitters.clear();
	receivers.clear();
	Nx = new_nx;
	Ny = new_ny;
	initialization();
}

void Field :: allocate_memory()
{
	f = new float[Nx*Ny*Nc]; 
	fTemp = new float[Nx*Ny*Nc];
	fEq = new float[Nx*Ny*Nc]; 
	uX = new float[Nx*Ny]; 
	uY = new float[Nx*Ny];
	rho = new float[Nx*Ny];
	cell = new int[Nx*Ny]; 
	u_sqr = new float[Nx*Ny];
	sound_map = new SoundMap(Nx, Ny, rho, 0.0006);
}

void Field :: clear_memory()
{
	delete f;
	delete fTemp;
	delete fEq;
	delete uX;
	delete uY;
	delete u_sqr;
	delete rho;
	delete cell;
	delete sound_map;
}

void Field :: initialization()
{
	allocate_memory();
	initialization_parameters();
	initialization_speed_rho();
	compute_f_equilibrium();
	initialize_f();
	create_fence_solids(ABSORBER);
	set_tau(1.0);
	set_rad(2);
	set_step(1);
	iteration = 0;
	set_figure(CIRCLE);
}

void Field :: next_step()
{	
	for(int i = 0; i < compute_n_steps; ++i)
		switch(computing)
		{
		case CPU:
			next_step_CPU();
			break;
		case CPU_PARALLEL: 
			next_step_CPU_parallel();
			break;
		}
	for(unsigned int i = 0; i < emitters.size(); ++i)
		emitters[i].next_step();
	for(unsigned int i = 0; i < receivers.size(); ++i)
		receivers[i].next_receive();
	iteration++;
	sound_map -> refresh_map();
}

void Field :: next_step_CPU()
{
	solids();
	absorbers();
	streaming();
	compute_speed_rho(); 
	compute_f_equilibrium();
	collide();
}

void Field :: next_step_CPU_parallel()
{
	parallel();
}

void Field :: pressure_increase(Point* points, int n, float k)
{
	for(int i = 0; i < n; ++i)
		rho_increase(points[i].x, points[i].y, k);
}

void Field :: rho_increase(int x, int y, float k)
{
	int index = x*Ny + y;
	k = (k > 1.0 ? 1.0 : k);
	k = (k < 0.0 ? 0.0 : k);
	if(x < 0 || x >= Nx || 
	   y < 0 || y >= Ny)
		return;
	for(int i = 0; i < Nc; ++i)
		f[index * Nc + i] += (k * rho_inc) / Nc;
}

void Field :: clear_liquids()
{
	for(int x = 0; x < Nx; ++x)
	for(int y = 0; y < Ny; ++y)
	for(int i = 0; i < Nc; ++i)
		f[(x*Ny + y)*Nc + i] = w[i] * start_rho;
	sound_map -> clear_map();
}

void Field :: isolate_solids()
{
	for(int x = 1; x < Nx-1; ++x)
	for(int y = 1; y < Ny-1; ++y)
		if( cell[x*Ny + (y+1)] != LIQUID &&
		    cell[x*Ny + (y-1)] != LIQUID &&
		    cell[(x+1)*Ny + y] != LIQUID &&
		    cell[(x-1)*Ny + y] != LIQUID &&
		    cell[(x+1)*Ny + (y+1)] != LIQUID &&
		    cell[(x-1)*Ny + (y-1)] != LIQUID &&
		    cell[(x+1)*Ny + (y-1)] != LIQUID &&
		    cell[(x-1)*Ny + (y+1)] != LIQUID )
			cell[x*Ny + y] = ISOLATED_SOLID;
		else
			if(cell[x*Ny + y] == ISOLATED_SOLID)
				cell[x*Ny + y] = SOLID;
}	

void Field :: set_lattice_object(Point* points, int n, int type)
{
	for(int i = 0; i < n; ++i)
		set_obj((points + i)->x, (points + i)->y, type);
}

void Field :: set_obj (int x, int y, int type)
{
	int x_left, x_right, y_top, y_bot;
	x_left = (x-rad < 0) ? (0) : (x-rad);
	x_right = (x+rad > Nx-1) ? (Nx-1) : (x+rad);
	y_top = (y+rad > Ny-1) ? (Ny-1) : (y+rad);
	y_bot = (y-rad < 0) ? (0) : (y-rad);
	switch(figure)
	{
	case SQUARE:
		for(int ix = x_left; ix <= x_right; ++ix)
		for(int iy = y_bot; iy <= y_top; ++iy)
			cell[ix*Ny + iy] = type;
		break;
	case CIRCLE:
		for(int ix = x_left; ix <= x_right; ++ix)
		for(int iy = y_bot; iy <= y_top; ++iy)
			if( (ix - x)*(ix - x) + 
			    (iy - y)*(iy - y) <= rad*rad )
				cell[ix*Ny + iy] = type;
		break;
	}
	isolate_solids();
}

void Field :: fill_area(int x, int y, int type)
{
	if( x < 0 || x > Nx-1 || y < 0 || y > Ny-1)
		return;
	if( cell[x*Ny + y] != type)
	{
		cell[x*Ny + y] = type;
		fill_area(x-1, y, type);
		fill_area(x+1, y, type);
		fill_area(x, y-1, type);
		fill_area(x, y+1, type);
	}
	else 
		return;
}

float* Field :: get_lattice_rho()
{
	return rho;
}

float* Field :: get_lattice_u_x()
{
	return uX;
}

float* Field :: get_lattice_u_y()
{
	return uY;
}

int* Field :: get_lattice_object()
{
	return cell;
}

float* Field :: get_lattice_u_sqr()
{
	return u_sqr;
}

float* Field :: get_lattice_f()
{
	return f;
}

void Field :: get_lattice_size(int& nx, int& ny)
{
	nx = Nx;
	ny = Ny;
}

void Field :: set_rad(int value)
{
	rad = value;
}

void Field :: set_figure(int fig)
{
	figure = fig;
}

void Field :: set_step(int value)
{
	value = (value < 1 ? 1 : value);
	value = (value > 15 ? 15 : value);
	compute_n_steps = value;
}

void Field :: set_tau(float value)
{
	tau = (tau < 0.5 ? 0.5 : tau);
	tau = (tau > 2.0 ? 2.0 : tau);
	tau = value;
}

void Field :: set_computing_type(int type)
{
	if(type != CPU && type != GPU && type != CPU_PARALLEL)
		type = CPU;
	computing = type;
}

void Field :: create_fence_solids(int type)
{
	if(type != SOLID && type != ABSORBER)
		type = SOLID;
	for(int x = 0; x < Nx; ++x)
	for(int y = 0; y < Ny; ++y)
		if((x==0) || (x==Nx-1) || (y==0) || (y==Ny-1))
			cell[x*Ny + y] = type;
		else 
			cell[x*Ny + y] = LIQUID;
}

void Field :: clear_solids()
{
	for(int x = 0; x < Nx; ++x)
	for(int y = 0; y < Ny; ++y)
		cell[x*Ny + y] = LIQUID;
}

void Field :: save_sound_map(char* path)
{
	sound_map -> save_density_map_bmp(path);
	printf("Sound map created.\n");
	sound_map -> clear_map();
}

float* Field :: get_sound_map()
{
	return sound_map -> get_density_map();
}

void Field :: prelude()
{
	sound_map -> work(true);

	//UnitConverter UC;
	//UC.set_material(Air);
	//UC.set_tau(tau);
	//UC.set_max_samle_rate(44100);
	//UC.display(Nx, Ny);

	//Receiver r(Nx, Ny, f);
	//receivers.push_back(r);
}
/*
if(iteration == 100)
	{
		Codec C;
		float* values;
		unsigned int samples;
		samples = C.num_of_samples("voice_mono.wav");
		values = new float[samples];		
		C.wav_to_array(values, "voice_mono.wav");
		set_step(1);
		set_computing_type(CPU_PARALLEL);
		Emitter e(f, Nx, Ny);
		e.set_point_position(10, 10);
		e.create_special_wave(values, samples);
		delete values;
		emitters.push_back(e);
		Receiver r(f, Nx, Ny);
		r.set_position(Nx/2, Ny/2);
		receivers.push_back(r);
	}/*
	if(!(iteration%1000))
	{
		printf("\r Iteration = %6d;\n", int(iteration));
		//receivers[0].set_position(iteration/1000 + 20, 
		//			  iteration/1000 + 20);
	}
	if(iteration == 40000)
		receivers[0].set_position(Nx-10, Ny-10);
	if(iteration == 41000*2)
	{
		receivers[0].save_wav("test.wav", 41000);
		receivers[0].write_to_file("test.txt");
		printf("Record\n");
	}*/

