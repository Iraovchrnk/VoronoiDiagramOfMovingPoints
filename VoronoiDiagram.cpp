#include "VoronoiDiagram.h"

set<Line*> ray_set;

VoronoiDiagram::VoronoiDiagram()
{

}

VoronoiDiagram::VoronoiDiagram(vector<Line*> *lines, int l, int r, vector<Point> * points, ConvexHull *ch) {
	this->lines = lines;
	this->l = l;
	this->r = r;
	this->points = points;
	this->ch = find_convex();
}


ConvexHull* VoronoiDiagram::find_convex() {
	ConvexHull hull(this->points);
	return hull.Andrew_monotone_chain(this->l, this->r);
}


VoronoiDiagram::~VoronoiDiagram()
{
}


bool isUpperTangent(Point pl, Point pr, bool is_left) {
	if (is_left) {
		return cross(pr, pl, *(pl.ccw)) <= 0;
	}
	return cross(pl, pr, *(pr.cw)) >= 0;
}


bool isLowerTangent(Point pl, Point pr, bool is_left) {
	if (is_left) {
		return cross(pr, pl, *(pl.cw)) >= 0;
	}
	return cross(pl, pr, *(pr.ccw)) <= 0;
}


pair<Line*, Line*> findTangents(VoronoiDiagram VL, VoronoiDiagram VR) {
	auto pl = &(*VL.points)[VL.r];  
	auto pr = &(*VR.points)[VR.l];  


	while (!(isUpperTangent(*pl, *pr, true) && isUpperTangent(*pl, *pr, false))) {
		while (!isUpperTangent(*pl, *pr, true))
		{
			pl = pl->ccw;
		}
		while (!isUpperTangent(*pl, *pr, false))
		{
			pr = pr->cw;
		}
	}

	auto upper_tangent = new Line(pl, pr);

	auto pl_ = &(*VL.points)[VL.r];
	auto pr_ = &(*VR.points)[VR.l];

	while (!(isLowerTangent(*pl_, *pr_, true) && isLowerTangent(*pl_, *pr_, false)))
	{
		while (!isLowerTangent(*pl_, *pr_, true))
		{
			pl_ = pl_->cw;
		}
		while (!isLowerTangent(*pl_, *pr_, false))
		{
			pr_ = pr_->ccw;
		}
	}

	auto lower_tangent = new Line(pl_, pr_);

	return{ upper_tangent, lower_tangent };
}


void recursive_discard_edge(Line * ray, Point * other_point, Point * base_point, bool is_left) {
	for (auto candidate : ray->connected) {
		if (candidate->avail && ray_set.find(candidate) == ray_set.end()) {
			Point* next_base_point = NULL;
			Point* next_other_point = NULL;
			if (*candidate->p1 == *base_point || *candidate->p2 == *base_point) {
				if (*candidate->p1 == *base_point) {
					next_base_point = candidate->p2;
					next_other_point = candidate->p1;
				}
				else {
					next_base_point = candidate->p1;
					next_other_point = candidate->p2;
				}

				if (is_left == false) {
					if (cross(*base_point, *next_base_point, *other_point) >= 0) {
						candidate->avail = false;
						recursive_discard_edge(candidate, next_other_point, next_base_point, false);
					}
				}
				else {
					if (cross(*base_point, *next_base_point, *next_other_point) <= 0) {
						candidate->avail = false;
						recursive_discard_edge(candidate, next_other_point, next_base_point, true);
					}
				}
			}
		}
	}
}
int kk = 0;

tuple<Point*, Point*, Line*> nextPoint(vector<PointLinePair> pool, Line* sg_bisector) {
	tuple<Point*, Point*, Line*> ans;
	bool first = true;

	for (auto candidate : pool) {
		if (candidate.line->avail && sg_bisector->p1 != (candidate.line->hole)) {
			auto res = intersect(*(candidate.line), *sg_bisector);
			if (res != NULL) {
				auto t = tuple<Point*, Point*, Line*>(res, (candidate.point), (candidate.line));  
				if (first) {
					ans = t;
					first = false;
				}
				else {
					if (*get<0>(t) > *get<0>(ans)) {

					}
					else {
						ans = t;
					}
				}
			}
		}
	}

	return ans;
}


void discard_edges(Line * ray, Point * circumcenter, string side, Line * sg_bisector) {
	++kk;
	if (side == "right") {
		if (cross(*circumcenter, *ray->p1, *sg_bisector->p1) >= 0) {
			if (ray->p1->iscircumcenter) {
				recursive_discard_edge(ray, circumcenter, ray->p1, false);
			}
			ray->p1 = circumcenter;
		}
		else {
			if (ray->p2->iscircumcenter) {
				recursive_discard_edge(ray, circumcenter, ray->p2, false);
			}
			ray->p2 = circumcenter;
		}
	}
	else if (side == "left") {
		if (cross(*circumcenter, *ray->p1, *sg_bisector->p1) <= 0) {
			if (ray->p1->iscircumcenter) {
				recursive_discard_edge(ray, circumcenter, ray->p1, true);
			}
			ray->p1 = circumcenter;
		}
		else {
			if (ray->p2->iscircumcenter) {
				recursive_discard_edge(ray, circumcenter, ray->p2, true);
			}
			ray->p2 = circumcenter;
		}
	}
	else { // unreachable?

	}
}

