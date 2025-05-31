#ifndef DRAWTOOLS_H
#define DRAWTOOLS_H

#include <QRandomGenerator>
#include <QPainterPath>
#include <QPoint>

QRandomGenerator getGen(QString seed);
QPoint distort(QPoint p, QPoint offset, QRandomGenerator gen, int distortPad = 10, int padding = 0);
float randDec(QRandomGenerator gen);

void continuePath(QPainterPath* pth, std::vector<QPoint> ps, QRandomGenerator gen, int mayhem = 5);
QPainterPath genPath(std::vector<QPoint> ps, QRandomGenerator gen, bool closed, int mayhem = 5);

#endif // DRAWTOOLS_H
