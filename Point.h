#pragma once
#include <vector>
using namespace std;
class Point
{
public:
	double x, y;
	bool iscircumcenter = false;
	Point* cw;
	Point* ccw;

	Point();
	Point(double _x, double  _y);
	Point* operator+(const Point other) const;
	Point* operator-(const Point other) const;
	Point* operator/(double d) const;
	Point* operator*(double d) const;
	bool operator==(const Point other) const;
	bool operator!=(const Point other) const;
	bool operator>(const Point other) const;
	bool operator<(const Point other) const;
    ~Point();

};
