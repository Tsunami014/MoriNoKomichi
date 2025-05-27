#ifndef TASKSVIEW_H
#define TASKSVIEW_H

#include "../../window.h"

#include <QFont>
#include <QFontMetrics>
#include <QGraphicsObject>
#include <QMouseEvent>

class bigTaskWidget; // Forward reference

class TaskWidget : public QGraphicsObject {
    Q_OBJECT
public:
    TaskWidget(QString nme, Window* window, QGraphicsItem* parent = nullptr);
    virtual void makePath();

    int width = 600;
    const static inline int padding = 20;

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    bigTaskWidget* toBigWidget();

protected:
    QPainterPath shape() const override;

    QTransform paintOutline(QPainter *painter);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    QPainterPath path;

private:
    QString name;
    Window* wind = nullptr;
    bool isHover = false;
    static inline QFont textFont = QFont("Segoe Script", 14);
    static inline QFontMetrics measure = QFontMetrics(textFont);
};

TaskWidget* MakeTaskWidget(QString nme, Window* window, QGraphicsItem* parent = nullptr);

#endif // TASKSVIEW_H
