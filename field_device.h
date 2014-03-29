#pragma once
struct FieldDevice
{	
		FieldDevice();
		~FieldDevice();
		int Nx;
		int Ny;
		float *fDev;
		float *uDev;
		float *rhoDev;
		int *cellDev;
		bool is_mem_alloc;
		unsigned int *pixels;
};

void allocate_memory_CUDA(FieldDevice *f, int nx, int ny);
void free_memory_CUDA(FieldDevice *f);
void get_device_properties();
void copy_f_to_device(FieldDevice *fieldDev, float *f);
void copy_f_to_host(FieldDevice *fieldDev, float *f);
void copy_cell_to_device(FieldDevice *fieldDev, int *cell);
void copy_u_to_host(FieldDevice *fieldDev, float *u);
void copy_rho_to_host(FieldDevice *fieldDev, float *rho);
void next_iteration(FieldDevice *fieldDev);
void calculate_u_rho(FieldDevice *fieldDev);
// image:
void create_values_for_image(FieldDevice *fieldDev);
void copy_values_for_image_to_host(FieldDevice *fieldDev, unsigned int* pixels);
