#include "playerviewwidget.h"
#include "drawtools.h"

#include <QPainter>

playerViewWidget::playerViewWidget(QWidget *parent)
    : QWidget(parent)
{
    bg = new QSvgRenderer(QString(":/assets/player/bg.svg"));
    rend = new QSvgRenderer(QString(path));
}

void playerViewWidget::paintEvent(QPaintEvent *event) {
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::black, 5));
    QRandomGenerator gen = getGen(path);

    int padding = 5;
    QSize thisSze(width()-(padding*2), height()-(padding*2));

    QPainterPath* pth = roughRect(gen, thisSze);

    painter.setClipPath(*pth);
    bg->render(&painter, QRectF(QPointF(0, 0), size()));

    QRectF paintR(QPointF(padding, padding), thisSze);
    rend->render(&painter, paintR);

    painter.setClipRect(QRect(), Qt::NoClip); // Remove clipping
    painter.drawPath(*pth);
    painter.end();

    delete pth;
}
