#include <vector>
#include "point.h"
using namespace std;

class Emitter
{
public:
	Emitter(float* f, int nx, int ny);
	void next_step();
	void set_point_position(int x_pos, int y_pos);
	void set_point_position(Point position);
	void set_line_position(int x1, int y1, int x2, int y2);
	void set_line_position(Point p1, Point p2);
	void set_period(unsigned long value);
	void create_standart_wave(int type);
	void create_special_wave(float* data, unsigned long size);
	void load_wave(char* file);
	void save_wave(char* file);
	void set_frequency(float value);
	void set_speed(float vx, float vy);
	void switch_emitter(bool new_state);
private:
	void movement();
	void emission();
	void rho_increase(float k);
	void set_default_params();

	float* f;
	int field_nx;
	int field_ny;
	bool state;
	int step;
	vector<Point> position;
	float vx;
	float vy;
	unsigned long T;
	float freq;
	float* wave;
};
