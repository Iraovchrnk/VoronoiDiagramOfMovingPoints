#include "Vector.h"

Vector::Vector() {}

Vector::Vector(Point *p1, Point *p2)
{
	this->p1 = p1;
	this->p2 = p2;
	v = *p2 - *p1;
	nv = normal_vector();
}


Point* Vector::normal_vector() {
	double x = v->y;
	double y = v->x;
	x *= -1;
	return new Point(x, y);
}


Vector::~Vector()
{
}
