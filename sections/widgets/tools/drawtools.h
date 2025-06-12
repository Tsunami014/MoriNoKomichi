#ifndef DRAWTOOLS_H
#define DRAWTOOLS_H

#include <QRandomGenerator>
#include <QPainterPath>
#include <QPoint>

/*! \brief Get a random number generator with a seed being a string */
QRandomGenerator getGen(QString seed);
/*! \brief Move a point by a random amount */
QPoint distort(QPoint p, QPoint offset, QRandomGenerator gen, int distortPad = 10, int padding = 0);

/*!
    \brief Create a path of roughly a rectangle, with distorted points, rounded edges and a rough path for the edges
*/
QPainterPath* roughRect(QRandomGenerator gen, QSize thisSze, int padding, int corner_radius);

/*!
    \brief Generate a path to add onto the end of a preexisting path

    Path generation takes a bunch of points and joins them with (randomly) either a line or curve.
*/
void continuePath(QPainterPath* pth, std::vector<QPoint> ps, QRandomGenerator gen, int mayhem = 5);
/*!
    \brief Generate a path.

    Path generation takes a bunch of points and joins them with (randomly) either a line or curve.
*/
QPainterPath genPath(std::vector<QPoint> ps, QRandomGenerator gen, bool closed, int mayhem = 5);

#endif // DRAWTOOLS_H
