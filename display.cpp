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
#include <stdio.h>
#include <vector>
#include <iostream>
#include "text.h"
#include "receiver.h"
#include "graph.h"

void Display::setRect(bool nValue)
{
	isRect = nValue;
}

void Display::setCorners(Point A, Point B)
{
	ltop.x = A.x, ltop.y = A.y;
	rbot.x = B.x, rbot.y = B.y;
	int nx, ny;						
	field -> get_lattice_size(nx, ny);
	if(nx != ny && isRect == false)
	{
		if(nx < ny)
		{
			dx = (float)ny / (rbot.y - ltop.y);
			ltop.x = ltop.x + ((ny - nx)/2)/dx;
			rbot.x = rbot.x - ((ny - nx)/2)/dx;
		}
		else if(nx > ny)
		{
			dy = (float)nx / (rbot.x - ltop.x);
			ltop.y = ltop.y + ((nx - ny)/2)/dy;
			rbot.y = rbot.y - ((nx - ny)/2)/dy;
		}
	}
	else
	{
		dx = 1;
		dy = 1;
	}
}

void Display::showDemoPoints(Point A, Point B, Point C)
{
	//no points.
}

void Display::radslide(bool up)
{
	if(up == true)
	{	
		rad++;
	}
	else
	{
		if(rad > 1)
			rad--;
	}
	field -> set_rad(rad);
}

void Display::stepslide(bool up)
{
	if(up == true)
	{	
		step++;
	}
	else
	{
		if(step > 1)
			step--;
	}
	field -> set_step(step);
}


void Display::cmodeslide(bool up)
{
	if(up == true)
	{	
		if(comp_method < 3)
			comp_method++;
		else
			comp_method = 0;
	}
	else
	{
		if(comp_method <= 0)
			comp_method = 3;
		else
			comp_method--;
	}
	field -> set_computing_type(comp_method);
}


void Display::chvalue_slide(bool up)
{
	if(up == true)
	{	
		if(chvalue < 8)
			chvalue++;
		else
			chvalue = 0;
	}
	else
	{
		if(chvalue <= 0)
			chvalue = 8;
		else
			chvalue--;
	}
}

void Display::colorslide(bool up)
{
	if(up == true)
	{	
		if(color < 1)
			color++;
		else
			color = 0;
	}
	else
	{
		if(color <= 0)
			color = 1;
		else
			color--;
	}
}

void Display::rmouseslide(bool up)
{
	if(up == true)
	{	
		if(rmouseobj < 3)
		{
			rmouseobj++;
		}
		else
			rmouseobj = 0;
		if(rmouseobj == 2)
			rmouseobj = 3;
	}
	else
	{
		if(rmouseobj <= 0)
			rmouseobj = 3;
		else
		{
			rmouseobj--;
		}
		if(rmouseobj == 2)
			rmouseobj = 1;
	}
}

void Display::lmouseslide(bool up)
{
	if(up == true)
	{	
		if(lmouseobj < 2)
		{
			lmouseobj++;
		}
		else
			lmouseobj = 0;
	}
	else
	{
		if(lmouseobj <= 0)
			lmouseobj = 2;
		else
		{
			lmouseobj--;
		}
	}
}

void Display::methodslide(bool up)
{
	if(up == true)
	{	
		if(displayMethod < 3)
			displayMethod++;
		else
			displayMethod = 0;
	}
	else
	{
		if(displayMethod <= 0)
			displayMethod = 3;
		else
			displayMethod--;
	}
}

void Display::intKeys(int key)
{
	switch(key)
	{
		case GLUT_KEY_LEFT:
			chvalue_slide(true);
		break;
		case GLUT_KEY_RIGHT:
			chvalue_slide(false);
		break;
	}
}

