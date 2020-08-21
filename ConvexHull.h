#pragma once
#include "Line.h"
class ConvexHull
{
public:
	vector<Point*> CH, vertex;
	int length = 0;
	vector<Point>* points;

	ConvexHull();
	ConvexHull(vector<Point> * p);
	void make_cw_ccw();
	ConvexHull* Andrew_monotone_chain(int l, int r);
	~ConvexHull();
};

double cross(Point po, Point pa, Point pb);
