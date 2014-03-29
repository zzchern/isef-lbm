#pragma once
#include "element.h"
#include <math.h>
#include <GL/glut.h>
#include "graph.h"

const int C_BWSCALE = 0;
const int C_RDSCALE = 1;
const int D_RHO = 0;
const int D_USQ = 1;
const int D_VEC = 2;
const int D_MAP = 3;

const int CH_C = 0;
const int CH_D = 1;
const int CH_K = 2;
const int CH_M = 3;
const int CH_RM = 4;
const int CH_LM = 5;

const int CH_CMODE = 6;
const int CH_STEP = 7;
const int CH_RAD = 8;

class Display : public Element
{
	private:
		int step;
		int rad;
		int comp_method;
		float max_color;
		int rmouseobj;
		int lmouseobj;
		int color;	//What is the color of the Night?
				//Black (or which?) , my Brother.
		int displayMethod;
		int chvalue;	
		float dx;
		float dy;
		Field* field;		
		float k;
		float temp_color_max;
		bool mouseFloat;
		Point mouseFloatPosition;
		Point mouseDownPosition;
		Point mouseMovePosition;

		void cmodeslide(bool);
		void stepslide(bool);
		void radslide(bool);

		void chvalue_slide(bool);
		void colorslide(bool);
		void methodslide(bool);
		void rmouseslide(bool);
		void lmouseslide(bool);
		Graph *tgraph;
		int gcounter;
		UnitConverter *unit;
	public:
		void setRect(bool);
		void drawGraphPoints(vector<Element*>);
		void drawGraphNames(vector<Element*>);
		Element* returnGraph();
		void setCorners(Point, Point);
		void showDemoPoints(Point, Point, Point);
		Info getInfo();
		void intKeys(int);
		void keys(unsigned char, int, int);
		void mouse(int, int, int, int);
		void mouse_move1(int, int);
		void mouse_move2(int, int);
		void draw_features();
		void draw_normal(float);
		void draw_soundmap(float);
		void get_color_values_for_rho(float**, int, int, float);
		void get_color_values_for_absvel(float**, int, int);
		void draw(float);
		void setColor(int);
		void setMethod(int);
		void setRhoK(float);
		Display(){};
		Display(Field *, Point, Point, int, int);
		Display(Field *, Point, Point, int, int, UnitConverter *);
};
