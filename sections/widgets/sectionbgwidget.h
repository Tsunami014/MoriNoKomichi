#ifndef SECTIONBGWIDGET_H
#define SECTIONBGWIDGET_H

#include <QGraphicsObject>
#include <QPainterPath>

class SectionBgWidget : public QGraphicsObject
{
public:
    SectionBgWidget(uint8_t section, std::array<QGraphicsItemGroup*, 4>* groups, QGraphicsItem* parent = nullptr);

    QRectF boundingRect() const override;
    /*! \brief Paint the edge path */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    /*! \brief Create the edge path */
    void makePath();

private:
    /*! \brief The section number this widget is for */
    uint8_t section;
    /*! \brief A pointer to the groups. Used for checking bounding box positions and sizes */
    std::array<QGraphicsItemGroup*, 4>* groups;
    /*! \brief The last size used for making the path. Used to avoid remaking the path each frame */
    QRectF lastSze = {0, 0, 0, 0};
    /*! \brief The path for the outside */
    QPainterPath pth;
};

#endif // SECTIONBGWIDGET_H