void Display::keys(unsigned char key, int x, int y)  
{
	switch(key)
	{
		case '9':
		{
			int nx, ny;							
			field -> get_lattice_size(nx, ny);
			unit -> display(nx, ny);
		}
		break;
		case '=':
			switch(chvalue)
			{
				case CH_CMODE:
					cmodeslide(true);
				break;
				case CH_STEP:
					stepslide(true);
				break;
				case CH_RAD:
					radslide(true);
				break;
				case CH_C:
					colorslide(true);
				break;
				case CH_D:
					methodslide(true);
				break;
				case CH_K:
					k*=2;
				break;
				case CH_M:
					max_color*=2;
				break;
				case CH_RM:
					rmouseslide(true);
				break;
				case CH_LM:
					lmouseslide(true);
				break;
			}
		break;	
		case '-':
			switch(chvalue)
			{
				case CH_CMODE:
					cmodeslide(false);
				break;
				case CH_STEP:
					stepslide(false);
				break;
				case CH_RAD:
					radslide(false);
				break;
				case CH_C:
					colorslide(false);
				break;
				case CH_D:
					methodslide(false);
				break;
				case CH_K:
					k/=2;
				break;
				case CH_M:
					max_color/=2;
				break;
				case CH_RM:
					rmouseslide(false);
				break;
				case CH_LM:
					lmouseslide(false);
				break;
			}
		break;
		case 'k':
			field -> clear_solids();
			field -> create_fence_solids(ABSORBER);
		break;
		case 'c':
			field -> clear_liquids();
		break;
	}
}

Info Display::getInfo()
{
	Info info;
	info.description = "Display is element that displays field.";
	info.name = name;
	info.param1 = "color";
	info.value1 = color;
	info.param2 = "method";
	info.value2 = displayMethod;
	info.addpar = "rho method k";
	info.addval = k;
	info.addpar2 = "edit";
	switch(chvalue)
	{

		case CH_RAD:
			info.addval2 = "radius";
			info.chvalue = rad;
		break;
		case CH_STEP:
			info.addval2 = "step";
			info.chvalue = step;
		break;
		case CH_CMODE:
			info.addval2 = "acceleration";
			info.chvalue = comp_method;
		break;

		case CH_C:
			info.addval2 = "color";
			info.chvalue = color;
		break;
		case CH_D:
			info.addval2 = "method";
			info.chvalue = displayMethod;
		break;
		case CH_K:
			info.addval2 = "k contrast";
			info.chvalue = k;
		break;
		case CH_RM:
			info.addval2 = "rmouse object";
			info.chvalue = rmouseobj;
		break;
		case CH_M:
			info.addval2 = "max color";
			info.chvalue = max_color;
		break;
		case CH_LM:
			info.addval2 = "lmouse obj";
			info.chvalue = lmouseobj;
		break;
		default:
			info.addval2 = "error";
		break;
	}
	return info;
}

void simple_pressure(Field *field ,int x1, int y1, int x2, int y2, float value)
{
	vector<Point>points;
	Point *pointArray;
	if((x1 != x2 && y1 != y2) || (x1 == x2 || y1 == y2))
	{
		float vec_x = x2 - x1;
		float vec_y = y2 - y1;
		float distance = sqrt(vec_x * vec_x + vec_y * vec_y);
		vec_x /= distance;
		vec_y /= distance;
		Point into;
		for(float i = 0; i < distance; ++i)
		{
			into.x = x1 + vec_x * i;
			into.y = y1 + vec_y * i;
			points.push_back(into);
		}
	}
	if(x1 == x2 && y1 == y2)
	{
		Point into;
		into.x = x1;
		into.y = y1;
		points.push_back(into);
	}
	pointArray = new Point[points.size()];
	for(int i = 0; i < points.size(); ++i)
	{
		pointArray[i] = points[i];
	}
	field->pressure_increase(pointArray, points.size(), value);
	delete pointArray;
}

