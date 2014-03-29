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
#include "layout.h"
#include "infobox.h"
#include <iostream>
#include <vector>
#include <GL/glut.h>
#include <stdio.h>

UnitConverter *unit;
Field *field; 
Layout *layout;
Element *box;
bool tpause = false;
bool fullactive = false;

float defaultx;
float defaulty;
float fpixw = 1;
float fpixh = 1;

void intKeys(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_END:
			if(field -> emitters.size() > 0)
				field -> emitters.pop_back();
		break;
		case GLUT_KEY_DOWN:
			layout -> swap(false);
			layout -> refresh();
		break;
		case  GLUT_KEY_UP:
			layout -> swap(true);
			layout -> refresh();
		break;
		default:
			layout -> getActive() -> intKeys(key);
		break;
	}
}

void keys(unsigned char key, int x, int y)  
{
	Point A, B;
	A.x = 0, A.y = 0;
	B.x = 0, B.y = 0;
	switch(key)
	{
		case 'z':
			fullactive =! fullactive;
			layout -> getActive() -> setRect(fullactive);
		break;
		case 'u':
		{
			std::cout<<"Input filename: ";
			char path[16];
			std::cin>>path;
			Wrapper::save(path, field);
		}
		break;
		case 'j':
		{
			//printf("1");
				layout -> clear();
				layout->refresh();
				std::cout<<"Input filename: ";
				char path[16];
				std::cin>>path;
				Wrapper::load(path, field);
				int nx, ny;							
			field -> get_lattice_size(nx, ny);
				//printf("2");
				//field -> create_fence_solids(ABSORBER);
				Display *display;
				unit -> set_material(Air);
		std::cout<<"Input maximal frequency:"<<endl;
		int frequency;
		std::cin>>frequency;
		unit -> set_max_sample_rate(frequency * 2.0);
		std::cout<<"Real x: "<<nx * unit -> get_dx(ProportionalConversion)<<" Real y: "<<ny * unit -> get_dx(ProportionalConversion)<<endl;
		float dt = unit -> get_dt(ProportionalConversion);
		std::cout<<"Real time for one iteration: "<<dt<<endl;
				display = new Display(field, A, B, 0, 1, unit);	
				layout -> add(display);
				layout->refresh();
				layout -> swap(true);
				//printf("3");
		}
		break;
		case 's':
		{
			std::cout<<"Input filename: ";
			char path[16];
			std::cin>>path;
			Wrapper::save_field_structure_bmp(path, field);
		}
		break;
		case 'o':
		{
				//printf("1");
				layout -> clear();
				layout->refresh();
				std::cout<<"Input filename: ";
				char path[16];
				std::cin>>path;
				Wrapper::load_field_structure_bmp(path, field);
				//printf("2");
				//field -> create_fence_solids(ABSORBER);
				Display *display;
				display = new Display(field, A, B, 0, 1);	
				layout -> add(display);
				layout->refresh();
				layout -> swap(true);
				//printf("3");
		}
		break;
		case 'h':
			layout -> isHelped = ! layout -> isHelped;
		break;
		case 27:
			Wrapper::save("previous_session.fld", field);
			exit(0);
		break;
		case 'p':
			tpause =! tpause;
		break;
		case 't':
			Display *display;
			display = new Display(field, A, B, 0, 1);	
			layout -> add(display);
			layout -> refresh();
		break;
		case 'y':
			layout -> free_active();
		break;
		case '1':
			field -> set_computing_type(0);
		break;
		case '2':
			field -> set_computing_type(1);
		break;
		case '3':
			field -> set_computing_type(2);
		break;
		default:
			layout -> getActive() -> keys(key, x, y);
		break;
	}	
	layout -> refresh();
	
}

void mouse(int button, int state, int x, int y)
{
	//std::cout<<"MOUSE DOWN"<<endl;
	x = (int)(x / fpixw);
	y = (int)(y / fpixh);
	if(layout -> getActive() -> isGraph != true)
	{
		layout -> getActive() -> mouse(button, state, x, y);
		if(layout -> getActive() -> isGraphThrown == true)
		{
			if(layout -> getActive() -> returnGraph() != NULL)
				layout -> add(layout -> getActive() -> returnGraph());
			layout -> refresh();
		}
	}
}

