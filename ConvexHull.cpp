#include "ConvexHull.h"



ConvexHull::ConvexHull()
{
}


ConvexHull::ConvexHull(vector<Point> *p) {
	this->points = p;
}


void ConvexHull::make_cw_ccw() {
	for (int i = 0; i < length; ++i) {
		CH[i]->ccw = CH[(length + i - 1) % length];
		CH[i]->cw = CH[(i + 1) % length];
		vertex.push_back(CH[i]);
	}
}


ConvexHull* ConvexHull::Andrew_monotone_chain(int l, int r) {
	CH.resize(2 * (r - l + 1));
	//CH.resize(r - l + 5);
	int m = 0;
	for (int i = l; i <= r; ++i) {
		while (m >= 2 && cross(*CH[m - 2], *CH[m - 1], (*points)[i]) <= 0) {
			--m;
		}
		//auto e = &(*points)[i];
		CH[m] = &(*points)[i];
		++m;
	}

	auto t = m + 1;
	for (int i = r - 1; i >= l; --i) {
		while (m >= t && cross(*CH[m - 2], *CH[m - 1], (*points)[i]) <= 0) {
			--m;
		}
		CH[m] = &(*points)[i];
		++m;
	}
	--m;
	length = m;
	make_cw_ccw();
	return this;
}


ConvexHull::~ConvexHull()
{
}


double cross(Point po, Point pa, Point pb) {
	double val = (pa.x - po.x)*(pb.y - po.y) - (pa.y - po.y)*(pb.x - po.x);
	return val;
}