void discard_edges(Line * ray1, Line * ray2, Point * circumcenter, string side, Line * sg_bisector) {
	++kk;
	if (side == "both") {
		if (cross(*circumcenter, *ray1->p1, *sg_bisector->p1) < 0) {
			if (ray1->p1->iscircumcenter) {
				recursive_discard_edge(ray1, circumcenter, ray1->p1, true);
			}
			ray1->p1 = circumcenter;
		}
		else {
			if (ray1->p2->iscircumcenter) {
				recursive_discard_edge(ray1, circumcenter, ray1->p2, true);
			}
			ray1->p2 = circumcenter;
		}

		if (cross(*circumcenter, *ray2->p1, *sg_bisector->p1) > 0) {
			if (ray2->p1->iscircumcenter) {
				recursive_discard_edge(ray2, circumcenter, ray2->p1, false);
			}
			ray2->p1 = circumcenter;
		}
		else {
			if (ray2->p2->iscircumcenter) {
				recursive_discard_edge(ray2, circumcenter, ray2->p2, false);
			}
			ray2->p2 = circumcenter;
		}
	}
	else {// unreachable?

	}
}


VoronoiDiagram merge(VoronoiDiagram VL, VoronoiDiagram VR, Line line, map<Point*, vector<PointLinePair>> &related) {
	
	vector<tuple<Line*, Line*, string, Point*>> clip_lines;

	ray_set.clear();

	auto tangents = findTangents(VL, VR);
	auto upper_tangent = tangents.first;
	auto lower_tangent = tangents.second;

	vector<Line*> HP;

	auto SG = upper_tangent;
	auto px = SG->p1;
	auto py = SG->p2;

	auto sg_bisector = biSector(SG->p1, SG->p2);
	sg_bisector->_p1 = SG->p1;
	sg_bisector->_p2 = SG->p2;

	HP.push_back(sg_bisector);

	Point* circumcenter = NULL;

	bool firsttime = true;
	map<Point*, vector<PointLinePair>> newpl;
	Line * ray = NULL, *ray1 = NULL, *ray2 = NULL;
	while (!(*SG == *lower_tangent)) {
		if (sg_bisector->p1->y > sg_bisector->p2->y) {
			swap(sg_bisector->p1, sg_bisector->p2);
		}
		else if (abs((sg_bisector->p1->y) - (sg_bisector->p2->y)) <= 0.00005) {
			if (sg_bisector->p1->x < sg_bisector->p2->x) {
				swap(sg_bisector->p1, sg_bisector->p2);
			}
		}

		newpl[SG->p1].push_back(PointLinePair(SG->p2, sg_bisector));
		newpl[SG->p2].push_back(PointLinePair(SG->p1, sg_bisector));

		if (!firsttime && circumcenter != NULL) {
			sg_bisector->p1 = circumcenter;
		}

		auto pll = related[px];
		auto prl = related[py];

		auto result_l = nextPoint(pll, sg_bisector);
		auto result_r = nextPoint(prl, sg_bisector);

		string side = "";

		if (get<0>(result_l) != NULL && get<0>(result_r) != NULL) {
			if (abs(get<0>(result_l)->x - get<0>(result_r)->x) <= 1e-10 && abs(get<0>(result_l)->y - get<0>(result_r)->y) <= 1e-10) {
			//if (abs(get<0>(result_l)->x - get<0>(result_r)->x) <= 0. && abs(get<0>(result_l)->y - get<0>(result_r)->y) <= 0.) {
				SG = new Line(get<1>(result_l), get<1>(result_r));
				circumcenter = get<0>(result_l);
				ray1 = get<2>(result_l);
				ray2 = get<2>(result_r);
				side = "both";
			}
			else if (get<0>(result_l)->y > get<0>(result_r)->y) {
				SG = new Line(px, get<1>(result_r));
				circumcenter = get<0>(result_r);
				ray = get<2>(result_r);
				side = "right";
			}
			else if (get<0>(result_l)->y < get<0>(result_r)->y) {
				SG = new Line(get<1>(result_l), py);
				circumcenter = get<0>(result_l);
				ray = get<2>(result_l);
				side = "left";
			}
			else
			{
				qDebug() << "WAC\n";
			}
		}
		else {
			if (get<0>(result_l) != NULL && get<0>(result_r) == NULL) {
				SG = new Line(get<1>(result_l), py);
				circumcenter = get<0>(result_l);
				ray = get<2>(result_l);
				side = "left";
			}
			else if (get<0>(result_l) == NULL && get<0>(result_r) != NULL) {
				SG = new Line(px, get<1>(result_r));
				circumcenter = get<0>(result_r);
				ray = get<2>(result_r);
				side = "right";
			}
			else {
				SG = lower_tangent;
				sg_bisector = biSector(SG->p1, SG->p2);
				sg_bisector->_p1 = SG->p1;
				sg_bisector->_p2 = SG->p2;
				HP.push_back(sg_bisector);
				continue;
			}
		}

		if (ray != NULL || (ray1 != NULL && ray2 != NULL)) {
			if (ray != NULL) {
				ray->hole = circumcenter;
				auto t = tuple<Line*, Line*, string, Point*>(ray, sg_bisector, side, circumcenter);
				if (ray_set.find(ray) == ray_set.end())
					ray_set.insert(ray);
				clip_lines.push_back(t);
			}
			else {
				ray1->hole = circumcenter;
				ray_set.insert(ray1);

				ray2->hole = circumcenter;
				ray_set.insert(ray2);

				auto t1 = tuple<Line*, Line*, string, Point*>(ray1, sg_bisector, side, circumcenter);
				auto t2 = tuple<Line*, Line*, string, Point*>(ray2, sg_bisector, side, circumcenter);

				clip_lines.push_back(t1);
				clip_lines.push_back(t2);
			}
		}

		if (circumcenter != NULL) {
			circumcenter->iscircumcenter = true;
			sg_bisector->p2 = circumcenter;
		}

		px = SG->p1;
		py = SG->p2;

		sg_bisector = biSector(SG->p1, SG->p2);
		sg_bisector->_p1 = SG->p1;
		sg_bisector->_p2 = SG->p2;

		HP.push_back(sg_bisector);
		firsttime = false;
	}

	if (sg_bisector->p1->y > sg_bisector->p2->y) {
		swap(sg_bisector->p1, sg_bisector->p2);
	}
	else if (abs(sg_bisector->p1->y - sg_bisector->p2->y) < 0.00005) {
		if (sg_bisector->p1->x < sg_bisector->p2->x) {
			swap(sg_bisector->p1, sg_bisector->p2);
		}
	}

	newpl[SG->p1].push_back(PointLinePair(SG->p2, sg_bisector));
	newpl[SG->p2].push_back(PointLinePair(SG->p1, sg_bisector));

	for (auto & p : newpl) {
		for (auto & t : p.second) {
			related[p.first].push_back(t);
		}
	}

	if (circumcenter != NULL) {
		sg_bisector->p1 = circumcenter;
	}

    for (size_t i = 0; i < clip_lines.size(); ++i) {
		auto t = clip_lines[i];
		ray = get<0>(t);
		circumcenter = get<3>(t);
		sg_bisector = get<1>(t);
		auto side = get<2>(t);
		if (side != "both") {
			discard_edges(ray, circumcenter, side, sg_bisector);
		}
		else {
			++i;
			auto t1 = clip_lines[i];
			auto ray1 = get<0>(t1);
			discard_edges(ray, ray1, circumcenter, side, sg_bisector);
		}
	}

    for (size_t t = 0, s = 0; t < HP.size() - 1; ++t, ++s) {
		HP[t]->connected.push_back(HP[t + 1]);
		HP[t + 1]->connected.push_back(HP[t]);

		if (s < clip_lines.size()) {
			auto o = clip_lines[s];
			if (get<2>(o) == "both") {
				auto oo1 = get<0>(clip_lines[s]);
				auto oo2 = get<0>(clip_lines[s + 1]);
				oo1->connected.push_back(HP[t]);
				oo1->connected.push_back(HP[t + 1]);
				oo2->connected.push_back(HP[t]);
				oo2->connected.push_back(HP[t + 1]);
				oo1->connected.push_back(oo2);
				oo2->connected.push_back(oo1);

				HP[t]->connected.push_back(oo1);
				HP[t]->connected.push_back(oo2);

				HP[t + 1]->connected.push_back(oo1);
				HP[t + 1]->connected.push_back(oo2);

				++s;
			}
			else {
				auto oo = get<0>(clip_lines[s]);
				oo->connected.push_back(HP[t]);
				oo->connected.push_back(HP[t + 1]);
				HP[t]->connected.push_back(oo);
				HP[t + 1]->connected.push_back(oo);
			}
		}
	}


	auto lines = new vector<Line*>();
									 
    for (size_t i = 0; i < VR.lines->size(); ++i) {
		auto e = VR.lines;
		auto ee = e->at(i);
		lines->push_back(ee);

	}
	
    for (size_t i = 0; i < VL.lines->size(); ++i) {
		auto e = VL.lines;
		auto ee = e->at(i);
		lines->push_back(ee);
	
	}

	for (auto it = HP.begin(); it != HP.end(); ++it) {
		lines->push_back(*it);
	}
	
	int l = VL.l;
	int r = VR.r;
	
	return VoronoiDiagram(lines, l, r, VR.points);
}
