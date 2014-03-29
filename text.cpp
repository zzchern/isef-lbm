/***
    This file is part of 'isef-lbm'. Program for simulation of acoustic
    waves using Lattice Boltzmann Method.
    
    Copyright (C) 2013  Arkadiy Prigojin, Sergey Dukanov

    'isef-lbm' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'isef-lbm' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***/ 

#include "text.h"

int i=0;

void putText(char* string, int x, int y)
{
	glRasterPos2f(x, y);
	while (*string != '\0')
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *string++);
	}
}

void putText(char* string, int x, int y, int xmax)
{
	i = 0;
	glRasterPos2f(x, y);
	while (*string != '\0')
	{
		i++;
		if(i*8 < xmax)
			glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *string++);
		else
			*string++;
	}
}
