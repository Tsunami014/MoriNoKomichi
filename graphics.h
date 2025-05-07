#ifndef PAINTER_H
#define PAINTER_H

#include <QBrush>
#include <QFont>
#include <QPen>
#include <QWidget>

class Graphic {
public:
    Graphic();

public:
    void paint(QPainter *painter, QPaintEvent *event, int elapsed);

private:
    QBrush background;
    QBrush circleBrush;
    QFont textFont;
    QPen circlePen;
    QPen textPen;
};

#endif // PAINTER_H
