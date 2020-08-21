#include "QtVoronoi.h"
#include <math.h>


QtVoronoi::QtVoronoi(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.drawPoints, SIGNAL(clicked()), this, SLOT(drawP()));
	connect(ui.motionButton, SIGNAL(clicked()), this, SLOT(motionTimer()));
	connect(ui.buildVD, SIGNAL(clicked()), this, SLOT(runAlgo()));
	connect(ui.clear, SIGNAL(clicked()), this, SLOT(clear()));
	this->timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(motion()));
}

void QtVoronoi::motion() {
	auto pts = ui.widget->getPoints();
	//ui.widget->clearPoints();
	//ui.widget->repaint();
	//this->clear(true == false);
    //auto f = []() { return (1 + rand() % 3) * (rand() & 1 ? 1 : -1); };
	for (int i = 0; i < pts.size(); ++i) {
		ui.widget->drawPoint(motionFunctions[i](pts[i]), false);
	}
	ui.widget->repaint();
	runAlgo();
}

void QtVoronoi::motionTimer() {
	if (timer->isActive()) {
		timer->stop();
		motionFunctions.clear();
	}
	else
	{
		auto f = []() { return  (1 + rand() % 5) * (rand() & 1 ? 1 : -1); };
        for (auto &p : ui.widget->getPoints()) {
			const int dx = f(), dy = f();
			function<QPoint(QPoint)> func = [dx, dy](const QPoint & p) {
				return QPoint(p.x() + dx, p.y() + dy);
			};
			motionFunctions.push_back(func);
		}

		timer->start(150);
	}
}

void QtVoronoi::drawP() {

	int num = ui.spinBox->value();
	//	int wdth = ui.widget->size().width();
	//	int hght = ui.widget->size().height();
	int w = ui.widget->size().width();
	int h = ui.widget->size().height();

	set<pair<int, int>> used;

	vector<int> dx = { 1, 1, -1, -1, 0 , 0, 1, -1 };
	vector<int> dy = { 1, -1, 1, -1, 1, -1, 0, 0 };

	for (int i = 0; i < num; ++i) {
		//ui.widget->drawPoint(QPoint(rand() % wdth, rand() % hght));
		//ui.widget->drawPoint(QPoint(rand() % w, rand() % h));
		int px = rand() % w;
		int py = rand() % h;

		if (used.count({ px, py })) { --i; continue; }

		used.insert({ px,py });
		for (int k = 1; k <= 3; ++k)
			for (int j = 0; j < 8; ++j)
				used.insert({ px + k * dx[j], py + k * dy[j] });
		ui.widget->drawPoint(QPoint(px, py), true);
	//	ui.widget->repaint();
	}
	ui.widget->repaint();
}

void QtVoronoi::runAlgo() {
	int w = ui.widget->size().width();
	int h = ui.widget->size().height();

	auto pts = ui.widget->getPoints();
	if (pts.size() == 0)
	{
		return;
	}

	this->points.clear();
	ui.widget->clearLines();

	for (auto &p : pts) this->points.push_back(Point(p.x(), p.y()));
	sort(points.begin(), points.end());
	this->related.clear();
	auto v = DivideAndConquare(0, this->points.size() - 1);

	vector<Line> output;

//	int w = ui.widget->size().width();
//	int h = ui.widget->size().height();

	vector<Line> box_edges = {
		Line(new Point(0, 0), new Point(w, 0)),
		Line(new Point(0, 0), new Point(0, h)),
		Line(new Point(w, 0), new Point(w, h)),
		Line(new Point(0, h), new Point(w, h)),
	};

    for (size_t i = 0; i < v.lines->size(); ++i) {
		auto ee = v.lines->at(i);
		if (ee->avail) {
			Point* p = NULL;
			auto s = vector<Point*>();
			for (auto edge : box_edges) {
				p = intersect(edge, *ee);
				if (p != NULL) {
					s.push_back(p);
				}
			}
			if (s.size() > 0) {
				auto p1 = ee->p1;
				auto p2 = ee->p2;
				if ((p1->x >= 0 && p1->x <= w) && (p1->y >= 0 && p1->y <= h)) {
					ee->p2 = s[0];
				}
				else if ((p2->x >= 0 && p2->x <= w) && (p2->y >= 0 && p2->y <= h)) {
					ee->p1 = s[0];
				}
				else {
					ee->p1 = s[0];
					ee->p2 = s[1];
				}
			}
		}
	}

    for (size_t i = 0; i < v.lines->size(); ++i) {
		auto ee = v.lines->at(i);
		if (ee->avail) {
			if (ee->p1->x >= 0 && ee->p1->x <= w && ee->p1->y >= 0 && ee->p2->y <= h) {
				output.push_back(*ee);
			}
		}
	}

    for (size_t i = 0; i <output.size(); ++i) {
		auto e = output.at(i);
		ui.widget->drawLine(QLine(e.p1->x, e.p1->y, e.p2->x, e.p2->y));
	}

	//ui.spinBox->setValue(0);
	ui.widget->repaint();

}

void QtVoronoi::clear() {
	ui.widget->clearAll();
	ui.widget->repaint();
}


