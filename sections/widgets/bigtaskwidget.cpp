#include "bigtaskwidget.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainter>

#include <QGraphicsTextItem>

bigTaskWidget::bigTaskWidget(QString nme, Window* window, QGraphicsItem *parent)
    :TaskWidget(nme, window, parent)
{
    setAcceptHoverEvents(false);
    setAcceptedMouseButtons(Qt::NoButton);

    QGraphicsTextItem *it = new QGraphicsTextItem(this);
    it->setPlainText(nme);
    it->setTextInteractionFlags(Qt::TextEditorInteraction);
    it->show();
    extras.push_back(it);
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

void bigTaskWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QTransform prevTrans = paintOutline(painter);

    // Reset transform
    painter->setTransform(prevTrans);
}
