/***
    This file is part of 'isef-lbm'. Program for simulation of acoustic
    waves using Lattice Boltzmann Method.
    
    Copyright (C) 2013  Arkadiy Prigojin, Sergey Dukanov

    'isef-lbm' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***/ 

#include "display.h"
#include "graph.h"
#include "infobox.h"
#include <vector>
#include <GL/glut.h>
#include <stdio.h>

Element *active;
Element *second;
Element *box;
Field *field; 

float defaultx;
float defaulty;
float fpixw = 1;
float fpixh = 1;

void keys(unsigned char key, int x, int y)  
{
	//printf(" %i ", key);
	if(key != 27)
	{
		active -> keys(key, x, y);
	}
	else
	{
		Wrapper::save("previous_session.fld", field);
		exit(0);
	}
}

void mouse(int button, int state, int x, int y)
{
	x = (int)(x / fpixw);
	y = (int)(y / fpixh);
	active -> mouse(button, state, x, y);
}

void mouse_move(int x,int y)
{
	x = (int)(x / fpixw);
	y = (int)(y / fpixh);
	active -> mouse_move1(x, y);
}

void mouse_move2(int x,int y)
{
	x = (int)(x / fpixw);
	y = (int)(y / fpixh);
	active -> mouse_move2(x, y);
}

void show()							//must go on!
{
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_ALPHA_TEST);
  	glEnable(GL_BLEND);
  	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	/*Point *A = active -> getCorners();
	A[0].x = A[0].x * fpixw;
	A[0].y = A[0].y * fpixh;
	A[1].x = A[1].x * fpixw;
	A[1].y = A[1].y * fpixh;
	active -> setCorners(A[0], A[1]);*/
	active -> draw (0) ;
	second -> draw (0);
	box -> import(active->getInfo());
	box -> psize(fpixw);
	box -> draw (0);
	glDisable(GL_BLEND);
 	glDisable(GL_ALPHA_TEST);
	glutSwapBuffers();
}

void reshape(int width, int height)
{
	fpixw = (float) width / defaultx ;
	fpixh = (float) height / defaulty;
	glViewport(0, 0, (GLsizei) width, (GLsizei) height);
    	glMatrixMode(GL_PROJECTION);
    	glLoadIdentity();
    	glOrtho(0, width / fpixw, height / fpixh, 0, 1, -1);
    	glMatrixMode(GL_MODELVIEW); 
    	glLoadIdentity();
}

void nextStep(int)
{
	field -> next_step();

	int inx, iny;
	field -> get_lattice_size(inx, iny);
	float *a = field -> get_lattice_rho();
	vector<float>b;
	b.push_back(((a[iny*inx/2 + iny/2] - start_rho ) / max_rho));
	second->flow(b);

	glutPostRedisplay();
	glutTimerFunc(0, nextStep, 0);
}

int main(int argc, char **argv)
{
	defaultx = 300;						//начальные размеры экрана
	defaulty = 300;
	field = new Field(100, 100);				//ТЕСТОВЫЕ ЗАПИЛЫ
	Point A, B, C, D, E, F;					//ПЕРЕПИСАТЬ К ЧЕРТОВОЙ МАТЕРИ
	int nx, ny;
	field -> get_lattice_size(nx, ny);
	A.x = 000;
	A.y = 000;
	B.x = 200;
	B.y = 200;

	C.x = 200;
	C.y = 000;
	D.x = 300;
	D.y = 100;

	E.x = 000;
	E.y = 200;
	F.x = 200;
	F.y = 300;
	
	Display *display;
	display = new Display(field, A, B, 0, 2);	

	Graph *graph;
	graph = new Graph(100, G_FLOW, C, D, "coolgraph");

	IBox *ibox;
	ibox = new IBox(E, F);	

	active = display;
	second = graph;
	
	ibox -> import(active -> getInfo());

	box = ibox;

	glutInit(&argc, argv);
    	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    	glutInitWindowSize(defaultx, defaulty);
	//int x, y;
	//getScreenSize(&x, &y);
    	glutInitWindowPosition(100, 100);
    	glutCreateWindow("LBM SOUNDSCAPES");
	glDisable(GL_DEPTH_TEST);
	glClearColor(0, 0, 0, 1);
    	glMatrixMode(GL_PROJECTION);
    	glLoadIdentity();
    	glutDisplayFunc(show);
	glutKeyboardFunc(keys);				
	glutMouseFunc(mouse);					
	glutMotionFunc(mouse_move);
	glutPassiveMotionFunc(mouse_move2);
	glutReshapeFunc(reshape);				
    	glutTimerFunc(0, nextStep, 0);	
    	glutMainLoop();
	return 0;
}
