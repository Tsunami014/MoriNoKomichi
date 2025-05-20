#include "bigtaskwidget.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainter>

bigTaskWidget::bigTaskWidget(QString nme, Window* window)
    :TaskWidget(nme, window)
{
    setAcceptHoverEvents(false);
    setAcceptedMouseButtons(Qt::NoButton);
}

void bigTaskWidget::makePath() {
    TaskWidget::makePath();
    purePath = path;
    pureSze = boundingRect().size();
}

void bigTaskWidget::updatePath(int newWid) {
    prepareGeometryChange();
    width = newWid - (padding * 2);

    QSizeF nsze = boundingRect().size();
    float diffX = nsze.width() / pureSze.width();
    float diffY = nsze.height() / pureSze.height();
    for (int idx = 0; idx < path.elementCount(); idx++) {
        QPointF point = purePath.elementAt(idx);
        path.setElementPositionAt(idx, point.x()*diffX, point.y()*diffY);
    }
    update();
}