void putSingleObject(Field *field ,Point objPoint, Point objPoint2, int rmouseobj)
{
	Point *pointArray;
	vector<Point>points;
	if((objPoint.x != objPoint2.x && objPoint.y != objPoint2.y) || (objPoint.x == objPoint2.x || objPoint.y == objPoint2.y))
	{
		float vec_x = objPoint2.x - objPoint.x;
		float vec_y = objPoint2.y - objPoint.y;
		float distance = sqrt(vec_x * vec_x + vec_y * vec_y);
		vec_x /= distance;
		vec_y /= distance;
		Point into;
		for(float i = 0; i < distance; ++i)
		{
			into.x = objPoint.x + vec_x * i;
			into.y = objPoint.y + vec_y * i;
			points.push_back(into);
		}
	}
	if(objPoint.x == objPoint2.x && objPoint.y == objPoint2.y)
	{
		Point into;
		into.x = objPoint.x;
		into.y = objPoint.y;
		points.push_back(into);
	}
	pointArray = new Point[points.size()];
	for(int i = 0; i < points.size(); ++i)
	{
		pointArray[i] = points[i];
	}
	field->set_lattice_object(pointArray, points.size(), rmouseobj);
	delete pointArray;
}

void Display::drawGraphPoints(vector<Element*>graphs)
{
	int nx, ny;							
	field -> get_lattice_size(nx, ny);
	float a = abs(rbot.x - ltop.x);
	float b = abs(rbot.y - ltop.y);
	float rectx = float(a) / float(nx);
	float recty = float(b) / float(ny);
	int x, y;
	glBegin(GL_QUADS);
	glColor3f(1, 1, 0);
	for(int i = 0; i < graphs.size(); i++)
	{
		x = graphs[i] -> getPosition().x;
		y = graphs[i] -> getPosition().y;
		glVertex2f(ltop.x + x * rectx, ltop.y + y * recty);
		glVertex2f(ltop.x + (x + 1) * rectx, ltop.y + y * recty);	
		glVertex2f(ltop.x + (x + 1) * rectx, ltop.y + (y + 1) * recty);
		glVertex2f(ltop.x + x * rectx, ltop.y + (y + 1) * recty);
	}
	glEnd();
}

void Display::drawGraphNames(vector<Element*>graphs)
{
	int nx, ny;							
	field -> get_lattice_size(nx, ny);
	float a = abs(rbot.x - ltop.x);
	float b = abs(rbot.y - ltop.y);
	float rectx = float(a) / float(nx);
	float recty = float(b) / float(ny);
	int x, y;
	glColor4f(1, 1, 0, 0.6);
	for(int i = 0; i < graphs.size(); i++)
	{
		x = graphs[i] -> getPosition().x;
		y = graphs[i] -> getPosition().y;
		putText(graphs[i] -> getName(), ltop.x + x * rectx, ltop.y + y * recty);
	}
}

Element* Display::returnGraph()
{
	isGraphThrown = false;
	return tgraph;
}

int bizzare_counter = 0;

