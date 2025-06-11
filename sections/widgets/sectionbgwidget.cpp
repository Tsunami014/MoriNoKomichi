#include "sectionbgwidget.h"

#include <QPainter>

SectionBgWidget::SectionBgWidget(uint8_t sect, std::array<QGraphicsItemGroup*, 4>* pgroups, QGraphicsItem* parent) : QGraphicsObject(parent) {
    section = sect;
    groups = pgroups;
}

QRectF SectionBgWidget::boundingRect() const {
    auto group = (*groups)[section];
    return QRectF(group->pos(), group->childrenBoundingRect().size());
}

void SectionBgWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(QPen(QColor(0, 0, 0), 3));
    painter->drawRect(boundingRect());
}
