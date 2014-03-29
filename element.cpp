#include "element.h"
#include <GL/glut.h>
#include "text.h"
#include <stdio.h>

Info Element::getInfo()
{
	Info info;
	info.description = "element";
	info.name = name;
	info.param1 = "none";
	info.value1 = 0;
	info.param2 = "none";
	info.value2 = 0;
	info.addpar = "none";
	info.addval = 0;
	info.addpar2 = "none";
	info.addval2 = "none";
	info.chvalue = 0;
	return info;
}

void Element::drawName()
{
	char c[12];
	glColor4f(1, 1, 1, 0.5);
	snprintf(c, sizeof(char)*12 , "%s", name);
	putText(c,ltop.x + 2, ltop.y + 8);
}

Point Element::getPosition()
{
	return ltop;
}

void Element::setName(char *n)
{
	name = n;
}

char *Element::getName()
{
	return name;
}

void Element::setCorners(Point A, Point B)
{
	ltop.x = A.x, ltop.y = A.y;
	rbot.x = B.x, rbot.y = B.y;
}

Element::Element(Point A, Point B)
{
	isGraphThrown = false;
	isGraph = false;
	isField = false;
	ltop.x = A.x, ltop.y = A.y;
	rbot.x = B.x, rbot.y = B.y;
}

Point * Element::getCorners()
{
	Point A[2];
	A[0].x = ltop.x;
	A[1].x = rbot.x;
	A[0].y = ltop.y;
	A[1].y = rbot.y;
	return A;
}	
