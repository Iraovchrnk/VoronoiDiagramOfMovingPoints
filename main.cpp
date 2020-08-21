#include "QtVoronoi.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtVoronoi w;
    w.show();
    return a.exec();
}