void Display::mouse(int button, int state, int x, int y)
{
	int firstx = x, firsty = y;
	int realx = x - ltop.x;
	int realy = y - ltop.y;
	int nx, ny;							
	field -> get_lattice_size(nx, ny);	
	float a = abs(rbot.x - ltop.x);
	float b = abs(rbot.y - ltop.y);
	float rectx = float(nx) / float(a);
	float recty = float(ny) / float(b);
	x = realx * rectx, y = realy * recty;
	//if(globals -> mouseDownPosition.x <= inx && x <= inx)
	//	simple_pressure(x, y, globals -> mouseDownPosition.x, globals -> mouseDownPosition.y, 1.0);
	Point position;
	position.x = x;
	position.y = y;
	switch(state)
	{
		case GLUT_DOWN:
			mouseFloat = true;
			mouseDownPosition.x = x;
			mouseDownPosition.y = y;
			mouseFloatPosition.x = x;
			mouseFloatPosition.y = y;		
		break;
		case GLUT_UP:	
			switch(button)
			{
				case GLUT_LEFT_BUTTON:
					switch(lmouseobj)
					{
						case 0:
							simple_pressure(field, x, y, mouseDownPosition.x, mouseDownPosition.y, 1.0);
						break;
						case 1:
						{
							if(firstx > ltop.x &&
							   firstx < rbot.x &&
							   firsty > ltop.y &&
							   firsty < rbot.y)
							{
								float* f;
								f = field -> get_lattice_f();
								Emitter e(f, nx, ny);
								putText("Console input",nx/2, ny/2);
								std::cout<<"Input frequency in Hz (-1 to cancel) and type(0-2):"<<endl;
								int freq = 0, type =0, maxfreq = unit -> get_max_sample_rate() / 2;
								std::cout<<"Max frequency : "<<maxfreq<<endl;
								float xspeed, yspeed;
								if(mouseDownPosition.x != x && mouseDownPosition.y != y)
								{
									xspeed = (mouseDownPosition.x - x);
									yspeed = (mouseDownPosition.y - y);
								}
								else
									xspeed = yspeed = 0;
								std::cin>>freq>>type;
								if(freq != -1)
								{
									if(mouseDownPosition.x != x && mouseDownPosition.y != y)
										e.set_line_position(x, y, mouseDownPosition.x, mouseDownPosition.y);
									else
										e.set_point_position(x, y);
									if(freq > maxfreq)
										freq = maxfreq;
									float dt = unit -> get_dt(ProportionalConversion);
									e.set_period((1.0 / freq) / dt );
									e.create_standart_wave(type);
									//e.set_speed(xspeed, yspeed);
									e.set_speed(0.0, 0.0);
									field -> emitters.push_back(e);			
								}
							}
						}
						break;
						case 2:
						{
							if(firstx > ltop.x &&
							   firstx < rbot.x &&
							   firsty > ltop.y &&
							   firsty < rbot.y)
							{
								isGraphThrown = true;
								Point A, B;
								B.x = x;
								B.y = y;
								char *c;
								c = new char[10];
								snprintf(c, sizeof(char)*10 , "graph%i", gcounter);
								//printf(c);
								Graph* graph = new Graph(100, 0, A, A, B, c, field);
								
								tgraph = graph;
								gcounter++;
							}
						}
						break;
					}
				break;
				case GLUT_RIGHT_BUTTON:
					putSingleObject(field, position, mouseDownPosition, rmouseobj); 
				break;
			}
			mouseFloat = false;	
		break;
	}
}

void Display::mouse_move1(int x , int y)
{
	int realx = x - ltop.x;
	int realy = y - ltop.y;
	int nx, ny;							
	field -> get_lattice_size(nx, ny);			
	float a = abs(rbot.x - ltop.x);
	float b = abs(rbot.y - ltop.y);
	float rectx = float(nx) / float(a);
	float recty = float(ny) / float(b);
	x = realx * rectx, y = realy * recty;
	mouseMovePosition.x = x;
	mouseMovePosition.y = y;
	mouseFloatPosition.x = x;
	mouseFloatPosition.y = y;
}

void Display::mouse_move2(int x, int y)
{
	int realx = x - ltop.x;
	int realy = y - ltop.y;
	int nx, ny;							
	field -> get_lattice_size(nx, ny);			
	float a = abs(rbot.x - ltop.x);
	float b = abs(rbot.y - ltop.y);
	float rectx = float(nx) / float(a);
	float recty = float(ny) / float(b);
	x = realx * rectx, y = realy * recty;
	mouseMovePosition.x = x;
	mouseMovePosition.y = y;
}

Display::Display(Field *F, Point A, Point B, int a, int b)
{
	isRect = false;
	tgraph = NULL;
	isGraphThrown == false;
	rad = 4;
	step = 1;
	comp_method = 0;
	gcounter = 0;
	isField = true;
	isGraph = false;
	max_color = 0.0003;
	lmouseobj = 0;
	rmouseobj = SOLID;
	chvalue = 0;
	name = "field";
	k = 0.003;
	field = F;
	ltop.x = A.x, ltop.y = A.y;
	rbot.x = B.x, rbot.y = B.x;
	color = a;
	displayMethod = b;
	temp_color_max= -1;
}

