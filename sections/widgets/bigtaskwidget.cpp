#include "bigtaskwidget.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainter>

#include <QGraphicsTextItem>

BigTaskWidget::BigTaskWidget(QString nme, Window* window, std::vector<QString> inptodos, QGraphicsItem *parent)
    :TaskWidget(nme, window, inptodos, parent, true)
{
    // Set some flags
    setAcceptHoverEvents(false);
    setAcceptedMouseButtons(Qt::NoButton);
    title->setTextInteractionFlags(Qt::TextEditorInteraction);

    // Copy the font (no other way ): ) and change the font size (then set the font)
    QFont newFont = title->font();
    newFont.setPointSize(24);
    title->setFont(newFont);
}

void BigTaskWidget::updateWidth(int newWid) {
    prepareGeometryChange();
    width = newWid - (padding * 2); // Set the new width!

    updatePath();

    updateChildren(false); // No need to prepare geometry change again

    update();
}
