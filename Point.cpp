#include "Point.h"


Point::Point() {}

Point::Point(double _x, double  _y)
{
	this->x = _x;
	this->y = _y;
//	this->iscircumcenter = false;
	this->cw = nullptr;
	this->ccw = nullptr;
}

Point* Point::operator+(const Point other) const {
	return new Point(x + other.x, y + other.y);
}

Point* Point::operator-(const Point other) const {
	return new Point(x - other.x, y - other.y);
}

Point* Point::operator/(double d) const {
	return new Point(x / d, y / d);
}

Point* Point::operator*(double d) const {
	return new Point(x * d, y * d);
}

bool Point::operator==(const Point other) const {
	return x == other.x && y == other.y;
}

bool Point::operator!=(const Point other) const {
	return !(*this == other);
}

bool Point::operator>(const Point other) const {
	return x > other.x || (x == other.x && y > other.y);
}

bool Point::operator<(const Point other) const {
	return x < other.x || (x == other.x && y < other.y);
}

Point::~Point()
{
}

Point nullpoint = Point(-1488, 1488);
