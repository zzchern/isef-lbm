#include "element.h"
#include <vector>

class Layout
{
	private:
		Point A;
		Point B;
		Point C;
		Point D;
		Point E;
		Point F;
		Point G;
		Point H;
		Point I;
		Point J;
		vector<Element*>elements;
	public:
		bool isHelped;
		void flow();
		void swap(bool);
		Element* getActive();
		Layout(int, int);	//screenx, screeny
		void showHelp();
		void refresh();
		void display();
		void add(Element*);
		void free_active();
		void clear();
};
