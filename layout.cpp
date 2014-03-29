#include "layout.h"
#include <stdio.h>
#include "text.h"
#include <GL/glut.h>

void Layout::clear()
{
	elements.clear();
}

void Layout::showHelp()
{
	if(isHelped)
	{
	glColor3f(0.7, 0.7, 0.7);
	char *a = "Help:";
	putText(a, A.x + (J.x - A.x)/10, A.y + (J.y - A.y)/10); 
	a = "To change editing value hit 'a' or 'd', to edit";
	putText(a, A.x + (J.x - A.x)/10 , A.y + (J.y - A.y)/10 + 30); 
	a = "value hit 'w' or 's'";
	putText(a, A.x + (J.x - A.x)/10 , A.y + (J.y - A.y)/10 + 30 + 20); 
	a = "To change active element hit 'z' or 'x'";
	putText(a, A.x + (J.x - A.x)/10 , A.y + (J.y - A.y)/10 + 30 + 20 * 2); 
	a = "To set emitter choose lobject and set it's value to 1,";
	putText(a, A.x + (J.x - A.x)/10 , A.y + (J.y - A.y)/10 + 30 + 20 * 3); 
	a = "then put emitter by click and input waveform and period value";
	putText(a, A.x + (J.x - A.x)/10 , A.y + (J.y - A.y)/10 + 30 + 20 * 4); 
	a = "To set graph point set lobject to value 2 and click";
	putText(a, A.x + (J.x - A.x)/10 , A.y + (J.y - A.y)/10 + 30 + 20 * 5); 
	a = "To save receiver's data into wav file hit 'r' and ";
	putText(a, A.x + (J.x - A.x)/10 , A.y + (J.y - A.y)/10 + 30 + 20 * 6); 
	a = "set path and byterate in console.";
	putText(a, A.x + (J.x - A.x)/10 , A.y + (J.y - A.y)/10 + 30 + 20 * 7); 
	glColor3f(0.9, 0.9, 0.9);
	a = "To kill active element hit 'y', to add new display hit 't'";
	putText(a, A.x + (J.x - A.x)/10 , A.y + (J.y - A.y)/10 + 30 + 20 * 8); 
	a = "To save field hit 'l', to open field hit 'o'";
	putText(a, A.x + (J.x - A.x)/10 , A.y + (J.y - A.y)/10 + 30 + 20 * 9); 
	}
}

Layout::Layout(int fx, int fy)
{
	A.x = 0; A.y = 0;
	B.x = fx - fx/4; B.y = fy - fy/4;

	C.x = B.x; C.y = 0;
	D.x = fx; D.y = fy/4;

	E.x = C.x; E.y = fy/4;
	F.x = fx; F.y = fy - 2 * fy/4;

	G.x = C.x; G.y = fy - 2 * fy/4;
	H.x = fx; H.y = B.y;

	I.x = B.x; I.y = B.y;
	J.x = fx; J.y = fy;
}

Element* Layout::getActive()
{
	return elements[0];
}

void Layout::refresh()
{
	if(elements.size() > 0)
	{
		
		elements[0]->setCorners(A, B);
		if(elements.size() > 1)
		{
			elements[1]->setCorners(C, D);
			if(elements.size() > 2)
			{
				elements[2]->setCorners(E, F);
				if(elements.size() > 3)
				{
					elements[3]->setCorners(G, H);
					if(elements.size() > 4)
						elements[4]->setCorners(I, J);
				}
			}
		}
	}				
}

void Layout::swap(bool plus)
{
	//printf("\n%d", elements.size());
	if(plus == true)
	{
		elements.insert(elements.begin(),elements[elements.size()]);
		elements.pop_back();
	}
	else
	{
		elements.push_back(elements[0]);
		elements.erase(elements.begin());
	}
}

void Layout::display()
{
	vector<Element*> graphs;
	for(int i = 0; i < elements.size(); i++)
	{
		if(i < 5 && elements[i] != NULL)
			elements[i]->draw(0);
	}
	for(int i = 0; i < elements.size(); i++)
	{
		if(elements[i] != NULL && elements[i]->isGraph == true)
		{
			graphs.push_back(elements[i]);
		}
	}
	if(graphs.size() != 0 )
		elements[0] -> drawGraphNames(graphs); 
	for(int i = 0; i < elements.size(); i++)
	{
		if(elements[i]->isField == true && i < 5 && elements[i] != NULL)
		{
			if(graphs.size() != 0)
				elements[i] -> drawGraphPoints(graphs);
		}
	}
	if(elements.size() > 5)
	{
		glBegin(GL_POLYGON);
		glColor4f(0.7, 0.7, 0.7, 0.6);
		glVertex2f(I.x - (I.x - J.x)/2 - 10, J.y - 7);
		glVertex2f(I.x - (I.x - J.x)/2, J.y - 2);
		glVertex2f(I.x - (I.x - J.x)/2 + 10, J.y - 7);
		glEnd();
	}
}

void Layout::flow()
{
	vector<float> a;
	a.push_back(0);
	for(int i = 0; i < elements.size(); i++)
	{
		elements[i] -> flow(a);
	}
}

void Layout::add(Element* A)
{
	elements.push_back(A);
	for(int i = 0; i < elements.size(); i++)
	{
		elements.push_back(elements[0]);
		elements.erase(elements.begin());
	}
}

void Layout::free_active()
{
	elements.erase(elements.begin());
}
