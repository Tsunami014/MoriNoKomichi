#ifndef TASKSVIEW_H
#define TASKSVIEW_H

#include "../../window.h"

#include <QFont>
#include <QFontMetrics>
#include <QGraphicsItem>
#include <QMouseEvent>

class TaskWidget : public QGraphicsItem {
public:
    TaskWidget(QString nme, Window* window);

    const static inline int width = 600;
    const static inline int padding = 20;
    const static inline int paddedWid = width + (padding*2);

    QRectF boundingRect() const final override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    void makePath();
    QPainterPath shape() const override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QString name;
    QPainterPath path;
    Window* wind = nullptr;
    bool isHover = false;
    static inline QFont textFont = QFont("Segoe Script", 14);
    static inline QFontMetrics measure = QFontMetrics(textFont);
};

#endif // TASKSVIEW_H
