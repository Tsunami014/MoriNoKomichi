#ifndef SECTIONBGWIDGET_H
#define SECTIONBGWIDGET_H

#include <QGraphicsObject>

class SectionBgWidget : public QGraphicsObject
{
public:
    SectionBgWidget(uint8_t section, std::array<QGraphicsItemGroup*, 4>* groups, QGraphicsItem* parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    uint8_t section;
    std::array<QGraphicsItemGroup*, 4>* groups;
};

#endif // SECTIONBGWIDGET_H
