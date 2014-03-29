#include <GL/glut.h>

#pragma once
#include "wrapper.h"
#pragma once
#include "constants.h"
#include "unit_conversion.h"
#include <vector>
#include "info.h"

class Element
{			
	protected:
		Point ltop;
		Point rbot;
		char *name;
		void drawName();
	public:
		virtual void setRect(bool){};
		bool isRect;
		bool isGraph;
		bool isField;
		bool isGraphThrown;
		virtual void drawGraphPoints(vector<Element*>){}
		virtual void drawGraphNames(vector<Element*>){}
		virtual Point getPosition();
		virtual Element* returnGraph(){};
		virtual void showDemoPoints(Point, Point, Point){};
		virtual void demoflow(Field*){};
		virtual void psize(float){};
		virtual void import(Info){};
		virtual Info getInfo();
		virtual void draw(float){};
		virtual void intKeys(int){};
		virtual void keys(unsigned char, int, int){};
		virtual void mouse(int, int, int, int){};
		virtual void mouse_move1(int, int){};
		virtual void mouse_move2(int, int){};
		virtual void flow(vector<float>){};
		void setName(char*);
		char* getName();
		virtual void setCorners(Point, Point);
		Point* getCorners();
		Element(){};
		Element(Point, Point);
};//of harmony

//  ____________  ____________________________________________________      
// /\ \__ \     \/\ ___/__ \ \ \  _ / __  /\ ___/ __/ _\ __\ ___\ ___/ 	
// \ \/\_\ \ \ \ \ \__ \\/ / /\ \ \ \ \/ /\ \__ \ \_\   \ \/\ ______ \	
//  \  / /__\_\_\_\ \___\_/______\_\_\__//\\ \___\___\_\_\_\ \________\	
//   \/_/___/_/_/_/\/___//_______/___/_//__\\/___/___/_/_/_/\/________/	
