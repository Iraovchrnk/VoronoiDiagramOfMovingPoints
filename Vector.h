#pragma once
#include "Point.h"

class Vector
{
public:
	Point *p1, *p2, *v, *nv;
	Vector();
	Vector(Point *p1, Point *p2);
	Point* normal_vector();
	~Vector();
};

