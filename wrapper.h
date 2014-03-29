/*
  Class "Wrapper" is responsible for saving/loading modeling area.
  Functions:
  - Saving/Loading a field structure to a file.
  - Saving a filed structure to *.bmp file.
  - Loading a field structure from *.bmp file. It's useful to create some field in 
  a some graphics editor, the save it and use it the programm.
 */
#pragma once
#include "field.h"

class Wrapper
{
public:
	static void save(char* path, Field* field);
	static void load(char* path, Field* field);
	static void save_field_structure_bmp(char* bmp_file, Field* field);
	static void load_field_structure_bmp(char* bmp_file, Field* field);
	static void get_pixel_array_from_bmp(char* bmp_file, 
										 int& width, int& height, unsigned int* array);
};
