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
