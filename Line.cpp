#include "Line.h"
#include <stdlib.h>


Line::Line()
{
}

Line::Line(Point* p1, Point* p2) {
	this->p1 = p1;
	this->p2 = p2;

	_p1 = nullptr;
	_p2 = nullptr;
	hole = nullptr;

	avail = true;
}


bool Line::operator==(const Line other) const {
	return (p1 == other.p1 && p2 == other.p2) || (p1 == other.p2 && p2 == other.p1);
}


bool Line::operator!=(const Line other) const {
	return !(*this == other);
}

/*
bool Line::operator<(const Line other) const {
	return LineHash(*this) < LineHash(other);
}
*/

Line::~Line()
{
}


/*
size_t LineHash(const Line & l) {
	return (41 * (41 + l._p1->x) + l._p1->y) + (41 * (41 + l._p2->x) + l._p2->y);
	//return NULL;
}
*/


Point* intersect(const Line line1, const Line line2) {
	auto x1 = line1.p1->x;
	auto y1 = line1.p1->y;
	auto x2 = line1.p2->x;
	auto y2 = line1.p2->y;

	auto x3 = line2.p1->x;
	auto y3 = line2.p1->y;
	auto x4 = line2.p2->x;
	auto y4 = line2.p2->y;

	auto a = x1;
	auto b = x2 - x1;
	auto c = y1;
	auto d = y2 - y1;

	auto f = x4 - x3;
	auto h = y4 - y3;

	if (abs(b * h - d * f) > 1e-10) {
	//if (abs(b * h - d * f) != 0) {
		auto t = ((y4 - y3)*(x3 - x1) + (x4 - x3)*(y1 - y3)) / ((x2 - x1)*(y4 - y3) - (y2 - y1)*(x4 - x3));
		auto s = ((y2 - y1)*(x3 - x1) + (x2 - x1)*(y1 - y3)) / ((x2 - x1)*(y4 - y3) - (y2 - y1)*(x4 - x3));
		if (t >= 0 && t <= 1 && s >= 0 && s <= 1) {
			auto x = a + b * t;
			auto y = c + d * t;
			return new Point(x, y);
		}
	}
	return NULL;
}

Line* biSector(Point* p1, Point* p2) {
	auto mid = *(*p1 + *p2) / 2;
	auto vec = Vector(p1, p2);
	auto p11 = *(*vec.nv * (100000.0)) + *mid;
	auto p22 = *(*vec.nv * (-100000.0)) + *mid;
	return new Line(p11, p22);
}
