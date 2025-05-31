#ifndef TASKSVIEW_H
#define TASKSVIEW_H

#include "../../window.h"

#include <QFont>
#include <QGraphicsObject>
#include <QMouseEvent>

class bigTaskWidget; // Forward reference
class TaskWidget; // Forward reference

class TodoGraphicObject : public QGraphicsObject {
public:
    TodoGraphicObject(QString name, TaskWidget* parent);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QString name;

private:
    static inline QFont* font = nullptr;
    const static inline int thickness = 3;
    const static inline float boxDiff = 0.8f;
};

class TaskWidget : public QGraphicsObject {
    Q_OBJECT
public:
    TaskWidget(QString nme, Window* window, std::vector<QString> todos, QGraphicsItem* parent = nullptr);
    virtual void makePath();

    int width = 600;
    const static inline int padding = 20;

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    bigTaskWidget* toBigWidget();

    void updateChildren(bool prepare = true);

protected:
    QPainterPath shape() const override;

    QTransform paintOutline(QPainter *painter);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    QTransform getExpansionTransform();

    QPainterPath path;
    std::vector<QGraphicsItem*> extras; //< [0]: title
    std::vector<TodoGraphicObject*> todos;

private:
    QString name;
    Window* wind = nullptr;
    bool isHover = false;
    unsigned int lastHei = 0;
};

TaskWidget* MakeTaskWidget(QString nme, Window* window, std::vector<QString> todos, QGraphicsItem* parent = nullptr);

#endif // TASKSVIEW_H