Display::Display(Field *F, Point A, Point B, int a, int b, UnitConverter* unity)
{
	unit = unity;
	isRect = false;
	tgraph = NULL;
	isGraphThrown == false;
	rad = 4;
	step = 1;
	comp_method = 0;
	gcounter = 0;
	isField = true;
	isGraph = false;
	max_color = 0.0003;
	lmouseobj = 0;
	rmouseobj = SOLID;
	chvalue = 0;
	name = "field";
	k = 0.003;
	field = F;
	ltop.x = A.x, ltop.y = A.y;
	rbot.x = B.x, rbot.y = B.x;
	color = a;
	displayMethod = b;
	temp_color_max= -1;
}

void Display::setRhoK(float kk)
{
	k = kk;
}

void Display::setColor(int c)
{
	color = c;
}

void Display::setMethod(int m)
{
	displayMethod = m;
}

void Display::get_color_values_for_rho(float** color_values, int nx, int ny, float k)
{
	float *rho = field -> get_lattice_rho();
	float maxRho;
	float minRho;
	float deltaRho;
	float val;
	minRho = start_rho * (1.0 - k/5.0);
	maxRho = start_rho * (1.0 + k);
	deltaRho = maxRho - minRho;
	for(int x = 0; x < nx; ++x)
	for(int y = 0; y < ny; ++y)
	{
		val = rho[x*ny+y];
		val = (val > maxRho ? maxRho : val);
		val = (val < minRho ? minRho : val);
		color_values[x][y] = (val - minRho) / deltaRho;
	}
}

void Display::get_color_values_for_absvel(float** color_values, int nx, int ny)
{	
	float *usqd = field -> get_lattice_u_sqr();
	float current_color;
	for(int x = 0; x < nx; ++x)
	for(int y = 0; y < ny; ++y)
	{
		current_color = sqrt(usqd[x*ny + y]);
		current_color = (current_color > temp_color_max ? current_color : temp_color_max);
		color_values[x][y] = current_color;
	}
}

void Display::draw_features()
{
	int nx, ny;							
	field -> get_lattice_size(nx, ny);			
	float a = abs(rbot.x - ltop.x);
	float b = abs(rbot.y - ltop.y);
	float rectx;
	float recty;
	if(isRect == true)
	{
		rectx = float(nx * dx) / float(a);
		recty = float(ny * dy) / float(b);
	}
	else
	{
		rectx = float(nx) / float(a);
		recty = float(ny) / float(b);
	}
	//draw mouse line
	int inx, iny;
	field -> get_lattice_size(inx, iny);
	if(mouseFloat == true && (mouseFloatPosition.x > 0 && mouseFloatPosition.y > 0)
			      && ( mouseFloatPosition.x < inx && mouseFloatPosition.y < iny))
	{
		glBegin(GL_LINES);
		glColor3f(1.0,1.0,0.0);
		glVertex2f(ltop.x + (mouseDownPosition.x  + 0.5) / rectx, ltop.y + (mouseDownPosition.y + 0.5) / recty);
		glVertex2f(ltop.x + (mouseFloatPosition.x + 0.5) / rectx, ltop.y + (mouseFloatPosition.y + 0.5) / recty);
		glEnd();
	}
	drawName();
	//draw emitter value
	//draw receiver?
}

void Display::draw_normal(float val)
{
	if (val <= 1) 
		glColor3f(0, val, 1);					
	else if ((val > 1) && (val <= 2))
		glColor3f(0, 1, 2 - val);					
	else if ((val > 2) && (val <= 3))
		glColor3f(val - 2, 1, 0);					
	else
		glColor3f(1, 4 - val, 0);
}

void Display::draw_soundmap(float val)
{
	if(color == C_BWSCALE)	
		glColor3f(val, val, val);
	else
	{
		val *= 4;
		if (val <= 1) 
			glColor3f(0, val, 1);					
		else if ((val > 1) && (val <= 2))
			glColor3f(0, 1, 2 - val);					
		else if ((val > 2) && (val <= 3))
			glColor3f(val - 2, 1, 0);					
		else
			glColor3f(1, 4 - val, 0);
	}
}

