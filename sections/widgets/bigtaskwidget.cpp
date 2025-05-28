#include "bigtaskwidget.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainter>

#include <QGraphicsTextItem>
#include <QTextCursor>
#include <QtMath>


bigTaskWidget::bigTaskWidget(QString nme, Window* window, QGraphicsItem *parent)
    :TaskWidget(nme, window, parent)
{
    setAcceptHoverEvents(false);
    setAcceptedMouseButtons(Qt::NoButton);

    static_cast<QGraphicsTextItem*>(extras[0])->setTextInteractionFlags(Qt::TextEditorInteraction);
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

    updateChildren(false);

    update();
}