VoronoiDiagram QtVoronoi::DivideAndConquare(int l, int r) {
	if (r - l + 1 == 3) {
//		auto lower = l;
//		auto upper = r;
		vector<Line*> *lines = new vector<Line*>();
		vector<tuple<int, double, Line*>> dis;
		vector<tuple<Point*, int>> mid;
		int t = 0;
		for (int i = l; i < r; ++i) {
			for (int j = i + 1; j <= r; ++j) {
				lines->push_back(biSector(&points[i], &points[j]));
				related[&points[i]].push_back(PointLinePair(&points[j], lines->back()));
				related[&points[j]].push_back(PointLinePair(&points[i], lines->back()));
				lines->back()->_p1 = &points[i];
				lines->back()->_p2 = &points[j];
				mid.push_back(tuple<Point*, int>(*(points[i] + points[j]) / 2, t));
				dis.push_back(tuple<int, double, Line*>(t, pow(points[i].x - points[j].x, 2)
					+ pow(points[i].y - points[j].y, 2), new Line(&points[i], &points[j])));
				++t;
			}
		}

		auto circumcenter = intersect(*lines->at(0), *lines->at(1));
		if (circumcenter != NULL) {
			vector<tuple<int, double, Line*>> tmp_lines;
			sort(dis.begin(), dis.end(),
				[](const tuple<int, double, Line*> & a,
					const tuple<int, double, Line*> & b) -> bool {	return get<1>(a) < get<1>(b);	});

			string triangle = "acute";
			if (abs(get<1>(dis[0]) + get<1>(dis[1]) - get<1>(dis[2])) < 1e-10) {
				triangle = "right";
				tmp_lines.push_back(dis[0]);
				tmp_lines.push_back(dis[1]);
			}
			else if (get<1>(dis[0]) + get<1>(dis[1]) < get<1>(dis[2])) {
				triangle = "obtuse";
			}

			int s = get<0>(dis[2]);
			t = 0;

			for (int i = l; i < r; ++i) {
				for (int j = i + 1; j <= r; ++j) {
					auto ab = new Line(&points[i], &points[j]);
					auto hl = new Line(lines->at(t)->p1, circumcenter);
					auto res = intersect(*hl, *ab);

					if (!(triangle == "right" && s == t)) {
						if (res == NULL) {
							if (!(triangle == "obtuse" && s == t)) {
								lines->at(t)->p1 = circumcenter;
							}
							else {
								lines->at(t)->p2 = circumcenter;
							}
						}
						else {
							if (!(triangle == "obtuse" && s == t)) {
								lines->at(t)->p2 = circumcenter;
							}
							else {
								lines->at(t)->p1 = circumcenter;
							}
						}
					}
					++t;
				}
			}
			if (triangle == "right") {
				t = get<0>(dis[2]);
				if (intersect(Line(lines->at(t)->p1, circumcenter), *get<2>(tmp_lines[0])) != NULL
					|| intersect(Line(lines->at(t)->p1, circumcenter), *get<2>(tmp_lines[1])) != NULL) {
					lines->at(t)->p1 = circumcenter;
				}
				else {
					lines->at(t)->p2 = lines->at(t)->p1;
					lines->at(t)->p1 = circumcenter;
				}
			}

			circumcenter->iscircumcenter = true;

			lines->at(0)->connected.push_back(lines->at(1));
			lines->at(0)->connected.push_back(lines->at(2));

			lines->at(1)->connected.push_back(lines->at(0));
			lines->at(1)->connected.push_back(lines->at(2));

			lines->at(2)->connected.push_back(lines->at(0));
			lines->at(2)->connected.push_back(lines->at(1));
		}
		else {
			sort(mid.begin(), mid.end(),
				[](const tuple<Point*, int> & a, const tuple<Point*, int> & b) -> bool { return get<1>(a) < get<1>(b);  });

			t = get<1>(mid[1]);
			auto del_line = lines->at(t);
			for (int i = l; i <= r; ++i) {
				auto u = &related[&points[i]];
                for (size_t j = 0; j < u->size(); ++j) {
					if (u->at(j).line == del_line) {
						u->erase(u->begin() + j);
						break;
					}
				}
			}
			lines->erase(lines->begin() + t);
		}
		return VoronoiDiagram(lines, l, r, &points);
	}
	else if (r - l + 1 == 2) {
		auto lower = l;
		auto upper = r;

		auto* line = biSector(&points[lower], &points[upper]);
		line->_p1 = &points[lower];
		line->_p2 = &points[upper];

		related[&points[lower]].push_back(PointLinePair(&points[upper], line));
		related[&points[upper]].push_back(PointLinePair(&points[lower], line));

		vector<Line*> *lines = new vector<Line*>();
		lines->push_back(line);
		return VoronoiDiagram(lines, l, r, &points);
	}
	else if (r - l + 1 == 1) {
		vector<Line*> * lines = new vector<Line*>();
		return VoronoiDiagram(lines, l, r, &points);
	}
	else {
		int mid = (r + l) / 2;
		auto VL = DivideAndConquare(l, mid);
		auto VR = DivideAndConquare(mid + 1, r);
		auto merged = merge(VL, VR, Line(), related);

		return merged;
	}
}