void Display::draw(float alpha)
{
	int nx, ny;							//инициализируем размеры поля
	field -> get_lattice_size(nx, ny);				//берем размеры поля из поля
	float a = abs(rbot.x - ltop.x);
	float b = abs(rbot.y - ltop.y);
	
	float rectx = float(a) / float(nx);
	float recty = float(b) / float(ny);

	float **color_values;						//цвета инициализируем
	int *cell = field->get_lattice_object();			//берем значения клеток поля
	color_values = new float*[nx];					//цвета делаем реальными
	for(int i = 0; i < nx; i++)
		color_values[i] = new float[ny];
	temp_color_max = -1;						//иниц минимальный цвет
	float *soundmap = field -> get_sound_map(); 			//берем всякие массивы из поля
	float *ux = field -> get_lattice_u_x();
	float *uy = field -> get_lattice_u_y();
	float *rho = field -> get_lattice_rho();
	glBegin(GL_QUADS);						//поехали.
	if(displayMethod == D_RHO)				
		get_color_values_for_rho(color_values, nx, ny, k);
	if(displayMethod == D_USQ || displayMethod == D_VEC)
		get_color_values_for_absvel(color_values, nx, ny);
	temp_color_max = max_color * 32;					//какие-то константы, которые хорошо бы было бы менять
	for(int x = 0; x < nx; ++x)
	for(int y = 0; y < ny; ++y)
	{
		if(color ==  C_BWSCALE && cell[x*ny + y] == LIQUID)
		{	
			color_values[x][y] /= temp_color_max;	
			glColor3f(color_values[x][y], color_values[x][y], color_values[x][y]);
		}
		else if(cell[x*ny + y] == LIQUID)
		{	
			float temp_color = color_values[x][y] / temp_color_max;
			temp_color = temp_color * 4;
			float val = temp_color;
			draw_normal(val);
		}
		if(displayMethod == D_MAP)
		{	
			float val = soundmap[x*ny + y];
			draw_soundmap(val);
		}
		switch(cell[x*ny + y])
		{
			case SOLID:
			glColor3f(0.0, 0.1, 0.1);
			break;
			case ISOLATED_SOLID:
			glColor3f(0.0, 0.2, 0.2);
			break;
			case ABSORBER:
			glColor3f(0.1, 0.0, 0.1);
			break;
		}
		if(displayMethod != D_VEC)
		{
			glVertex2f(ltop.x + x * rectx, ltop.y + y * recty);
			glVertex2f(ltop.x + (x + 1) * rectx, ltop.y + y * recty);
			glVertex2f(ltop.x + (x + 1) * rectx, ltop.y + (y + 1) * recty);
			glVertex2f(ltop.x + x * rectx, ltop.y + (y + 1) * recty);
		}
		else
		{
			if(cell[x*ny + y] != LIQUID)
			{
				glVertex2f(ltop.x + x * rectx, ltop.y + y * recty);
				glVertex2f(ltop.x + (x + 1) * rectx, ltop.y + y * recty);
				glVertex2f(ltop.x + (x + 1) * rectx, ltop.y + (y + 1) * recty);
				glVertex2f(ltop.x + x * rectx, ltop.y + (y + 1) * recty);
			}
		}
	}
	glEnd();
	glBegin(GL_LINES);
	if(displayMethod ==  D_VEC)
	{
		for(int x = 0; x < nx; ++x)
		for(int y = 0; y < ny; ++y)
		{
			if(cell[x*ny + y] == LIQUID)
			{
				color_values[x][y] /= temp_color_max;	
				glColor3f(color_values[x][y], color_values[x][y], color_values[x][y]);
				glVertex2f(ltop.x + rectx * (x + 1/2) , ltop.y + recty * (y + 1/2));
				glVertex2f(ltop.x + (ux[x*ny+y] * 10000 + x) * rectx, ltop.y + (uy[x*ny+y]* 10000 + y) * recty);
			}
		}
	}
	glEnd();
	draw_features();
	for(int i = 0; i < nx; i++)
		delete color_values[i];
}
