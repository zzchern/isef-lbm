#include "element.h"
#include <vector>
#include "receiver.h"
#include "field.h"

#ifndef GRAPH
#define GRAPH

const int G_FLOW = 0;
const int G_SLICE = 1;
const int G_FSLICE = 2;
const int G_FFF = 3;

class Graph : public Element 
{
 public:
	Point point;
	void keys(unsigned char, int, int);
	void draw(float);
	void mouse(int, int, int, int){};
	void flow(vector<float>);
	void setType(int){};
	Graph(){};
	Graph(int, int, Point, Point, Point, char*, Field*);
	Info getInfo();
 private:
	Field *field;
	int nRec;
	Point getPosition();
	Receiver *receiver;
	float cval;
	float xmod;
	float fvmax;			//для фичи
	float ymod;			
	vector< float >fvalues; 	//для норм и слайс
	int tvcounter;
	vector< bool >vcounter;
	vector< vector<float> >vvalues; //для слайс и спектр.
	int type;
	void drawf(float);
	void drawsl(float){};
	void drawfsl(float){};
	void drawfft(float){};
	
	void flown(float);
	void flowsl(vector< float >);
	void flowfsl(vector< float >){};
	void flowfft(vector< float >){};
	
};
#endif
