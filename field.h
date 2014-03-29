// giglavy@gmail.com
// giglavy@ya.ru
#pragma once
#include "sound_map.h"
#include "emitter.h"
#include "receiver.h"
#include <vector>
using namespace std;

class Field
{
public:
	Field(int nx, int ny);
	~Field();
	void resize(int new_nx, int new_ny);
	void next_step();
	void pressure_increase(Point* points, int n, float k); 
	void rho_increase(int x, int y, float k);	
	void set_lattice_object(Point* points, int n, int type);
	void set_obj(int x, int y, int type);
	void fill_area(int x, int y, int type);
	float* get_lattice_rho();
	float* get_lattice_u_x();
	float* get_lattice_u_y();
	float* get_lattice_u_sqr();
	float* get_lattice_f();
	int*   get_lattice_object();
	float* get_sound_map();
	void get_lattice_size(int& nx, int& ny);
	void set_rad(int value);
	void set_figure(int fig);
	void set_step(int value);
	void set_computing_type(int type);
	void set_tau(float tau);
	void create_fence_solids(int type);
	void clear_liquids();
	void clear_solids();
	void save_sound_map(char*);
	SoundMap* sound_map;

	void testing_of_speed();

	vector<Emitter> emitters;
	vector<Receiver> receivers;
private:
	// LBmethod.cpp
	void initialization_parameters();
	void initialization_speed_rho(); 
	void compute_f_equilibrium(); 
	void compute_u_sqr();
	void initialize_f(); 
	void streaming();
	void solids(); 
	void simple_BC();
	void absorbers();
	void compute_speed_rho(); 
	void collide();
	// field.cpp
	void allocate_memory();
	void clear_memory();
	void initialization();
	void next_step_CPU();
	void next_step_CPU_parallel();
	void isolate_solids();
	void parallel();
	void prelude();
	// verification.cpp
	void constant_frequency_verification();
	void periodicity_verification();
	void circle_verification();
	void wave_damping();

	int Nx;
	int Ny;
	float *f;
	float *fTemp;
	float *fEq;
	float *uX;
	float *uY;
	float *u_sqr;
	int *cell;
	float *rho;
	float c[9][2];
	float w[9];
	float tau;
	unsigned int compute_n_steps;
	unsigned long iteration;	
	int rad;
	unsigned int figure;
	unsigned int computing;
};
