#include <stdio.h>
#include <SDL/SDL.h>
#include "field_device.h"
#include "constants.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

FieldDevice :: FieldDevice()
{
	is_mem_alloc = false;
}

FieldDevice :: ~FieldDevice()
{
	free_memory_CUDA(this);
}

void allocate_memory_CUDA(FieldDevice *f, int nx, int ny)
{
	f -> Nx = nx;
	f -> Ny = ny;
	cudaMalloc( (void**)&(f->fDev),    
		    (f->Nx)*(f->Ny)*Nc*sizeof(float) ); 
	cudaMalloc( (void**)&(f->cellDev), 
		    (f->Nx)*(f->Ny)*sizeof(int) );
	cudaMalloc( (void**)&(f->pixels),   
		    (f->Nx)*(f->Ny)*sizeof(unsigned int) );
	cudaMalloc( (void**)&(f->uDev),   
		    (f->Nx)*(f->Ny)*sizeof(float) );
	cudaMalloc( (void**)&(f->rhoDev),   
		    (f->Nx)*(f->Ny)*sizeof(float) );
	f -> is_mem_alloc = true;
}

void free_memory_CUDA(FieldDevice *f)
{
	cudaFree(f -> fDev);
	cudaFree(f -> cellDev);
	cudaFree(f -> pixels);
	cudaFree(f -> uDev);
	cudaFree(f -> rhoDev);
	f -> is_mem_alloc = false;
}

void get_device_properties()
{ 
	int deviceCount;
	cudaDeviceProp prop;
	cudaGetDeviceCount(&deviceCount);
	if (deviceCount == 0) 
	    fprintf(stderr, "error: no devices supporting CUDA.\n"); 
	for(int dev = 0; dev < deviceCount; ++dev)
	if (cudaGetDeviceProperties(&prop, dev) == cudaSuccess)
	{
		printf("------------DEVICE PROPERTIES------------\n");
		printf("Device name: %s \n", prop.name);
		printf("Multiprocessor count: %d \n", prop.multiProcessorCount);
		printf("Total constant memory: %d \n", prop.totalConstMem);
		printf("Total global memory: %d \n", prop.totalGlobalMem);
		printf("Warp size: %d \n", prop.warpSize);
		printf("Regs per block: %d \n", prop.regsPerBlock);
		printf("-----------------------------------------\n");
	}
}

void copy_f_to_device(FieldDevice *fieldDev, float *f)
{
	int size = fieldDev -> Nx * fieldDev -> Ny * Nc * sizeof(float);
	if(fieldDev -> is_mem_alloc)
		cudaMemcpy(fieldDev -> fDev, f, size, cudaMemcpyHostToDevice);
	else
		printf("Memory isn't allocated.\n");
}

void copy_f_to_host(FieldDevice *fieldDev, float *f)
{
	int size = fieldDev -> Nx * fieldDev -> Ny * Nc * sizeof(float);
	if(fieldDev -> is_mem_alloc)
		cudaMemcpy(f, fieldDev -> fDev, size, cudaMemcpyDeviceToHost);
	else
		printf("Memory isn't allocated.\n");
}

void copy_cell_to_device(FieldDevice *fieldDev, int *cell)
{
	int size = fieldDev -> Nx * fieldDev -> Ny * sizeof(int);
	if(fieldDev -> is_mem_alloc)
		cudaMemcpy(fieldDev -> cellDev, cell, size, cudaMemcpyHostToDevice);
	else
		printf("Memory isn't allocated.\n");
}

void copy_u_to_host(FieldDevice *fieldDev, float *u)
{
	int size = fieldDev -> Nx * fieldDev -> Ny * sizeof(float);
	if(fieldDev -> is_mem_alloc)
		cudaMemcpy(u, fieldDev->uDev, size, cudaMemcpyDeviceToHost);
	else
		printf("Memory isn't allocated.\n");
}

void copy_rho_to_host(FieldDevice *fieldDev, float *rho)
{
	int size = fieldDev -> Nx * fieldDev -> Ny * sizeof(float);
	if(fieldDev -> is_mem_alloc)
		cudaMemcpy(rho, fieldDev->rhoDev, size, cudaMemcpyDeviceToHost);
	else
		printf("Memory isn't allocated.\n");
}

void copy_values_for_image_to_host(FieldDevice *fieldDev, unsigned int* pixels)
{
	int size = fieldDev -> Nx * fieldDev -> Ny * sizeof(unsigned int);
	if(fieldDev -> is_mem_alloc)
		cudaMemcpy(pixels, fieldDev -> pixels, size, cudaMemcpyDeviceToHost);
	else
		printf("Memory isn't allocated.\n");
}
