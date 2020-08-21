#pragma once

#include <QWidget>
#include "ui_Plot.h"
#include <qpainter.h>
#include <QPoint>
#include <QLine>
#include <QVector>
#include <QWheelEvent>

class Plot : public QWidget
{
    Q_OBJECT

public:
    Plot(QWidget *parent = Q_NULLPTR);
    void drawPoint(const QPoint & point, bool flag = false);
    void drawLine(const QLine & line);
    void hidePoints();
    void displayPoints();
    void clearAll();
    void clearPoints();
    void clearLines();
    QVector<QPoint> getPoints();
    ~Plot();

signals:
    void pointAdded(QPoint point);



protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    //void wheelEvent(QWheelEvent * event) override;
    //void resizeEvent(QResizeEvent * event) override;

private:
    Ui::Plot ui;
    QVector<QPoint> pointsToDraw;
    QVector<QLine> linesToDraw;
    bool is_points_hidden = false;
    double scale_ratio = 1.0;
    double scale_change = 0.0;
    QPoint target;
};
