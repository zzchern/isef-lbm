#include "element.h"
#include "info.h"

#ifndef INFOBOX
#define INFOBOX

class IBox : public Element 
{
	public:
		IBox(){};
		IBox(Point, Point);
		void draw(float);
		void import(Info);
		void psize(float);
	private:
		float xmod;
		Info info;
};
#endif
