#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtVoronoi.h"
#include "Line.h"
#include "VoronoiDiagram.h"
#include <vector>
#include <qvector.h>
#include <qpoint.h>
#include <qtimer.h>
#include <time.h>
#include <functional>

using namespace std;


class QtVoronoi : public QMainWindow
{
	Q_OBJECT

public:
	QtVoronoi(QWidget *parent = Q_NULLPTR);
	vector<Point> points;
	VoronoiDiagram DivideAndConquare(int l, int r);
	map<Point*, vector<PointLinePair>> related;

	public slots:
	void drawP();
	void runAlgo();
	void clear();
	void motion();
	void motionTimer();

private:
	Ui::QtVoronoiClass ui;
	vector<function<QPoint(QPoint)>> motionFunctions;
	QTimer *timer;
};
