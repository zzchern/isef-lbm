#pragma once
#include <vector>
using namespace std;

class Receiver
{
public:
		Receiver(float *f, int Nx, int Ny);
		void set_position(int x, int y);
		void next_receive();
		float get_last_value();
		void write_to_file(char* filename);
		void save_wav(const char* file_name, int sample_rate);
private:
		float get_rho();
		void normalize(float* values, unsigned int number);
		int nx;
		int ny;
		float* f;
		int x;
		int y;
		vector<float> val;
};
