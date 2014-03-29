#include "constants.h"
#include "receiver.h"
#include "wav_codec.h"
#include <stdio.h>

Receiver :: Receiver(float* _f, int Nx, int Ny) :
	nx(Nx), ny(Ny), f(_f)
{
	x = nx/2;
	y = ny/2;
	val.clear();
}

float Receiver :: get_rho()
{
	float rho;
	rho = 0.0;
	for(int i = 0; i < Nc; ++i)
		rho += f[(x*ny + y)*Nc + i];
	rho -= start_rho;
	rho /= rho_inc;
	return rho;
}

void Receiver :: set_position(int new_x, int new_y)
{
	x = new_x;
	y = new_y;
}

void Receiver :: next_receive()
{
	float value;
	value = get_rho();
	val.push_back(value);
}

float Receiver :: get_last_value()
{
	return (val.size() ? val[val.size()-1] : 0);
}

void Receiver :: write_to_file(char* filename)
{
	FILE* file;
	file = fopen(filename, "w");
	fprintf(file, "%d\n", val.size());
	for(int i = 0; i < val.size(); ++i)
		fprintf(file, "%0.10f\n", val[i]);
	fclose(file);
}

void Receiver :: save_wav(const char* file_name, int sample_rate)
{
	Codec codec;
	float* values;
	unsigned int samples;
	samples = val.size();
	values = new float[samples];
	for(unsigned int i = 0; i < samples; ++i)
		values[i] = val[i];
	normalize(values, samples);
	codec.array_to_wav(values, file_name, samples, sample_rate);
	delete values;
}

void Receiver :: normalize(float* values, unsigned int number)
{
	float max = -1.0;
	for(unsigned int i = 0; i < number; ++i)
		if(values[i] > max)
			max = values[i];
	for(unsigned int i = 0; i < number; ++i)
		values[i] /= max;
}