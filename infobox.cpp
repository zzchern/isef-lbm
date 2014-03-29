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

#include "infobox.h"
#include "text.h"
#include <stdio.h>
#include <math.h>

void IBox::psize(float a)
{
	xmod = a;
}

IBox::IBox( Point A, Point B)
{
	ltop.x = A.x, ltop.y = A.y;
	rbot.x = B.x, rbot.y = B.y;
}

void IBox::import(Info a)
{
	info = a;
}

void IBox::draw(float alpha)
{
	glColor3f(1,1,1);
	char c[64];
	snprintf(c, sizeof(char)*64 , "element : %s", info.name);
	putText(c, ltop.x + 2, ltop.y + (rbot.y-ltop.y)/8, rbot.x * xmod);
	snprintf(c, sizeof(char)*64 , "%s", info.description);
	putText(c, ltop.x + 2, ltop.y + 2 * (rbot.y-ltop.y)/8, rbot.x * xmod);
	if(ceilf(info.value1) != info.value1)
	snprintf(c, sizeof(char)*64 , "%s : %f", info.param1, info.value1);
	else
	snprintf(c, sizeof(char)*64 , "%s : %d", info.param1, (int)info.value1);
	putText(c, ltop.x + 2, ltop.y + 3 * (rbot.y-ltop.y)/8, rbot.x * xmod);
	if(ceilf(info.value2) != info.value2)
	snprintf(c, sizeof(char)*64 , "%s : %f", info.param2, info.value2);
	else
	snprintf(c, sizeof(char)*64 , "%s : %d", info.param2, (int)info.value2);
	putText(c, ltop.x + 2, ltop.y + 4 * (rbot.y-ltop.y)/8, rbot.x * xmod);
	if(ceilf(info.addval) != info.addval)
	snprintf(c, sizeof(char)*64 , "%s : %f", info.addpar, info.addval);
	else
	snprintf(c, sizeof(char)*64 , "%s : %d", info.addpar, (int)info.addval);
	putText(c, ltop.x + 2, ltop.y + 5 * (rbot.y-ltop.y)/8, rbot.x * xmod);
	snprintf(c, sizeof(char)*64 , "%s : %s", info.addpar2, info.addval2);
	putText(c, ltop.x + 2, ltop.y + 6 * (rbot.y-ltop.y)/8, rbot.x * xmod);
	if(ceilf(info.chvalue) != info.chvalue)
	snprintf(c, sizeof(char)*64 , "%s : %f", "mod value", info.chvalue);
	else
	snprintf(c, sizeof(char)*64 , "%s : %d", "mod value", (int)info.chvalue);
	putText(c, ltop.x + 2, ltop.y + 7 * (rbot.y-ltop.y)/8, rbot.x * xmod);
}
