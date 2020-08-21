#pragma once
#include "Line.h"
class PointLinePair
{
public:
	Point* point;
	Line* line;
	PointLinePair();
	PointLinePair(Point* p, Line* l);
	~PointLinePair();
};

