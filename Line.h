#pragma once
#include "Point.h"
#include "Vector.h"
#include <vector>
using namespace std;
class Line
{
public:
	Point* p1, *p2;
	Point* _p1, *_p2, *hole;
	bool avail;
	vector<Line*> connected;



	Line();
	Line(Point *p1, Point *p2);
	bool operator==(const Line other) const;
	bool operator!=(const Line other) const;
	//bool operator<(const Line other) const;
	~Line();
};

Point* intersect(const Line line1,const Line line2);
Line* biSector(Point* p1, Point* p2);


//size_t LineHash(const Line & l);