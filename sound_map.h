#pragma once
class SoundMap
{
public:
	SoundMap(int nx, int ny, float* rho, float k);
	~SoundMap();
	void refresh_map();
	float* get_density_map();
	float* get_max_val_map();
	void save_density_map_bmp(const char* path);
	void save_max_val_map_bmp(const char* path);
	void clear_map();
	void work(bool state);
	void find_silent_areas();
	void find_loud_areas();
	void find_max_min_vals();
	float find_arithmetic_mean();
	float find_average_deviation_from_mean();
	void save_bmp_from_pixels(unsigned int *pixels, char* path);

private:
	bool state;
	int nx;
	int ny;
	int iteration;
	float k;
	float* rho;
	float* density_map;
	float* max_val_map;
	short* obj;
	float max_value;
	float min_value;
	float mean_value;
	float deviation_value;
	int t;
};
