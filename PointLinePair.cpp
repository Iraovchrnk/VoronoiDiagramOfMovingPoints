#include "PointLinePair.h"



PointLinePair::PointLinePair()
{
}


PointLinePair::PointLinePair(Point* p, Line* l) {
	this->point = p;
	this->line = l;
}


PointLinePair::~PointLinePair()
{
}
