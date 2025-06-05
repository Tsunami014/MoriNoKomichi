#include "bigtaskwidget.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainter>

#include <QGraphicsTextItem>

bigTaskWidget::bigTaskWidget(QString nme, Window* window, std::vector<QString> inptodos, QGraphicsItem *parent)
    :TaskWidget(nme, window, inptodos, parent, true)
{
    setAcceptHoverEvents(false);
    setAcceptedMouseButtons(Qt::NoButton);

    auto *it = static_cast<QGraphicsTextItem*>(extras[0]);
    it->setTextInteractionFlags(Qt::TextEditorInteraction);
    QFont newFont = it->font();
    newFont.setPointSize(24);
    it->setFont(newFont);
}

// Make the purePath that is used when updating so updates to the path can't make the path go all wonky
void bigTaskWidget::makePath() {
    TaskWidget::makePath();
    purePath = path;
    pureSze = boundingRect().size();
}

void bigTaskWidget::updatePath(int newWid) {
    prepareGeometryChange();
    width = newWid - (padding * 2);

    // Set the new path to the old path but with the position differing by a good ratio
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