void mouse_move(int x,int y)
{
	x = (int)(x / fpixw);
	y = (int)(y / fpixh);
	layout -> getActive() -> mouse_move1(x, y);
}

void mouse_move2(int x,int y)
{
	x = (int)(x / fpixw);
	y = (int)(y / fpixh);
	layout -> getActive() -> mouse_move2(x, y);
}

void show()			//must go on!
{
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_ALPHA_TEST);
  	glEnable(GL_BLEND);
  	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if(fullactive == false)
	{
		layout -> display();
		glBegin(GL_LINES);
		glColor4f(0.3, 0.3, 0.3, 0.7);
		glVertex2f(600 - 600/4, 0);
		glVertex2f(600 - 600/4, 600);
		glVertex2f(0, 600 - 600/4);
		glVertex2f(600 - 600/4, 600 - 600/4);
		glEnd();
		layout -> showHelp();
		box -> import(layout -> getActive() -> getInfo());
		box -> psize(fpixw);
		box -> draw (0);
	}
	else
	{
		Point ltop, rbot;
		ltop.x = ltop.y = 0;
		rbot.x = rbot.y = 600;
		layout -> getActive() -> setCorners(ltop, rbot);
		layout -> getActive() -> draw(1);
	}
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
	if(tpause == false)
	{
		field -> next_step();
		layout -> flow();
	}
	
	glutPostRedisplay();
	glutTimerFunc(0, nextStep, 0);
}

bool initialize(int comptype, int step, int nx, int ny)
{
	try
	{
		defaultx = 600;
		defaulty = 600;
		field = new Field(nx, ny);
		unit = new UnitConverter();
		layout = new Layout(defaultx, defaulty);
		
		Point A, B;
		A.x = 0, A.y = 0;
		B.x = 0, B.y = 0;	

		unit -> set_material(Air);
		std::cout<<"Input maximal frequency:"<<endl;
		int frequency;
		std::cin>>frequency;
		unit -> set_max_sample_rate(frequency * 2.0);
		std::cout<<"Real x: "<<nx * unit -> get_dx(ProportionalConversion)<<" Real y: "<<ny * unit -> get_dx(ProportionalConversion)<<endl;
		float dt = unit -> get_dt(ProportionalConversion);
		std::cout<<"Real time for one iteration: "<<dt<<endl;

		Display *display;
		display = new Display(field, A, B, 0, 1, unit);	
		Point E, F;
		E.x = 0; E.y = defaulty - defaulty/4;
		F.x = defaultx - defaultx/4; F.y = defaulty;
		
		IBox *ibox;
		ibox = new IBox(E, F);

		box = ibox;

		layout->add(display);
		layout->refresh();
	}
	catch(bad_alloc&)
	{
		Wrapper::save("crashed_session.fld", field);	//оно, похоже, вообще бесполезно
		return false;
	}
	return true;
}

int main(int argc, char **argv)
{
	int comptype, step, nx, ny;
	if(argc >= 3)
	{
        	nx = atoi(argv[1]);
		ny = atoi(argv[2]);
		if(argc >= 4)
		{
			step = atoi(argv[3]); 
			if(argc == 5)
				comptype = atoi(argv[4]);
			else
				comptype = 0;
		}	
		else
		{
			step = 4;
			comptype = 0;
		}
    	}	
	else
	{
		nx = ny = 100;
		step = 4;
		comptype = 0;
	}
	printf("%s\n", "Initializing...");
	if(initialize(comptype, step, nx, ny))
	{
		printf("%s\n", "Init success");
	
		glutInit(&argc, argv);
	    	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    		glutInitWindowSize(defaultx, defaulty);
    		glutInitWindowPosition(100, 100);
    		glutCreateWindow("LBM Acoustic Modelling");
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
		glutSpecialFunc(intKeys);			
    		glutTimerFunc(0, nextStep, 0);	
    		glutMainLoop();
		return 0;
	}
	else
	{
		printf("%s\n", "Init failure, something went wrong.");
		return 1;
	}
}
