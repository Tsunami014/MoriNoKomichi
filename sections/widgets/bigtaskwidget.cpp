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

void bigTaskWidget::updateWidth(int newWid) {
    prepareGeometryChange();
    width = newWid - (padding * 2);

    updatePath();

    updateChildren(false);

    update();
}
