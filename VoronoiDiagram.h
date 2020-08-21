#pragma once
#include "Line.h"
#include "ConvexHull.h"
#include <set>
#include <unordered_set>
#include "PointLinePair.h"
#include <tuple>
#include <map>
#include <qdebug.h>

class VoronoiDiagram
{
public:
	vector<Point> *points;
	vector<Line*> *lines;
	int l, r;

	ConvexHull *ch;

	VoronoiDiagram();
    VoronoiDiagram(vector<Line*> * lines, int l, int r, vector<Point> * points, ConvexHull *ch = nullptr);
	ConvexHull* find_convex();
	~VoronoiDiagram();
};

bool isUpperTangent(Point pl, Point pr, bool is_left);
bool isLowerTangent(Point pl, Point pr, bool is_left);
pair<Line*, Line*> findTangents(VoronoiDiagram VL, VoronoiDiagram VR);
void recursive_discard_edge(Line * ray, Point * other_point, Point * base_point, bool is_left);
void discard_edges(Line * ray, Point * circumcenter, string side, Line * sg_bisector);
void discard_edges(Line * ray1, Line * ray2, Point * circumcenter, string side, Line * sg_bisector);
//tuple<Point*, Point*, Line*> nextPoint(vector<PointLinePair> pool, Line sg_bisector);
VoronoiDiagram merge(VoronoiDiagram VL, VoronoiDiagram VR, Line line, map<Point*, vector<PointLinePair>> & related);

