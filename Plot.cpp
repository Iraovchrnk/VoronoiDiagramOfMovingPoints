#include "Plot.h"
#include <qpainter.h>
#include <QPaintEvent>
#include <QDebug>
#include <iostream>
#include <ostream>

using namespace std;

Plot::Plot(QWidget *parent): QWidget(parent) {
    ui.setupUi(this);
    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, Qt::white); //
    this->setAutoFillBackground(true);
    this->setPalette(Pal);

    this->target = QPoint(this->size().width() / 2, this->size().height() / 2);
}

Plot::~Plot()
{
    clearAll();
}

/*
void Plot::resizeEvent(QResizeEvent * event) {
    this->target = QPoint(this->size().width() / 2, this->size().height() / 2);
}
*/

//zoom in (out) on wheel scroll, not working now
/*
void Plot::wheelEvent(QWheelEvent * event)
{
    this->scale_change = event->delta() / 1500.0;
    this->scale_ratio += this->scale_change;
    this->target = QPoint(event->pos().x(), event->pos().y());
    this->repaint();
}
*/

//draw point on mouse left button click


void Plot::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        auto p = event->pos();
        auto add = QPoint(p.x(), this->size().height() - p.y());
        emit pointAdded(add);
        this->drawPoint(add, false);
        //ui.setupUi(this);
        update();
        this->repaint();
    }
}



void Plot::drawPoint(const QPoint & point, bool flag) {
    this->pointsToDraw.push_back(QPoint(point.x(), flag * this->size().height() - point.y()));
}

void Plot::drawLine(const QLine & line) {
    this->linesToDraw.push_back(QLine(line.x1(), /*this->size().height() -*/ line.y1(),
        line.x2(), /*this->size().height() -*/ line.y2()));
}

void Plot::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);

    painter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::SquareCap)); //
    for (const auto & l : this->linesToDraw) {
        int a = this->size().width() / 2 - this->target.x();
        int b = this->size().height() / 2 - this->target.y();
        QLine ll(l.x1() * this->scale_ratio + a, l.y1() * this->scale_ratio + b,
            l.x2() * this->scale_ratio + a, l.y2() * this->scale_ratio + b);
        painter.drawLine(l);
    }

    if (!this->is_points_hidden) {
        painter.setBrush(QBrush(Qt::red)); // red
        for (const auto & p : this->pointsToDraw) {
            int a = this->size().width() / 2 - this->target.x();
            int b = this->size().height() / 2 - this->target.y();
            QPoint pp(p.x() * this->scale_ratio + a, p.y() * this->scale_ratio + b);
            painter.drawEllipse(p, 2, 2);
        }
    }

    painter.end();
}

void Plot::hidePoints() {
    this->is_points_hidden = true;
    this->repaint();
}

void Plot::displayPoints() {
    this->is_points_hidden = false;
    this->repaint();
}

void Plot::clearAll() {
    clearPoints();
    clearLines();
}

void  Plot::clearPoints() {
    this->pointsToDraw.clear();
}
void  Plot::clearLines() {
    this->linesToDraw.clear();
}

QVector<QPoint> Plot::getPoints() {
    return this->pointsToDraw;
}
