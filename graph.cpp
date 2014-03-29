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

#include "graph.h"
#include <GL/glut.h>
#include "math.h"
#include <stdio.h>
#include <iostream>
#include "text.h"

Info Graph::getInfo()
{
	Info info;
	info.description = "Graph is element which displays dynamics of the field";
	info.name = name;
	info.param1 = "current value";
	info.value1 = cval;
	info.param2 = "type";
	info.value2 = type;
	info.addpar = "max value";
	info.addval = fvmax;
	info.addpar2 = "edit value";
	info.addval2 = "none";
	info.chvalue = 0;
	return info;
}

Graph::Graph(int length, int t, Point A, Point B, Point C, char *n, Field* fi)
{
	field = fi;
	isGraph = true;	
	isGraphThrown = false;
	float* f;
	f = field -> get_lattice_f();

	int nx, ny;							
	field -> get_lattice_size(nx, ny);		

	Receiver nreceiver(f, nx, ny);
	nreceiver.set_position(C.x, C.y);
	
	field -> receivers.push_back(nreceiver);
	nRec = field -> receivers.size();				// тлоуктпжлкуотжлкуоптужколтужктжуоктпжуктужптокжл
	receiver = &field -> receivers[nRec];
	//printf("%i", nRec);	

	point.x = C.x;
	point.y = C.y;
	cval = 0;
	name = n;
	ymod = 0;
	tvcounter = 0;
	ltop.x = A.x, ltop.y = A.y;
	rbot.x = B.x, rbot.y = B.y;
	for(int i=0; i < length; i++)
	{
		fvalues.push_back(0);
	}
	for(int i=0; i < length; i++)
	{
		vcounter.push_back(false);
	}
	for(int i=0; i < length; i++)
	{
		vvalues.push_back(fvalues);
	}
	type = t;
} 

void Graph::keys(unsigned char key, int x, int y)  
{
	switch(key)
	{
		case 'r':
			std::cout<<"Filename/path:";
			char path[16];
			std::cin>>path;
			std::cout<<endl<<"Sample rate:";
			int rate;
			std::cin>>rate;
			//receiver -> write_to_file(path);
			if(receiver != NULL)
				receiver -> save_wav(path, rate);
			else
				std::cout<<"receiver was BAD, cancelled"<<endl;
		break;
		//case 'u':
		
	}
}

void Graph::flow(vector<float> a)
{
	if(a.size()!=0)
	switch(type)
	{
		case G_FLOW:
			flown(a[0]);
			break;
		case G_SLICE:
			flowsl(a);
      			break;
    		case G_FSLICE:
      			flowfsl(a);
      			break;
    		case G_FFF:
      			flowfft(a);
      			break;
    	}
}

void Graph::flown(float a)
{
	int inx, iny;
	field -> get_lattice_size(inx, iny);
	float *fld = field -> get_lattice_rho();
	a = ((fld[iny*point.x + point.y] - start_rho ) / max_rho);
	//a = field -> receivers[nRec].get_last_value();
	cval = a;
	float fmax = 0;
	float fmin = 0;
	a *= 1000;
	tvcounter ++;
	for(int i = fvalues.size(); i > 0; --i)
		fvalues[i] = fvalues[i-1];
	for(int i = vcounter.size(); i > 0; --i)
		vcounter[i] = vcounter[i-1];
	for(int i = 0; i < fvalues.size(); ++i)
	{
		if(fvalues[i] > fmax)
			fmax = fvalues[i];
		if(fvalues[i] < fmin)
			fmin = fvalues[i];
	}
	if(- fmin > fmax)
	{
		fmax = -fmin;
		fvmax = -fmax;
	}
	else
		fvmax = fmax;
	if(fmax < 1) fmax = 1;
	float z = abs(rbot.x - ltop.x) / 100;
	if(z < 1) z = 1;
	ymod = (float)(4000) * z / fmax / ((abs(rbot.y - ltop.y))/2 - 5);	
	//ymod = (float) 1000 / fmax / (ltop.y - 5);	
	fvalues[0] = a;	
	if(tvcounter >= abs(rbot.x - ltop.x)/16)
	{
		tvcounter = 0;
		vcounter[0] = true;
	}	
	else
		vcounter[0] = false;
}

void Graph::flowsl(vector< float > a)
{

}

void Graph::draw(float a)
{
  	switch(type)
    	{
   		case G_FLOW:
      			drawf(a);
      			break;
    		case G_SLICE:
      			drawsl(a);
      			break;
    		case G_FSLICE:
      			drawfsl(a);
      			break;
    		case G_FFF:
      			drawfft(a);
      			break;
    	}
	
	drawName();
}

void Graph::drawf(float alpha)
{
	int inx = abs(rbot.x - ltop.x);
	int iny = abs(rbot.y - ltop.y);
	xmod = (float)inx / fvalues.size();

	int pixsize = 1;
	
	glBegin(GL_LINES);
	glColor3f(0.01, 0.01, 0.01);
	for(int i = 0; i < vcounter.size() + 1; ++i)
	{	
		if(vcounter[i] == true)
		{
			glVertex2f(rbot.x - i * xmod , ltop.y);
			glVertex2f(rbot.x - i * xmod , rbot.y);
		}		
	}
	glEnd();

	glBegin(GL_LINES);
	if(fvmax * ymod < 100)
	for(float i = 0; i < 0.1; i+=0.001)
	{	
			if(int(i*1000) % (int)(0.01 * 1000) == 0)
				glColor3f(0.1, 0.1, 0.1);
			else
				glColor3f(0.02, 0.02, 0.02);
			if(int(i*1000) % (int)(0.1 * 1000) == 0)
				glColor3f(0.3, 0.3, 0.3);
			if(ltop.y + iny/2 - i * ymod * 1000 > ltop.y)
			{
				glVertex2f(ltop.x , ltop.y + iny/2 + i * ymod * 1000);
				glVertex2f(rbot.x,  ltop.y + iny/2 + i * ymod * 1000);	
			}
			if(ltop.y + iny/2 + i * ymod * 1000 < rbot.y)
			{
				glVertex2f(ltop.x ,  ltop.y + iny/2 - i * ymod * 1000);
				glVertex2f(rbot.x,  ltop.y + iny/2 - i * ymod * 1000);	
			}	
	}
	else
	for(float i = 0; i < fvmax * ymod / 1000; i+=0.001)
	{	
			if(int(i*1000) % (int)(0.01 * 1000) == 0)
				glColor3f(0.1, 0.1, 0.1);
			else
				glColor3f(0.02, 0.02, 0.02);
			if(int(i*1000) % (int)(0.1 * 1000) == 0)
				glColor3f(0.3, 0.3, 0.3);
			if(ltop.y + iny/2 - i * ymod * 1000 > ltop.y)
			{
				glVertex2f(ltop.x , ltop.y + iny/2 + i * ymod * 1000);
				glVertex2f(rbot.x,  ltop.y + iny/2 + i * ymod * 1000);	
			}
			if(ltop.y + iny/2 + i * ymod * 1000 < rbot.y)
			{
				glVertex2f(ltop.x ,  ltop.y + iny/2 - i * ymod * 1000);
				glVertex2f(rbot.x,  ltop.y + iny/2 - i * ymod * 1000);	
			}	
	}
	glEnd();


	glBegin(GL_LINES);
	glColor3f(0.2, 0.0, 0);
		glVertex2f(ltop.x , ltop.y + iny/2 - fvmax * ymod);
		glVertex2f(rbot.x,  ltop.y + iny/2 - fvmax * ymod);	
	glEnd();

	char c[16];
	if(fvmax >= 0)
		snprintf(c, sizeof(char)*16 , "max : %f", fvmax/100);
	else
		snprintf(c, sizeof(char)*16 , "min : %f", fvmax/100);
	putText(c,ltop.x + 2, rbot.y - 2);
	
	glBegin(GL_LINE_STRIP);
	glColor3f(1, 1, 1);
	for(int i = 0; i < fvalues.size() + 1; ++i)
	{	
		glVertex2f(rbot.x - i * xmod, ltop.y + iny/2 - fvalues[i] * ymod);
	}
	glEnd();
}

Point Graph::getPosition()
{
	return point;
}
